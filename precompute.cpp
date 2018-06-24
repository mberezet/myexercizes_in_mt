#include <future>
#include <chrono>
#include <iostream>

namespace std::chrono = tmr ;
using namespace std ;

int precisecompute() ;          //precise, very slow
int fastcompute() ;             //fast, not precise

int compute(int ac , char** av)
{

    auto tmbarrier =  tmr::steady_clock::now() + tmr::secs(30); //this is max abs tmbarrier we agree to wait for calc

    //  in fact its std::future<int>  type. std::launch::async is a policy flag that will force compiler not to generate
    //     lazy[delayed] thread start				
    auto prres = async(launch::async, precisecompute()) ;         //start in parallel thread computaiton    

    int  fares = fastcompute() ;

    future_status calcstatus = prres.wait_until(tmbarrier);     //we will wait for the precise calc not past <tmbarrier>
    if ( calcstatus == future_status::ready)                    // time stamp
        return prres.get() ;
    else
        return fares;
}
