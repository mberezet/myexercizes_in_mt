#include <atomic>

///////////////////////////////////////////////////////////////////////
// naive implementation of lk-free stack.
// not wait-free !
// we cannot use std containers.
//

//  fuck PC! in softwre dev at least
#define cass comapre_and_exchange_strong
#define casw comapre_and_exchange_weak
//  eod fuck PC!

template<typename T> class lf_stack {
  struct Node {
    Node* nx   ;
    T     data ;

    Node(const T& dat): data(dat)
    {}
  } ;

  std::atomic<Node*> hd_ ;      //stack hd, ptr is trivial type

public:
  void push(const T& data) {
    Node* nnode = new Node(data) ;

    nnode->nx = hd_.load() ;    //initialization, cas can change that
    while( !hd_.cass(nnode->nx/*expected*/ ,nnode/*desired*/) );
  } 

  lf_stack() = default ;

  lf_stack(const lf_stack&)            = delete ;
  lf_stack& operator=(const lf_stack&) = delete ;
};
