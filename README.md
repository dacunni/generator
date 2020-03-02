# generator
C++ Range Generators

Simple header-only range generator library allowing for compact foreach-style iteration over ranges.

## Usage

All ranges are closed on the lower value and open on the upper value (eg: [min, max)).

```
#include <generator.h>
```

### Item Iteration
```c++
// Range of ints from 0 to 19 by 5
range_generator<int> g(0, 5, 20);

g.for_each([](int v) {
    // ...
});    
```

### Subrange Iteration
```c++
range_generator<float> g(-2.3, 1.2, 5.7);

// low = current range value
// high = next range value
g.for_each([](float low, float high) {
    // ...
});
```

### Nested Ranges
```c++
auto print_pair = [](int v, float n) {
    std::cout << v << ", " << n << std::endl;
};

auto n = nest(range(outermin, outerstep, outermax),
              range(innermin, innerstep, innermax));

n.for_each<int, float>(print_pair);
```
