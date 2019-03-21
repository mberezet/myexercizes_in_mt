#include <thread>
#include <mutex>
#include <iostream>

// I don't beleive it but GCC stl implementation of the 
// std::call_once is @#^$%  buggy !!!
// It uses incorrect object internally [unique_lock instead of direct mutex] and
// throws !!! 
// And that is for long time now. muck STL !!!!
// Use original pthread library -- tested, working, no c++ overcomplicated
//     and implicit BS! [ and yes --- pthreads does have call once facility].
//
template <typename T>
class ThSngl
{
  
protected:
  ThSngl() = default ;
 ~ThSngl() = default ;

private:
  static std::once_flag Oncegrp  ; 
  static T*   Instance ;

public:
  ThSngl(const ThSngl&) = delete ;
  ThSngl& operator=(const ThSngl&) = delete ;

  static T* Inst()
  {
    std::call_once(Oncegrp, [&](){ Instance = new T;}) ;
    return Instance ;
  }
};

template <typename T>
T* ThSngl<T>::Instance = nullptr ;

template <typename T>
std::once_flag ThSngl<T>::Oncegrp ;

/////////////////////////////////////////////////
//  test of the singleton
struct SysEnv {
  int max_procs;
  int max_sockets;
  int n_irqs ;
} ;

int main()
{
   auto SYSENV = ThSngl<SysEnv>::Inst() ;

   if (SYSENV) {
     SYSENV->max_procs   = 1024 ;
     SYSENV->max_sockets = 2048 ;
     SYSENV->n_irqs = 0 ;
   }
   else {
       std::cerr << "No System !" << std::endl ; 
   }
}
