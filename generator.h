// Header-only library for creating ranged generators
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
#ifndef __GENERATOR_LIBRARY__
#define __GENERATOR_LIBRARY__
#include <functional>

template<typename T>
class generator
{
    public:
        generator() = default;
        virtual ~generator() = default;

        virtual bool done() = 0;
        virtual T next() = 0;
        virtual void reset() = 0;

        virtual void for_each(std::function<void(T)>) = 0;
        virtual void for_each(std::function<void(T, T)> f) = 0;
};

template<typename T>
class range_generator : public generator<T>
{
    public:
        range_generator(T min, T step, T max);
        range_generator(T min, T max);
        virtual ~range_generator() = default;

        virtual bool done();
        virtual T current();
        virtual T next();
        virtual void reset();

        virtual void for_each(std::function<void(T)> f);
        virtual void for_each(std::function<void(T, T)> f);

    protected:
        T next_value;
        const T min_value;
        const T max_value;
        const T step_size;
};

template<typename T>
range_generator<T> range(T min, T step, T max) {
    return range_generator<T>(min, step, max);
}

template<typename T>
range_generator<T> range(T min, T max) {
    return range_generator<T>(min, max);
}

template<typename G1, typename G2>
class nested_generator
{
    public:
        nested_generator(G1 g1, G2 g2);

        bool done();

        template<typename T1, typename T2>
        std::pair<T1, T2> next();

        template<typename T1, typename T2>
        void for_each(std::function<void(T1, T2)> f);

    protected:
        G1 gen1;
        G2 gen2;
};

template<typename G1, typename G2>
nested_generator<G1, G2> nest(G1 g1, G2 g2) {
    return nested_generator<G1, G2>(g1, g2);
}

// Template implementations
#include "range_generator.hpp"
#include "nested_generator.hpp"

#endif // __GENERATOR_LIBRARY__
