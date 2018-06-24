#include <algorithm>
#include <numeric>
#include <thread>
#include <vector>
#include <iostream>
#include <iterator>
#include <unistd.h>

using namespace std ;

const unsigned long IniBlockSz = 32 ;

///////////////////////////////////////////
template <typename T, typename IT>
void accum_block(IT beg, IT end, T& reslt)
{
   reslt = std::accumulate(beg, end, reslt);
}	

//////////////////////////<long>////////////
template <typename T, typename IT>
T mt_accumulate(IT beg, IT end, T init)
{
	const unsigned long len = distance(beg, end) ;	
	if ( !len)
		return init ;

	cerr << "length to accumulate=" << len << endl ;

	auto blocks = len / IniBlockSz;
	blocks = blocks ? blocks : 1 ;

	auto vcores = thread::hardware_concurrency() ;
	auto th_pool = min<decltype(blocks)>(vcores, blocks) ;
	auto blksz_perth = len / th_pool ; 

	cerr << "thread pool=" << th_pool << " + 1(m); "
	     << "blk_size=" << blksz_perth
	     << "; hrdw_threads=" << vcores << endl ;

	vector<std::thread> threads(th_pool) ;
	vector<T>           results(th_pool) ;
	IT b = beg ;
	IT e = b ;
	for (unsigned int x = 0; x < th_pool; ++x)
	{
		advance(e, blksz_perth) ;
		//threads is a vector, which fr. c++11 understands move semantics, needed for threads contianer
		threads[x] = thread( [=, &results]()->void
			       	     {
				        results[x] = std::accumulate<IT,T>(b ,e, results[x]);
                                     } ) ;
		b = e ;
	}
	for (auto& t : threads)
		t.join() ;

	// main th work
	T reslt_main{} ;
	accum_block<T,IT>(b, end, ref(reslt_main)) ;
	accum_block<T,IT>(results.begin(), results.end(), reslt_main) ;

	return reslt_main ;
}

/////////////////////////
int main(int ac, char** av)
{
	vector<long> v(1000, 5) ;

	long reslt = mt_accumulate(v.begin(), v.end(), 0L) ;
	cerr << "Accumulator=" << reslt << endl ;
}
