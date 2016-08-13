//
//  main.cpp
//  MetaProgramming
//
//  Created by Joseph Canero on 8/11/16.
//  Copyright © 2016 jcanero. All rights reserved.
//

#include <iostream>
#include <functional>
#include <assert.h>
#include <type_traits>

namespace containers {

template <typename L, typename R>
struct PAIR { };

struct FST {};
struct SND {};

template <typename P, typename F>
struct GET { };

template <typename L, typename R>
struct GET<PAIR<L, R>, FST> {
    typedef L result_type;
    result_type result = L::result_type;
};

template <typename L, typename R>
struct GET<PAIR<L, R>, SND> {
    typedef R result_type;
    result_type result = R::result;
};
    
} // namespace containers

namespace enumerals {

struct ZERO {
    enum { result = 0 };
    typedef int result_type;
};

template <typename N>
struct SUCC {
    enum { result = N::result + 1 };
    typedef int result_type;
};

typedef SUCC<ZERO> ONE;
typedef SUCC<ONE>  TWO;
typedef SUCC<TWO>  THREE;

// so on..

struct SLIDE {};

template <typename F, typename Arg>
struct EVAL {};

template <typename L, typename R>
struct EVAL<SLIDE, containers::PAIR<L, R> > {
    typedef containers::PAIR<R, SUCC<R> > result_type;
};

template <typename N, typename F, typename X>
struct REPEAT {};

template <typename F, typename X>
struct REPEAT<ZERO, F, X> {
    typedef X result_type;
    result_type result = X::result;
};

template <typename N, typename F, typename X>
struct REPEAT<SUCC<N>, F, X> {
    typedef typename REPEAT<N, F, typename EVAL<F, X>::result_type >::result_type result_type;
    result_type result = result_type::result;
};

template <typename N>
struct DECREMENT {
    typedef typename containers::GET<
        typename REPEAT<N, SLIDE, containers::PAIR<ZERO, ZERO> >::result_type,
        containers::FST
    >::result_type result_type;

    result_type result = result_type::result;
};

} // namespace enumerals

namespace logic {

struct TRUE  {
    typedef bool result_type;
    const static result_type result = true;
};

struct FALSE {
    typedef bool result_type;
    const static result_type result = false;
};

template <class C, class T, class F>
struct IF {
    typedef typename std::conditional<
                        std::is_same<typename C::result_type,
                        TRUE>::value,
                        T,
                        F>::type result_type;
    typename result_type::result_type result = result_type::result;
};

template <typename L, typename R>
struct EQUALS {
    typedef typename std::conditional<
                        std::is_same<L, R>::value,
                    TRUE,
                    FALSE>::type result_type;
    typename result_type::result_type result = result_type::result;
};

} // namespace logic

int main(int argc, const char * argv[]) {
    using namespace enumerals;
    using namespace logic;
    using namespace containers;

    typedef SUCC<TWO> THREE;
    assert(THREE::result == 3);

    typedef typename IF<
                        EQUALS<ONE,
                               ONE>,
                        TWO,
                        THREE>::result_type COND_EVAL;
    assert(COND_EVAL::result == 2);
    static_assert(std::is_same<COND_EVAL, TWO>::value,
                  "if (1 == 1) { 2 } else { 3 }");

    typedef typename EQUALS<ONE, ONE>::result_type EQ;
    assert(EQ::result == true);
    static_assert(std::is_same<EQ, TRUE>::value, "1 == 1");

    typedef typename IF<
                        EQUALS<THREE,
                               SUCC<TWO>>,
                        SUCC<THREE>,
                        TWO>::result_type FOUR;
    assert(FOUR::result == 4);
    static_assert(std::is_same<FOUR, SUCC<THREE>>::value,
                  "if (3 == 2 + 1) { 3 + 1 } else { 2 }");

    typedef PAIR<THREE, FOUR> THREE_AND_FOUR;
    typedef typename GET<THREE_AND_FOUR, FST>::result_type FIRST;
    assert(FIRST::result == 3);
    static_assert(std::is_same<FIRST, THREE>::value, "fst (3,4) == 3");

    typedef DECREMENT<SUCC<SUCC<FOUR> > >::result_type FIVE;
    assert(FIVE::result == 6 - 1);
    static_assert(std::is_same<FIVE, SUCC<FOUR>>::value,
                  "(4 + 1 + 1) - 1 == 5");

    typedef GET<typename EVAL<
                    SLIDE,
                    typename EVAL<
                        SLIDE,
                        PAIR<ZERO, ZERO>
                    >::result_type
                >::result_type,
                FST>::result_type DEC_TWO;
    assert(DEC_TWO::result == 1);
    static_assert(std::is_same<DEC_TWO, ONE>::value, "2 - 1 == 1");

    typedef DECREMENT<TWO>::result_type DEC_TWO_V2;
    assert(DEC_TWO_V2::result == 1);
    static_assert(std::is_same<DEC_TWO_V2, ONE>::value, "2 - 1 == 1");

    return 0;
}
