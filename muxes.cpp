#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

using namespace std ;

struct elem_t {
  int x{9} ;
  mutex m ;
} ;

///////////////////////////////////////////////////
void t_main(elem_t& e1, elem_t& e2) {
  unique_lock<mutex> lo_1(e1.m, defer_lock) ;
  this_thread::sleep_for(chrono::milliseconds(1)) ; 

  unique_lock<mutex> lo_2(e2.m, defer_lock) ;
  this_thread::sleep_for(chrono::milliseconds(1)) ; 

  std::lock(lo_1, lo_2) ;    // this is ATOMIC lock of both locks !!!
  ++e1.x ;   ++e2.x ;
}

///////////////////////////////////////////////////
int main() {

  elem_t e_nxt ;
  elem_t e_prev ;

  // old way ... NB! pass std::ref 
  //             Passing as val will generate cpmpile error !
  //thread t1( t_main, std::ref(e_nxt), std::ref(e_prev) ) ;
  //thread t2( t_main, std::ref(e_prev), std::ref(e_nxt) ) ;

  // modern way ... NB! capture refrence.
  //             Any other will genrate compile error !
  thread t1( [&]{t_main(e_nxt, e_prev);} ) ;
  thread t2( [&]{t_main(e_prev, e_nxt);} ) ;

  t1.join(); t2.join() ;

  cerr << "e_nxt.x = " << e_nxt.x << ";" << "e_prev.x = " << e_prev.x << endl ;          
}
