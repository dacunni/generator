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


