#include <iostream>
#include <stdlib.h>
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
        range_generator(T min, T step, T max)
            : next_value(min), min_value(min),
              step_size(step), max_value(max) {}
        range_generator(T min, T max) : range_generator(min, (T) 1, max) {}
        virtual ~range_generator() = default;

        virtual bool done()  { return next_value >= max_value; }
        virtual T current()  { return next_value; }
        virtual T next()     { T cur = next_value; next_value += step_size; return cur; }
        virtual void reset() { next_value = min_value; }

        virtual void for_each(std::function<void(T)> f) {
            while(!done()) {
                T value = next();
                f(value);
            }
        }
        virtual void for_each(std::function<void(T, T)> f) {
            while(!done()) {
                T value = next();
                f(value, value + step_size);
            }
        }

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
        nested_generator(G1 g1, G2 g2) : gen1(g1), gen2(g2) {}

        bool done() { return gen1.done(); }

        template<typename T1, typename T2>
        std::pair<T1, T2> next() {
            auto r = std::make_pair(gen1.current(), gen2.current());
            gen2.next();
            if(gen2.done()) {
                gen1.next();
                gen2.reset();
            }
            return r;
        }

        template<typename T1, typename T2>
        void for_each(std::function<void(T1, T2)> f) {
            while(!done()) {
                auto value = next<T1, T2>();
                f(value.first, value.second);
            }
        }

    protected:
        G1 gen1;
        G2 gen2;
};

template<typename G1, typename... Gs>
class nested_generator_n
{
    public:
        nested_generator_n(G1 g1, Gs... gs)
            : gen1(g1), gens(gs...) {}

    protected:
        G1 gen1;
        nested_generator<Gs...> gens;
};

template<typename G1, typename... Gs>
nested_generator_n<G1, Gs...> nest(G1 g1, Gs... gs) {
    return nested_generator_n<G1, Gs...>(g1, gs...);
}

template<typename G1, typename G2>
nested_generator<G1, G2> nest(G1 g1, G2 g2) {
    return nested_generator<G1, G2>(g1, g2);
}

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

