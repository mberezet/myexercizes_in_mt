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
template<typename AT> struct islkfree {
    bool operator()() const { return std::atomic<AT>().is_lock_free(); }   
} ;


class lkspin {
  std::atomic_flag lk_ = ATOMIC_FLAG_INIT ;

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
