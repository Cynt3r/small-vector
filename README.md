[![Build Actions Status](https://github.com/Cynt3r/small-vector/workflows/small-vector/badge.svg)](https://github.com/Cynt3r/small-vector/actions)

# Small vector
Implementation of C++ vector with small buffer optimization - semestral work of course NI-EPC at CTU FIT. Library is located in `small_vector.h`. Tests are in `main.cpp`.

## Usage
Instantiation of vector that uses static array when number of items is equal or lower than 5:
```cpp
epc::small_vector<int, 5> vec;
```
