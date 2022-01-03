#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

static volatile int shared = 0 ;

std::mutex        MX ;
std::atomic<bool> Initialized ;


using namespace std ;
/////////////////////////////////
inline void ini_shared()
{
  if (! Initialized)
  {
    lock_guard L(MX) ;
    if (! Initialized) 
       shared += 42; // it is initial value        	    

    Initialized = true;
  }
}

void th_main()
{
  ini_shared() ;	

  if (shared != 42) {
    cout << this_thread::get_id() <<  ":ne mogy dalshe..vyxogy" << endl ;
    return ;
  }

  this_thread::sleep_for(chrono::milliseconds(100));
}

/////////////////////////////////////
int main() {
	thread t1(th_main) ;	
	thread t2(th_main) ;	
	thread t3(th_main) ;	
	thread t4(th_main) ;	
	thread t5(th_main) ;	
	thread t6(th_main) ;	
	thread t7(th_main) ;	

        cerr << t1.get_id() << ":trying to eow... " << endl ;
	t1.join() ;
        cerr << t2.get_id() << ":trying to eow... " << endl ;
	t2.join() ;
        cerr << t3.get_id() << ":trying to eow... " << endl ;
	t3.join() ;
        cerr << t4.get_id() << ":trying to eow... " << endl ;
	t4.join() ;
        cerr << t5.get_id() << ":trying to eow... " << endl ;
	t5.join() ;
        cerr << t6.get_id() << ":trying to eow... " << endl ;
	t6.join() ;
        cerr << t7.get_id() << ":trying to eow... " << endl ;
	t7.join() ;
}
