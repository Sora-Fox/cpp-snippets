# C++ Snippets Repository

Welcome to my C++ Snippets repository! This collection is a personal archive of the code snippets I've created while learning C++. It serves as both a reference and a way to document my progress. Inside, you'll find a variety of data structures, algorithms, and small programs. While this repository is a work in progress and may not always adhere to best practices, it reflects my current understanding of the language. The code here is primarily a learning tool for myself, but it may also be helpful to others who are on a similar learning stage.

## Table of Contents

- Directory Structure
- Code Examples
  - Data Structures
  - Algorithms 
- How to Use

## Directory Structure

The repository is organized into the following categories:
```
cpp-snippets/
    ├── data-structures/
    │   └── linked_list.hpp
    └── algorithms/
```

- **`data-structures/`**: header files implementing common data structures.
- **`algorithms/`**: header files with various algorithms, categorized by their type (e.g., sorting, searching).

## Code Examples

### Data Structures

This section includes C++ implementations of common data structures. These are provided as template classes in header files:

- **Linked List**: A basic template class for a doubly linked list (`linked_list.hpp`).


### Algorithms

Here, you'll find header files with implementations of various algorithms, grouped by their categories:


## How to Use

To use any of the code snippets:

**Data Structures and Algorithms**: 
   - These are provided as header files that you can include in your own projects. They are meant to be used as libraries and should not be compiled directly.

   Example usage in a separate file:
   ```cpp
   #include "data-structures/linked_list.hpp"

   int main() {
       LinkedList<int> list;
       list.push_back(1);
       list.push_back(2);
       // Continue with your program logic
       return 0;
   }
   ```

