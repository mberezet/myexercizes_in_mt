///////////////////////////////////////////
//  as such , std::packaged_task 
//     looks very ugly
//  and all that tasks,asyncs are good for
//     one-time processing -- otherws cost
//     of constant initiatig/destroing  threads is just too
//     big -- we need th-pool in that cases..   

#include <thread>
#include <future>
#include <iostream>
#include <utility>
#include <deque>

using namespace std ;

struct Summer {
	
   long operator()(int beg, int end)
   {
      long sum(0) ;
      for ( int i = beg; i <= end; ++i)	  
	    sum += i ;

      return sum ;
   }
};

/////////////////////////////////////////////
int main() 
{

  int incr = 100 ;

  int beg = 0;
  int end = incr ;

  Summer s1; 
  Summer s2; 
  Summer s3; 
  Summer s4; 

  std::packaged_task<long(int, int)> tsk1{ s1 }; 
  std::packaged_task<long(int, int)> tsk2{ s2 }; 
  std::packaged_task<long(int, int)> tsk3{ s3 }; 
  std::packaged_task<long(int, int)> tsk4{ s4 }; 

  std::future<long> f1 = tsk1.get_future() ;
  std::future<long> f2 = tsk2.get_future() ;
  std::future<long> f3 = tsk3.get_future() ;
  std::future<long> f4 = tsk4.get_future() ;

  // note:  As an object it can be placed in container (moved only ???) 
  //        As an illustration here...
  std::deque<std::packaged_task< long(int,int) > > Q ;
  Q.push_back(move(tsk1)) ;
  Q.push_back(move(tsk2)) ;
  Q.push_back(move(tsk3)) ;
  Q.push_back(move(tsk4)) ;

  while ( ! Q.empty())
  {
     std::packaged_task<long(int, int)> pt =  move(Q.front());
     Q.pop_front() ;

     thread t_subsum( move(pt), beg, end ) ;

     beg = end + 1 ;
     end += incr ;

     t_subsum.detach() ;
  }  

  long Summa = f1.get() + f2.get() + f3.get() + f4.get() ;
  std::cout << "Summa 0...400 = " << Summa << std::endl ;

  return 0 ;
}
