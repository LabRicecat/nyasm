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
    if(argc != 3) {
        std::cout << "No action and file given!\n";
        return 1;
    }
    using namespace stack_nyachine;
    std::string debuginfo;

    std::string action = std::string(argv[1]);
    if(action == "build") {
        StackNyachine m(0,0);
        try {
            m = compile(rd(std::string(argv[2])),true);
        }
        catch(FailedExecutionException& err) {
            std::cout << "\nFailed compilation!";
            return 1;
        }
        tofile("out.snya",&m);
        std::cout << "Compilation successfully performed into out.snya\n";
    }
    else if(action == "debug") {
        StackNyachine m(0,0);
        try {
            m = compile(rd(std::string(argv[2])),true,&debuginfo);
        }
        catch(FailedExecutionException& err) {
            std::cout << "\nFailed compilation!";
            return 1;
        }
        std::cout << debuginfo << "\n";
    }
    else if(action == "run") {
        std::string file = std::string(argv[2]);
        StackNyachine m(1,100000);
        fromfile(file.c_str(),&m);
        
        stack_nyachine::NyaSignal sig = ruwun(&m,0);
        if(!stack_nyachine::nasig_ok(sig)) std::cout << "Signal: " << sig << "\n"; 
        else std::cout << "Execution exited normally\n";
        return 0;
    }
    else if(action == "dump") {
        std::string file = std::string(argv[2]);
        StackNyachine m(1,1);
        fromfile(file.c_str(),&m);
        for(size_t i = 0; i < m.memowory_size; ++i) {
            printf(" %06ld", m.memowory[i]); // I hate you C++ for not having a proper `std::format`
            if((i+1) % 10 == 0) std::cout << "\n";
        }
        return 0;
    }
    else {
        std::cout << "Unknown option.\nAccepted: build, debug, dump, run\n";
        return 1;
    }
}