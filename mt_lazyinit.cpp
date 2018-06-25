#include <thread>
#include <vector>
#include <iostream>

#include <mutex>

using namespace std ;

std::once_flag         latch_fl ; 
static unsigned long   val{0} ;

////////////////////////////////////////////////////
void latched_init() {
   val = 999 ;
   cerr << "init called th=" << this_thread::get_id() 
        << " val=" << val << endl ;
}
  
void th_proc()
{
   call_once(latch_fl, latched_init);
   if (val == 999)
      cout << "initialized; th=" << this_thread::get_id() << endl ;
   else
      cout << "not-initialized ; th=" << this_thread::get_id() << endl ;
}
////////////////////////////////////////////////////
int main(int ac, char** av)
{
    thread t1(th_proc) ;
    thread t2(th_proc) ;
    thread t3(th_proc) ;
    thread t4(th_proc) ;
    thread t5(th_proc) ;

    t1.join() ;
    t2.join() ;
    t3.join() ;
    t4.join() ;
    t5.join() ;
}
