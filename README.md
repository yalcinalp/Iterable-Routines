# Iterable-Routines  
Series of routines that will perform actions on iterables  

## Overview

This project implements a series of routines that perform actions on iterables. An iterable represents a sequence of values that are read-only but can be used to create new iterables.

### Key Features

- All iterables are read-only
- Supports both finite and infinite sequences
- Implements lazy evaluation
- Memory efficient with constant memory usage
- Custom ownership semantics for proper memory management

## Functions Implemented

The following template functions are implemented:

1. `repeat(T v)` - Creates an infinite sequence of a value
2. `count(T start, T delta)` - Creates an arithmetic sequence
3. `skip(unsigned k, IIterable<T> *i)` - Skips first k elements
4. `take(unsigned k, IIterable<T> *i)` - Takes first k elements
5. `alternate(IIterable<T> *i, IIterable<T> *j)` - Alternates between two iterables
6. `cycle(IIterable<T> *i)` - Cycles through an iterable infinitely
7. `multab(IIterable<T> *i, Iterable<T> *j)` - Creates multiplication table
8. `flatten(IIterable<IIterable<T> *> *i)` - Concatenates iterables

## Technical Details

### Template Parameter Requirements

The template parameter T supports:
- Default and copy construction
- Assignment operator =
- Consistent +, += operators
- * operator

### Ownership Semantics

- Created iterables are owned by the caller
- Passed iterables transfer ownership to the function
- Caller guarantees distinct instances for multi-parameter functions
- Iterator ownership remains with creator
- Objects must be destroyed in child-to-parent order

### Lazy Evaluation

- Iterators retrieve minimum required data
- No unnecessary data operations
- No large containers allowed (arrays, vectors, etc.)
- Constant memory usage required


# Function Specifications
![image](https://github.com/user-attachments/assets/a3f9299e-d1e8-4b30-9acd-f5cd2a7b1635)
