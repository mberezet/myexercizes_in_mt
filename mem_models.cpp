#include <unistd.h>

#include <atomic>
#include <vector>
#include <thread>
#include <iostream>

#define M_REL std::memory_order_release 
#define M_ACQ std::memory_order_acquire 

// nb!  for that example where producer and 
//      consumer threads are 'disconnected' 
//      (do not have common shared atomic) we
//      use atomic<bool> instead of the atomic_flag.
//
std::atomic<bool> a_produced(false) ;
std::atomic<bool> a_consume(false) ;
std::vector<int>  work ;

////////////////////////////////////////////
void producer()
{
   work = { 1,2,0,4,5 } ;
   a_produced.store(true, M_REL) ;
}

void consumer()
{
   while ( a_consume.load(M_ACQ) != true ) ; 	
   work[2] = 3;     //'consume' work now
}

void dispatch()
{
   while ( a_produced.load(M_ACQ) != true );
   a_consume.store(true, M_REL) ;
}

////////////////////////////////////////////
int main() {
  std::thread td(dispatch) ;	
  std::thread tc(consumer) ;	
  std::thread tp(producer) ;	

  tc.join() ;
  tp.join() ;
  td.join() ;

  std::cout << std::endl ;
  for (const auto& e : work)
  {
   std::cout << e << "  " ;
  }
  std::cout << std::endl ;

  return 0 ;
}

/***************************************
g++ --std=c++17 -pthread -o mem_models ./mem_models.cpp
./mem_models 

1  2  3  4  5  

***************************************/
