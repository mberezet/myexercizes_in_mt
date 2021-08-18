#include <thread>
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

///////////////////////////////
int main()
{
auto t_beg = chrono::system_clock::now() ;

// those are 'definition' for lazy and actual invocation and
//       future preparation for hungry.
auto async_lazy   = std::async( std::launch::deferred, 
		               [](){return chrono::system_clock::now();}
		             ); 

auto async_hungry = std::async( std::launch::async, 
		               [](){return chrono::system_clock::now();}
		             ); 

this_thread::sleep_for(chrono::seconds(1)) ;

auto lazy =   (chrono::duration<double>(async_lazy.get()   - t_beg)).count() ;
auto hungry = (chrono::duration<double>(async_hungry.get() - t_beg)).count() ;

std::cout << "lazy   cycles=" << lazy   << endl ;
std::cout << "hungry cycles=" << hungry << endl ;

}


