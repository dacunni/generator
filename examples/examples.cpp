// Examples for the 'generator' single file header-only library
//
// MIT License
// 
// Copyright (c) 2020 David Cunningham
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "generator.h"

// Helper for annotating output
template <typename T>
void print_range(const char * desc, T minval, T step, T maxval)
{
    std::cout << "--- " << desc << " (min: " << minval << ", step: " << step << ", max: " << maxval << ") ---\n";
}

// Example of manually iteration through a range
void simple_range_manual()
{
    int minval = 0, step = 5, maxval = 20;

    print_range("Simple Range Manual", minval, step, maxval);

    range_generator<int> g(minval, step, maxval);

    while(!g.done()) {
        std::cout << g.next() << std::endl;
    }
}

// Example of for_each() calling a function on elements of a range
void simple_range_foreach()
{
    auto print_value = [](int v) {
        std::cout << v << std::endl;
    };

    int minval = 0, step = 5, maxval = 20;

    print_range("Simple Range For Each", minval, step, maxval);

    range_generator<int> g(minval, step, maxval);

    g.for_each(print_value);
}

// Example of for_each() calling a function for each subrange
void simple_range_foreach_subrange()
{
    auto print_subrange = [=](float low, float high) {
        std::cout << low << " : " << high << std::endl;
    };

    float minval = -2.3, step = 1.2, maxval = 5.7;

    print_range("Simple Range For Each SubRange", minval, step, maxval);

    range_generator<float> g(minval, step, maxval);

    g.for_each(print_subrange);
}

// Example of manually using a second range_generator to walk a each
// subrange of a range
void range_foreach_walk_subrange_manual()
{
    auto print_value = [](int v) {
        std::cout << v << "  ";
    };

    auto walk_subrange = [=](int v, int n) {
        std::cout << v << " - " << n << std::endl;
        range_generator<int> g(v, n);
        g.for_each(print_value);
        std::cout << '\n';
    };

    int minval = 0, step = 5, maxval = 20;

    print_range("Range For Each Walk Subrange Manual", minval, step, maxval);

    range_generator<int> g(minval, step, maxval);

    g.for_each(walk_subrange);
}

// Example of nested generators, calling for_each() with each pair of
// outer/inner values
void nested_ranges_foreach()
{
    int outermin = 0, outerstep = 5, outermax = 20;
    float innermin = 1.4, innerstep = 0.93, innermax = 5.7;

    print_range("Nested Ranges Outer", outermin, outerstep, outermax);
    print_range("Nested Ranges Inner", innermin, innerstep, innermax);

    auto print_pair = [](int v, float n) {
        std::cout << v << ", " << n << std::endl;
    };

    auto n = nest(range(outermin, outerstep, outermax),
                  range(innermin, innerstep, innermax));

    n.for_each<int, float>(print_pair);
}

// Walk through the pixels in each tile defined over a 2D domain
void tile_raster_walk()
{
    int numrows = 5, numcols = 4;
    int tilesize = 2;

    std::cout << "--- Walk Tile Pixels In Raster Order ---\n";

    auto walk_tile = [&](int startrow, int startcol) {
        printf("tile %2d %2d\n", startrow, startcol);

        // Walk pixels within tile, taking care not to overstep the bounds
        // of the range for imperfect tilings.
        auto n = nest(range(startrow , std::min(numrows, startrow + tilesize)),
                      range(startcol , std::min(numcols, startcol + tilesize)));

        n.for_each<int, int>([&](int pixrow, int pixcol) {
            printf("  px %2d %2d\n", pixrow, pixcol);
        });
    };

    auto n = nest(range(0, tilesize, numrows),
                  range(0, tilesize, numcols));

    n.for_each<int, int>(walk_tile);
}

int main(int argc, char ** argv)
{
    simple_range_manual();
    simple_range_foreach();
    simple_range_foreach_subrange();
    range_foreach_walk_subrange_manual();
    nested_ranges_foreach();
    tile_raster_walk();

    return EXIT_SUCCESS;
}

