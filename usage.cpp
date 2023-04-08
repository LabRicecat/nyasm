#include "nyasm.hpp"
#include <stdlib.h>

#include <fstream>
std::string rd(std::string f) {
    std::ifstream fi(f);
    std::string r;
    while(fi.good()) r += fi.get();
    if(r != "") r.pop_back();
    return r;
}

int main() {
    using namespace stack_nyachine;
    StackNyachine m = compile(rd("test.nyasm"));

    for(size_t i = 0; i < 50; ++i) {
        std::cout << (int)m.memowory[i] << " ";
        if(i%10 == 0 && i != 0) std::cout << "\n";
    }
    std::cout << "\n";
    
    stack_nyachine::NyaSignal sig = ruwun(&m,0);
    if(sig != 0) std::cout << "Signal: " << sig << "\n"; 
    std::cout << "30: " << (int)m.heawp[30] << "\n";
}