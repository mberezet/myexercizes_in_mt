#include <thread>
#include <pthread.h>
#include <future>
#include <iostream>
#include <cmath>

using namespace std;

int main() {
  auto result{0.0L} ;

  cerr << "main th=" << this_thread::get_id() << endl ;
  auto fut = std::async( [&](){ cerr << "futr_th=" << this_thread::get_id() << endl;
                                return sin(1.5);
                              } );
  cerr << fut.get() << endl ;

}


