#include "mux_hier.h"

#include <thread>
#include <iostream>

void process( mux_hier& himux  )
{

  std::cerr << "Requesting level-" << himux.getlevel() << " service" << std::endl ; 
  try {
    himux.lock();
    std::cerr << "In level-" << himux.getlevel() << " service" << std::endl ; 
  }
  catch (const std::exception& e) {
    std::cerr << "Refused level-" << himux.getlevel() << " service" << std::endl ;					
    std::cerr << "error: " << e.what()  << std::endl ;					
  }
}

/////////////////////////////////////////////////////////
int main ()
{	
  mux_hier himux_30{30} ;
  mux_hier himux_20{20} ;
  mux_hier himux_10{10} ;

  // this suceeds...
  std::thread t1(
     [&]() { process(himux_30); process(himux_20); }
  ) ;			

  // this should fail...
  std::thread t2(
     [&]() { process(himux_10); process(himux_20) ;}
  ) ;			

  t2.join() ;
  t1.join() ;	
}

/**************************************************
  
g++ --std=c++17 -pthread ./mux_hier_tst.cpp

./a.out
Requesting level-Requesting level-30 service10 service

In level-30 service
Requesting level-20 service
In level-20 service
In level-10 service
Requesting level-20 service
Refused level-20 service
error: Violated mutex hierarchy !

***********************************************/

