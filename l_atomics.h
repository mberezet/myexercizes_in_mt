#pragma once
#include <atomic>

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
