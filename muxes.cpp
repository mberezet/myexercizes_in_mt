#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std ;

////////////////////////////////////////////////
//  Ex 1:
//  To illustrate how std::unique_lock prevents
//     deadlock. Note that if i just lock regular mx or
//     lock_guard - whatever - in t_main(), main() would
//     have inevitable dead-lock, as it calls t1,t2 executor
//     params in reverse order to each other !
//     But mx's are not locked initially, and std::lock()
//     is an ATOMIC ops --- so it 'collpases' 2 locks in 'one'
//
//     BUT!  lock() or try_lock() free function's magic is achieved
//     by back-off-retry --- which can be highly non-deterministic
//     latency-wise, on top of high latency of contended mx, due to
//     possible kernel calls. Not for HFT or other sensitive stuff.
//
//     also:  std::lock is RAII !
//
struct elem_t {
  int x{9} ;
  mutex m ;
} ;

void t_main1(elem_t& e1, elem_t& e2) {
  unique_lock<mutex> lo_1(e1.m, defer_lock) ;
  this_thread::sleep_for(chrono::milliseconds(4)) ; 
  unique_lock<mutex> lo_2(e2.m, defer_lock) ;

  std::lock(lo_1, lo_2) ;    // this is ATOMIC lock of both locks !!! 
  cerr << "thread= " << this_thread::get_id()  << endl ;
  ++e1.x ;   ++e2.x ;
}

////////////////////////////////////////////////////////
//   Ex 2: scoped_lock (starting C++17)
//   this is object similar to above example, but more brief,
//   no need 'zavodit' locks and then atomically lck them -
//   its all can be done with scoped_lock object
//   Same about performance as above.
//
//   also:  std::scoped_lock is RAII !
//
void t_main2(elem_t& e1, elem_t& e2) {
  std::scoped_lock(e1.m, e2.m) ;                   // this is ATOMIC lock of both mux's !!!
  this_thread::sleep_for(chrono::milliseconds(4)) ; 
  cerr << "thread= " << this_thread::get_id()  << endl ;
  e1.x=999 ;   e2.x = 1000 ;
}

////////////////////////////////////////////////////////
//   Ex 3: prallel Fib
int cnt=1 ; int b=1 ; int c=1;
mutex mcnt ;
mutex minc ;
constexpr int TPOOLSZ = 10;   //fib of that...

void fibinc() {
  scoped_lock<mutex, mutex> GG(mcnt,minc) ;
  ++cnt ;
  b = std::exchange(c, b+c) ;
}

///////////////////////////////////////////////////
int main() {

  elem_t e_nxt ;
  elem_t e_prev ;

  // old way ... NB! pass std::ref. Passing as val will generate compile error !
  //thread t1( t_main1, std::ref(e_nxt), std::ref(e_prev) ) ;
  //thread t2( t_main1, std::ref(e_prev), std::ref(e_nxt) ) ;

  // modern way ... NB! capture refrence.  Any other will genrate compile error !
  thread t1( [&]() { t_main1(e_nxt,  e_prev); } ) ;
  thread t2( [&]() { t_main1(e_prev, e_nxt);  } ) ;
  t1.join(); t2.join() ;
  cerr << "e_nxt.x = " << e_nxt.x << ";" << "e_prev.x = " << e_prev.x << endl ;          

  // test scoped_lock
  // NB this must be gcc with -std=c++17 if default is c++14
  thread t3( [&]() { t_main2(e_nxt, e_prev); } ) ;
  thread t4( [&]() { t_main2(e_prev, e_nxt); } ) ;
  t3.join(); t4.join() ;

  cerr << "e_nxt.x = " << e_nxt.x << ";" << "e_prev.x = " << e_prev.x << endl ;          

  //test threaded fibonacci
  std::vector<thread> tpool ;
  tpool.reserve(TPOOLSZ) ;

  for (auto x=1 ; x <= TPOOLSZ ; ++x)
      tpool.emplace_back( thread(fibinc) ) ;

  for ( auto& t : tpool)
      t.join() ;

  cerr << "Fib(" << tpool.size() << ") = " << c << endl ;
}
