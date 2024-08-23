# C++ Snippets Repository

Welcome to my C++ Snippets repository! This collection is a personal archive of the code snippets I've created while learning C++. It serves as both a reference and a way to document my progress. Inside, you'll find a variety of data structures, algorithms, and small programs. While this repository is a work in progress and may not always adhere to best practices, it reflects my current understanding of the language. The code here is primarily a learning tool for myself, but it may also be helpful to others who are on a similar learning stage.

## Table of Contents

- [Repository Structure](#repository-structure)
- [Data Structures](#data-structures)
- [Algorithms](#algorithms) 
- [Programs](#programs)
- [How to Use](#how-to-use)

## Repository Structure

The repository is organized into the following categories:
```
cpp-snippets/
    ├── data-structures/
    │   ├── linked_list.hpp
    │   └── timer.hpp
    ├── algorithms/
    └── programs/
        └── timer.hpp
```
The repository is organized into the following categories:

- **`data-structures/`**: header files with various data structures.
- **`algorithms/`**: header files with various algorithm.
- **`programs/`**: standalone C++ programs that perform specific tasks.


## Data Structures

- **Linked List**: A basic template class for a doubly linked list (`linked_list.hpp`).
- **Timer**: A utility class for measuring elapsed time with support for multiple time units (`timer.hpp`).

## Algorithms

Comming soon...

## Programs

- **Finder**: Searches for a character or substring within a given text, with optional case-insensitive search (`finder.cpp`). 

## How to Use

**1. Data Structures and Algorithms**: 
   - These are provided as header files that you can include in your own projects. They are meant to be used as libraries and should not be compiled directly.

   Usage example:

```cpp
#include <iostream>
#include "data-structures/linked_list.hpp"
#include "data-structures/timer.hpp"

int main() {
    LinkedList<int> list;

    // Measure the time taken to insert 10 elements into the linked list
    Timer timer;
    timer.start();

    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
    }

    timer.stop();
    double elapsed = timer.elapsed(Timer::Unit::MILLI);
    std::cout << "List filled with 10 elements in " << elapsed << " ms" << std::endl;

    return 0;
}
```

**2. Programs**: 
To compile and run the finder.cpp program, use the following commands:

```bash
g++ -o finder programs/finder.cpp
./finder
```

This program will prompt you to enter the text to search in, the substring to search for, and whether to consider case sensitivity. The program will then output the indices of the matches found.

```bash
Enter the text: Hello World!
Enter a character or substring to find: o
Consider text case (y/n): n
    Match at index: 4
    Match at index: 7
Total matches: 2
```
