// g++ l_atomics_tst.cpp -lpthread

#include "l_atomics.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

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

// acq-rel semantics
atomic<int> semaf {0} ;
int sh_var {0} ;

void rShare() {
  while( ! semaf.fetch_sub(1, std::memory_order_acquire) > 0)
       this_thread::sleep_for(10000ms) ;

  cerr << "shared = " << sh_var << endl ;
  cerr <<  endl ;
}

void wShare() {
  sh_var = 2019 ;
  semaf.store(2, std::memory_order_release) ;
}

void tst_acqrel() {
 thread tt0(wShare) ;

 thread tt1(rShare) ;
 thread tt2(rShare) ;
 thread tt3(rShare) ;
 thread tt4(rShare) ;
 thread tt5(rShare) ;

 tt0.join() ;

 tt1.join() ;
 tt2.join() ;
 tt3.join() ;
 tt4.join() ;
 tt5.join() ;

 cerr << "semaf = " << semaf << endl ;
}

// relaxed semantics
atomic<int> refcnt {0};
std::vector<thread> tpool;

void inccnt() {
  refcnt.fetch_add(1, std::memory_order_relaxed) ;
}

void tst_relaxed() {
  for (int i=0; i<10; ++i)
      tpool.emplace_back(inccnt) ; 

  for (auto& t : tpool)
      t.join() ;

  cerr << "refcnt= " << refcnt << endl ;
}

///////////////////////////////////////
// driver
// ////////////////////////////////////
int main(int ac, char** av)
{

   //tst_lkspin() ;
   //tst_acqrel() ;
   tst_relaxed() ;

}


