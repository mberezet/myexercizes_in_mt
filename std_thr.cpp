#include <thread>
#include <iostream>
#include <vector>

using namespace std ;

/////////////////////////////////////
//  class who's member f is thr-driver
class Executor {
public:
	Executor() {
		std::cout << "c-executor" << std::endl ;
	}

	Executor(const Executor& e) {
		std::cout << "cc-executor" << std::endl ;
	}

	void  parall_calc(int x)
	      { std::cerr << "calc:" << ++x << std::endl ; }
} ;

//////////////////////////////////////////
int main(int ac, char** av)
{
	//ex-1 ///////////////////////////
	//   thread driver is a class member function.
	//   uses bind feature of c++
	vector<Executor> executors(100, Executor()) ;
	vector<thread> ths(executors.size()) ;

	for (int x=0 ; x< executors.size(); ++x)
   	{
	     thread t(&Executor::parall_calc, &executors[x], /*binding part*/
		      x /*param passing to executor*/) ;
              ths[x] = move(t) ;
	}

	for (int x=0 ; x< executors.size(); ++x)
		ths[x].join() ;
}
