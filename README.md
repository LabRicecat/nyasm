# !! MOVED TO CODEBERG.ORG !!
**This is an old version, please visit the current version at [codeberg.org](https://codeberg.org/LabRicecat/nyasm)!**

# Nyasm
A higher level assembly for the stack-nyachine

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

# Specification
Notable usage of nyasm: [`libnya`](https://github.com/labricecat/libnya)
## Chapters:
- Structure
- Memory & Stack
- Instructions
- Registers
- Inline operations
- Documentated Example

## Structure
A nyasm file typically has the `.nyasm` extension and often looks like this:
```nyasm
LABEL:
	instruction args..
	instruction args..
; this is a comment
LABEL2:
	instruction args..
	instruction args..
	..
```
Nyasm is whitespace insensitive, but only one instruction and label definition per line.
It will compile from the top to the bottom, and completely ignore comments. If they are removed before, after or during the compilation proccess is implementation defined.
Multiple definitions or instructions in the same line should yield an error.

## Memory & Stack
The StackNyachine has 2 kinds of memory, the `Heap` and the `Memory`. Both are long memory tapes, the `Heap` contains temporary mutable data that is uninitialized until written directly or indirectly onto it. The `Memory` contains the executable instructions, which are immutable and uninitialized as well. It's guaranteed that the last instruction on the memory tape is the halting instruction.

The `Heap` is split into 4 parts, the `Stack` area, `reserved` area, `data` area and `alloc` area.
The `Stack` begins at the end of the Heap is defined as `1/3` of the `Heap`. The `reserved` area is at the beginning and contains memory mapped registeres required by Nyasm. The `data` area is next to the `reserved` area and holds global mutable data cells. The last area, the `alloc` area is used by to allocate memory for your program. 

## Instructions
An instruction is defined as a keyword and an amount of arguments which can be 0 as well as unfixed. 

### void
Takes 0 arguments and evaluates to an `nop`, an optcode that does nothing.

### jump
Takes 1 argument, a label, and evaluates to the jump instruction to the given labels address.

### jumpif
Takes 3 arguments, 2 addresses and a label, and evaluates to the jump instruction, which jumps when both addresses hold the same value.

### move
Takes 2 arguments, both addresses, and copies the the value of the first into the address contained in the second. Note that this means the second argument gets dereferenced!

### data
Takes 1 argument, a name, and adds a new entry in the data field. 

### add
Takes 3 arguments, 3 addresses and adds the value of the first two into the address contained in the thried.

### sub
Takes 3 arguments, 3 addresses, and subtracts the second from the first where the result goes into the address contained in the third.

### mul
Takes 3 arguments, 3 addresses, and multiplies the first two into the address contained in the third. 

### div
Takes 3 arguments, 3 addresses, and integer divides the first two into the address contained in the thrid.

### deref
Takes 2 arguments, 2 addresses and dereferences the first into the address contained in the third.

### halt
Takes 0 arguments, and halts the execution.

### push
Takes 1 argument, an address and pushes it into the memory mapped stack. The old value is overriden. If the stack would grow too big, an error signal is emitted and execution halts.

### pop
Takes 0 arguments, and pops one element from the stack. If the stack would grow into negative size, an error signal is emitted and execution halts.

### call
Takes 1 argument, a label, pushes the current execution pointer onto the stack and jumps to the given label.

### return
Takes 0 arguments, pops one element from the stack and sets the current execution pointer to the aquired value.

## Registers
All registers are memory mapped, and as such mutable. The only exception is the stack pointer, which is not memory mapped and immutable. It's only possible to get a copy of it into a mapped register.

### ssz
Contains the current stacks size.

### svl
Contains a copy of the top element of the stack.

### spt
Contains a copy of the stack pointer.

### this
Contains the current position in memory.

### rv*
`rv1`, `rv2` and `rv3` are registers used for return values. Not a copy but the real mapped address.

### null
Link to address 0.

## Inline operations
To make the developers live easier, there are a few inline operations they can perform in with arguments.

### literal values
Putting an `%` before a number, register or label, will create an temporary address containing this literal value. Useful if you want to avoid a deref.

### inline deref with offset
Putting square braces (`[]`) around an argument, will add an additional dereference step. It's possible to perform addition and subtraction using `+` and `-` respectively to add an offset.
Example:
```
[10]        ; will deref 10
[10 + %1]   ; will deref the value at 10 plus one
[%5 + %5]   ; will deref the address 10
[spt - %1]  ; will return an address containing the element 
			; one below the top of the stack.
```
### inline offset
Using curly braces (`{}`) instead of square braces (`[]`) will remove the dereference step.
Example:
```
{10 + 11} ; returns an address containing the value of 10 plus the value of 11
{%4 + %5} ; returns an address containing 9
```

## Documented Example
This program recursively caluclates the fibunacci sequence.
```
jump MAIN ; jump to the main label

FIB: ; begin of the procedere
    jump FIB_BODY 
FIB_10:
    move [spt-%1] %rv1 ; return the given argument
    return ; jump back to the call
FIB_ELSE:
    push {[spt-%1]-%1} ; push the argument minus 1 onto the stack
    call FIB ; call again with new argument
    pop ; pop pushed argument
    push rv1 ; push the result temporarily onto the stack
    push {[spt-%2]-%2} ; push the argument minus 2 onto the stack
    call FIB ; call fob again with new argument
    pop ; pop pushed argument
    push rv1 ; push new result again onto the stack
    add [spt-%1] [spt] %rv1 ; add both pushed results
    pop ; pop both
    pop
    return ; return the result
FIB_BODY:
    jumpif [spt-%1] %1 FIB_10
    jumpif [spt-%1] %0 FIB_10 ; if the argument is 1 or 0, jump to FIB_10
    jump FIB_ELSE ; jump to the recursive calls

MAIN:
    push %10 ; push `10` onto the stack as argument
    call FIB ; 
    pop ; pop the argument
    move rv1 %30 ; move the result somewhere
    void ; undeeded, but left for asthetic 
```
