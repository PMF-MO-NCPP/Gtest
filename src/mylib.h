#pragma once
#include <limits>
#include <stdexcept>

long gcd(long,long);

char * strip(const char * str);

template <typename T>
T Sqrt(T x){
    T EPS = 4*std::numeric_limits<T>::epsilon();
    int NITER = 20;
    if(x < 0.0) 
        throw std::runtime_error("Sqrt: Negativan argument.");
    T prev{x/2}, next;
    int i=0;
    for(; i<NITER; ++i){
        next = (prev+x/prev)/2.0;
        if(std::abs(prev-next) < EPS*x || std::abs(next*next - x) < EPS)
            break;
        prev = next;
    }
    if(i == NITER)
        throw std::logic_error("Sqrt: nema konvergencije.");

    return next;
};
