#pragma once
#include <limits>
#include <stdexcept>
#include <limits>
#include <stdexcept>
#include <array>
#include <fmt/format.h>

long gcd(long,long);

char * strip(const char * str);

// Predložak metode za računanje drugog korijena broja. Izbacuje  std::runtime_error
// u slučaju negativnog argumenta, a std::logic_error u slučaju da algoritam ne konvergira. 
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

// Izuzetak za klasu Vector.
struct BoundException : public std::runtime_error{
    BoundException(std::size_t n) : 
        std::runtime_error{fmt::format("Index izvan granica: n = {}",n)} 
    {}
};

// Klasa Vector - vektor fiksne duljine s dvije posebnosti. Dodijeljeni konstruktor 
// inicijalizira sve vrijednosti s -Inf. Operator indeksiranja izbacuje izuzetak  
// BoundException ako je indeks izvan granica.
template <std::size_t N>
class Vector : private std::array<double, N> {
    using Base = std::array<double, N>;
    public:
        Vector() :  std::array<double, N>{} {
            double neg_inf = -1 * std::numeric_limits<double>::infinity();
            for(std::size_t i=0; i<N; ++i)
                (*this)[i] = neg_inf;
        }

        double operator[](std::size_t n) const { 
            if(n >= N) 
                throw BoundException{n};
            return Base::operator[](n);
        }
         
        double &  operator[](std::size_t n){ 
            if(n >= N) 
                throw BoundException{n};
            return Base::operator[](n);
        }
};
