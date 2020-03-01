#include <iostream>
#include <stdlib.h>
#include "generator.h"

int main(int argc, char ** argv)
{
    range_generator<int> g(0, 5, 20);

#if 1
    auto n = nest(range(0, 5, 20),
                  range(1.4, 0.93, 5.7));

    auto print_pair = [](int v, float n) {
        std::cout << v << ", " << n << std::endl;
    };

    n.for_each<int, float>(print_pair);
#elif 1
    auto print_value = [](int v) {
        std::cout << v << std::endl;
    };

    auto walk_subrange = [=](int v, int n) {
        std::cout << v << " - " << n << std::endl;
        range_generator<int> g(v, n);
        g.for_each(print_value);
    };

    //g.for_each(print_value);
    g.for_each(walk_subrange);
#else
    while(!g.done()) {
        std::cout << g.next() << std::endl;
    }
#endif

    return EXIT_SUCCESS;
}

