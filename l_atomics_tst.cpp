// g++ l_atomics_tst.cpp -lpthread

#include "l_atomics.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <string>

using namespace std ;

//////////////////////////////
//tsters
////////////////////////////////////
//  lkspin
unsigned int cnt = 0 ;
lkspin       sl;

void work() {
   sl.acq() ;
   ++cnt ;
   cout << cnt << endl ;
   sl.unacq() ;

}
void antiwork() {
   sl.acq() ;
   this_thread::sleep_for(10s) ; 
   cout << cnt << endl ;
   sl.unacq() ;
}
void tst_lkspin()
{
  thread t0(work) ; 
  thread t1(work) ; 
  thread t2(antiwork) ; 
  thread t3(work) ; 
  thread t4(antiwork) ; 

  t0.join() ;
  t1.join() ;
  t2.join() ;
  t3.join() ;
  t4.join() ;
}

///////////////////////////////////////
// driver
// ////////////////////////////////////
int main(int ac, char** av)
{
   //1. find which ones implemented lock-free for given arch
   cerr << "a_bool lkfree? " << islkfree<bool>()() << endl ;
   cerr << "a_int  lkfree? " << islkfree<int>()()  << endl ;
   cerr << "a_long lkfree? " << islkfree<long>()() << endl ;
   cerr << "a_char lkfree? " << islkfree<char>()() << endl ;
   cerr << "a_char* lkfree? " << islkfree<char*>()() << endl ;

   // this call for example is compile error --- one of the
   //      atomic's constraints is to be trivially copyable
   //      which std::string is definitely not !
   //cerr << "a_string lkfree? " << lkfree<std::string>()() << endl ;

   //2. spin-lock via atomic_flag
   tst_lkspin() ;
}


