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

    typedef SUCC<TWO> THREE;
    assert(THREE::result == 3);

    typedef typename IF<
                        EQUALS<ONE,
                               ONE>,
                        TWO,
                        THREE>::result_type COND_EVAL;
    assert(COND_EVAL::result == 2);

    typedef typename EQUALS<ONE, ONE>::result_type EQ;
    assert(EQ::result == true);

    typedef typename IF<
                        EQUALS<THREE,
                               SUCC<TWO>>,
                        SUCC<THREE>,
                        TWO>::result_type FOUR;
    assert(FOUR::result == 4);

    return 0;
}
