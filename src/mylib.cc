#include "mylib.h"
#include <stdexcept>
#include <cstring>

// Najveći zajednički djelitelj, Euklidova metoda.
long gcd(long a, long b) 
{
    if (a<0) a = -a;
    if (b<0) b = -b;
    
    if ( (a==0) && (b==0) ) {
       throw std::runtime_error("gcd called with both arguments equal to zero.");
    }

    if (b==0) return a;
    if (a==0) return b;
    long c = a%b;

    return gcd(b,c);
}

// Uzmi C-string i kopiraj ga u novi string s uklonjenim prednjim i stražnjim 
// bjelinama.
char * strip(const char * str){
    while(std::isspace(static_cast<unsigned char>(*str)))
        ++str;

    const char * p=str;
    while(!std::isspace(static_cast<unsigned char>(*p)) && *p != '\0')
        ++p;
     
    auto n = p-str;
    char * rez = nullptr;
    if(n != 0)
    {
        char * rez = new char[n+1];
        std::strncpy(rez,str,n);
        rez[n] = '\0';
        return rez; 
    }
    return rez;
}



