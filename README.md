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

## repeat(T v)
Creates an iterable representing the infinite sequence:
```v, v, v, ...```

## count(T start, T delta) 
Creates an iterable representing the infinite sequence:
```start, start + delta, start + delta + delta, ...```

## skip(unsigned k, IIterable<T> *i)
Skips the first k elements of i. This would correspond to the sequence:
```i[k], i[k+1], i[k+2], ...```
If k ≥ len(i), then the resulting iterable is empty. The result has infinite length if len(i) = ∞.

## take(unsigned k, IIterable<T> *i)
Creates an iterable representing the first k elements of i. That is:
```i[0], ..., i[k-1]```
The resulting iterable is always finite. If len(i) ≤ k elements, then the result contains all elements of i.

## alternate(IIterable<T> *i, IIterable<T> *j)
Alternates between two iterables i and j. That is:
```i[0], j[0], i[1], j[1], ...```
If either i or j is exhausted before the other one, the iterable continues only with the elements of the non-exhausted iterable.

## cycle(IIterable<T> *i)
Creates an iterable that cycles through i infinitely many times. That is, it represents the sequence:
```i[0], ..., i[len(i)-1], i[0], ..., i[len(i)-1], i[0], ...```
If i is empty then so is the resulting iterable. If len(i) = ∞, then the result is equivalent to i.

## multab(IIterable<T> *i, Iterable<T> *j)
Constructs a multiplication table of the elements in i against the elements in j. Each element of the result is an iterable that represents one row of the table. That is:
```(i[0] × j[0], ..., i[0] × j[len(j)-1]), (i[1] × j[0], ..., i[1] × j[len(j)-1]), ...```
The inner iterables should be produced from scratch every time. In particular, each time the result is iterated with a next() call, it should return a distinct and independent iterable representing the next row of the table (or throw StopIteration). Use the * operator to do the multiplication.

## flatten(IIterable<IIterable<T> *> *i)
Concatenates the iterables in i into a single iterable. That is:
```i[0][0], ..., i[0][len(i[0])-1], i[1][0], ..., i[1][len(i[1])-1], ...```
The caller guarantees that each next() call on i returns a distinct and independent iterable.
