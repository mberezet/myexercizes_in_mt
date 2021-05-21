#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>

//////////////////////////////////////////////////////////
//  implementation of the hierarchical mutex.
//  It guarantees that with right settings of the
//     locks-level the hierarchy of locks used protects from
//     dead-locks.
//  Operations:  you instantiate mux_hier w fixed hierarchy number (on construction).
//               Hier - from low-level(low number) -> high-level(higher number).
//               If thread already has mx(N) locked it cannot lock mx(M) where M >= N
//               (in this impl. i will throw an exception if it tries that). But the other way
//               is allowed (hi-level uses service of the low-level).  
//               Thus order of the locking is enforced.
//
//               Each mx will update thread's currently held hier after lock (if validated)
//               and resets that tread's  prev held hier after unlock. 
//               Low-cost, since mx can be held by only one th at a time.
//
//  Cost: local_trhread mem access(hashing+indirection)  + usual cost of std::mutex usage + exception
//              ( negligible if not invoked, significant if invoked)             
//
//  NOTE: if u need to lock several locks SIMULT and want a protection from
//        possible dedalok, use std::lock or std::unique_lock ! this
//        can be needed if u do SINGLE operaion on mt-protected structures 
//        ( think swap<T>(T& v1, T& v2), where v's are protected by their locks).
//
//  Our mux_hier needed when locking is TEMPORALY separated.                   

class mux_hier
{
public:
  constexpr static unsigned long MAXHIER = 2048 ;        // max. number of possible hier levels
				
  //     public std compatible interface (but we dont derive -- std classes are finals)
  explicit mux_hier(unsigned long hier) : my_hier_{hier}, th_hilevel_prev_{0}
  { }

  //  to follow std::mutex invariants
  mux_hier()                           = delete ;
  mux_hier(const mux_hier&)            = delete ;
  mux_hier& operator=(const mux_hier&) = delete ;

  bool try_lock()
  {
     validate_hier() ;
     if (! mx_.try_lock())
        return false ;

     set_th_hier() ;
     return true ;
  }

  void lock()
  {
    validate_hier() ;
    mx_.lock() ;
    set_th_hier() ;
  }

  void unlock()
  {
    mx_.unlock() ;			
    th_hilevel_curr = th_hilevel_prev_ ;
  }

  unsigned long getlevel() const
    {return my_hier_ ;}

private:
  void validate_hier()
  {
    if (my_hier_ >= th_hilevel_curr)
        throw std::logic_error("Violated mutex hierarchy !") ;			
  }

  void set_th_hier() 
  {
    th_hilevel_prev_ = th_hilevel_curr ;
    th_hilevel_curr = my_hier_ ;
  }
  	
private:
  static thread_local unsigned long th_hilevel_curr;   //hier of the thread acting on this mx			
  unsigned long                     th_hilevel_prev_ ; //hier of the mx this th holds already before me

  const  unsigned long my_hier_ ;                      //hier of this mx (fixed for its lifetime)
  std::mutex           mx_  ;

};
thread_local unsigned long mux_hier::th_hilevel_curr = mux_hier::MAXHIER ;

