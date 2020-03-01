// Single file header-only library for creating ranged generators
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

template<typename T>
range_generator<T>::range_generator(T min, T step, T max)
    : next_value(min), min_value(min),
    step_size(step), max_value(max)
{
}

template<typename T>
range_generator<T>::range_generator(T min, T max)
    : range_generator(min, (T) 1, max)
{
}

template<typename T>
bool range_generator<T>::done()  { return next_value >= max_value; }

template<typename T>
T range_generator<T>::current()  { return next_value; }

template<typename T>
T range_generator<T>::next()
{
    T cur = next_value;
    next_value += step_size;
    return cur;
}

template<typename T>
void range_generator<T>::reset() { next_value = min_value; }

template<typename T>
void range_generator<T>::for_each(std::function<void(T)> f)
{
    while(!done()) {
        T value = next();
        f(value);
    }
}
 
template<typename T>
void range_generator<T>::for_each(std::function<void(T, T)> f)
{
    while(!done()) {
        T value = next();
        f(value, value + step_size);
    }
}

template<typename G1, typename G2>
nested_generator<G1, G2>::nested_generator(G1 g1, G2 g2)
    : gen1(g1), gen2(g2)
{
}

template<typename G1, typename G2>
bool nested_generator<G1, G2>::done() { return gen1.done(); }

template<typename G1, typename G2>
template<typename T1, typename T2>
std::pair<T1, T2> nested_generator<G1, G2>::next()
{
    auto r = std::make_pair(gen1.current(), gen2.current());
    gen2.next();
    if(gen2.done()) {
        gen1.next();
        gen2.reset();
    }
    return r;
}

template<typename G1, typename G2>
template<typename T1, typename T2>
void nested_generator<G1, G2>::for_each(std::function<void(T1, T2)> f)
{
    while(!done()) {
        auto value = next<T1, T2>();
        f(value.first, value.second);
    }
}


