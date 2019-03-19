#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

using namespace std ;

////////////////////////////////////////////////
//  Ex 1:
//  To illustrate how std::unique_lock prevents
//     deadlock. Note that if i just lock regular mx or
//     lock_guard - whatever - in t_main(), main() would
//     have inevitable dead-lock, as it calls t1,t2 executor
//     params in reverse order to each other !
//     But mx's are not locked initially, and std::lock()
//     is an ATOMIX ops --- so it 'collpases' 2 mx in 'one'
//
//     BUT!  lock() or try_lock() free functions magic is achieved
//     by back-off-retry --- which can be highly non-deterministic
//     latency-wise, on top of high latency of contended mx, due to
//     possible kernel calls. Not for HFT or other sensitive stuff.
//
struct elem_t {
  int x{9} ;
  mutex m ;
} ;

///////////////////////////////////////////////////
void t_main(elem_t& e1, elem_t& e2) {
  unique_lock<mutex> lo_1(e1.m, defer_lock) ;
  this_thread::sleep_for(chrono::milliseconds(4)) ; 
  unique_lock<mutex> lo_2(e2.m, defer_lock) ;

  std::lock(lo_1, lo_2) ;    // this is ATOMIC lock of both locks !!!
  cerr << "thread= " << this_thread::get_id()  << endl ;
  ++e1.x ;   ++e2.x ;
}

///////////////////////////////////////////////////
int main() {

  elem_t e_nxt ;
  elem_t e_prev ;

  // old way ... NB! pass std::ref 
  //             Passing as val will generate compile error !
  //thread t1( t_main, std::ref(e_nxt), std::ref(e_prev) ) ;
  //thread t2( t_main, std::ref(e_prev), std::ref(e_nxt) ) ;

  // modern way ... NB! capture refrence.
  //             Any other will genrate compile error !
  thread t1( [&]{t_main(e_nxt, e_prev);} ) ;
  thread t2( [&]{t_main(e_prev, e_nxt);} ) ;

  t1.join(); t2.join() ;

  cerr << "e_nxt.x = " << e_nxt.x << ";" << "e_prev.x = " << e_prev.x << endl ;          
}
