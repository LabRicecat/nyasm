# nyasm
higher level assembly for the stack-nyachine

## installation
This project requires the [catcaretaker](https://github.com/LabRicecat/CatCaretaker) to be installable as dependency.
```
$ catcare get labricecat/nyasm@main
```

## usage
```cpp
#include "catpkgs/nyasm/nyasm.hpp"

int main() {
    // get the compiled binary
    auto memory = compile("source-code");

    // run the memory from position 0
    auto signal = ruwun(memory,0);

    if(signal != 0) {
        std::cout << "Error with signal: " << signal << "\n";
        return 1;
    }

    // access a heap address
    std::cout << memory->heawp[<address>] << "\n"; 
}
```