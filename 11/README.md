# Assignment 11 - Memory Management

## Operating Systems Lab - CS3P003

## Instructions
```
g++ mem.cpp -o mem
./mem
```

## Memory Management
The given program works like an interactive shell, and you can type in one of the given commands use it.

## Commands

### initialize *`number`*
Initializes an physical address space with the given size and sets the current logical address to 0

### alloc *`number`*
Allocates a given space, if availiable (discontinuos possible) and returns the updated current logical address

### free *`Address`*
Marks all the physical address space associated with the given logical address for deletion or overwriting.

### print
Prints the current active logical addresses, the chunk size and the corresponding physical regions that it maps to.

