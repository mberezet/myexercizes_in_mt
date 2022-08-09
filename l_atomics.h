#pragma once
#include <atomic>

///////////////////////////////////////////////////////////////////////
//  NB!  each atomic op supports (optional) memory-ord arg:
// std::memory_order_seq_cst  - full-seq ord (default, most restrictive)
// std::memory_order_relaxed
// std::memory_order_consume
// std::memory_order_acquire
// std::mempry_order_release
// std::memory_order_acq_rel
//
// compare_exchange_strong
//      and
// compare_exchange_weak
//      have 2 mem-ord params -
//      one for success, one for failure. if use
//      only one, it is for both succ and fail.
// ///////////////////////////////////////////////////////////////////
inline void pause() {
    asm volatile("pause");
}

template<typename AT> struct islkfree {
    bool operator()() const { return std::atomic<AT>().is_lock_free(); }   
} ;

////// Lock spin based on atomic_flag ///////////////////////////
class lkspin
{
protected:    
  alignas(64) std::atomic_flag  lk_ = ATOMIC_FLAG_INIT ;

public:
  void acq() {
    while(lk_.test_and_set()) ;
  }

 void unacq() {
    lk_.clear() ;
 }

 // for x86-64 its implemented via
 //     xchg ... ; test
 //     xchg[x] op in intel ALWAYS locked
};

////// Lock spin based on atomic<int> //////////////////
class lkspinwt 
{
protected:    
    alignas(64) std::atomic<int> lk_ ; // 0- free; 1-locked
    
public:
    lkspinwt() : lk_(0)
    { }                               //atomic ini is atomic   

    void lock()
    {
        while ( ! tryloclk() )        //eagrly try to lock
        {                             //   failed..pause-loop till get back 0 (meaning im-currently-free)
           while ( lk_.load(std::memory_order_acquire) )
           { pause(); } 
        }
    }
    
    bool trylock()
    {   // atomic set: if locked already - just 're-enforce' it(atomically)
        //             if free           - lock it and get(atomically) prev.state
        return ( 0 == lk_.exchange(1, std::memory_order_seq_cst) ;
    }
                
    void unlock()
    {
        lk_.store(0, std::memory_order_release) ;
    }
} ;
