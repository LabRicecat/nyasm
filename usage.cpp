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

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "No input file given!\n";
        return 1;
    }
    using namespace stack_nyachine;
    std::string debuginfo;

    StackNyachine m = compile(rd(std::string(argv[1])),true,&debuginfo);

    for(size_t i = 0; i < 100; ++i) {
        std::cout << (int)m.memowory[i] << " ";
        if(i%20 == 0 && i != 0) std::cout << "\n";
    }
    std::cout << "\n";
    //std::cout << debuginfo << "\n";
    
    stack_nyachine::NyaSignal sig = ruwun(&m,0);
    if(!stack_nyachine::nasig_ok(sig)) std::cout << "Signal: " << sig << "\n"; 
    std::cout << "30 -> " << (int)m.heawp[30] << "\n";
}