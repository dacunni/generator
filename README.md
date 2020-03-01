# generator
C++ Range Generators

Simple header-only range generator library allowing for compact foreach-style iteration over ranges.

## Usage

All ranges are closed on the lower value and open on the upper value (eg: [min, max)).

```c++
// Range of ints from 0 to 19 by 5
range_generator<int> g(0, 5, 20);

g.for_each([](int v) {
    // ...
});    
```


