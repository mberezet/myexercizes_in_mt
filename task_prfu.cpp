#include <thread>
#include <future>
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

/////////////////////////////////
//  promise executors
void ddiv(promise<double>&& pro, double d1, double d2) {
  this_thread::sleep_for(10s);
  pro.set_value( d1/d2);
}
 
void dmul(promise<double>&& pro, double d1, double d2) {
  this_thread::sleep_for(5s);
  pro.set_value( d1*d2);
}

///////////////////////////////
int main()
{
  // ////   tasks (async)
  auto result{0.0L} ;

  cerr << "main th=" << this_thread::get_id() << endl ;
  auto fut = std::async( [&](){ cerr << "futr_th=" << this_thread::get_id() << endl;
                                return sin(1.5);
                              } );
  cerr << fut.get() << endl ;

  // ///// promise/future
  promise<double> pcalc1 ; future<double> f1 = pcalc1.get_future();
  promise<double> pcalc2 ; future<double> f2 = pcalc2.get_future();

  thread t1(ddiv, std::move(pcalc1), 1.1, 2.2) ;
  thread t2(dmul, std::move(pcalc2), 1.1, 2.2) ;
  cerr << "waiting for promises" << endl ;
  cerr << "M=" << f2.get() << "; D=" << f1.get() << endl ;
  t1.join() ;
  t2.join() ;

}


