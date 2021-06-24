#include "PWM.h"
#include <cassert>
#include <iostream>

struct testFunctor {
    testFunctor() = default;

    uint16_t operator()() {
        ++set;
        return set;
    }


    static uint16_t set;
};
uint16_t testFunctor::set = 0;

int main() {

    //should not trigger functor
    PWM<testFunctor> PWMtest1(0, HIGH);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest1();
        assert(testFunctor::set == 0);
    }

    PWM<testFunctor> PWMtest2(255, LOW);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest2();
        assert(testFunctor::set == 0);
    }

    //should trigger all the time
    PWM<testFunctor> PWMtest3(255, HIGH);
    for (size_t i = 0; i < 1024; ++i) {
        PWMtest3();
        assert(testFunctor::set == i + 1);
    }

    testFunctor::set = 0;//reset counter after previous test

    PWM<testFunctor> PWMtest4(0, LOW);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest4();
        assert(testFunctor::set == i + 1);
    }

    testFunctor::set = 0;//reset counter after previous test

    //should count to max
    PWM<testFunctor> PWMtest6(10, HIGH);
    for (size_t i = 0; i < 256; ++i) {
        PWMtest6();
        assert(testFunctor::set == std::min(i + 1, (unsigned long int) 10));
    }


    testFunctor::set = 0;//reset counter after previous test

    PWM<testFunctor> PWMtest5(127, HIGH_TO_LOW);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest5();
        if (i == 127) {
            assert(testFunctor::set == 1);
        }
    }
    assert(testFunctor::set == 2);

    testFunctor::set = 0;//reset counter after previous test

    PWM<testFunctor> PWMtest7(127, BOTH);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest7();
        if (i == 127) {
            assert(testFunctor::set == 2);
        }
    }
    assert(testFunctor::set == 4);

    testFunctor::set = 0;//reset counter after previous test

    PWM<testFunctor> PWMtest8(127, BOTH, HIGH);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest8();
        if (i == 127) {
            assert(testFunctor::set == 1);
        }
    }
    assert(testFunctor::set == 3);

    testFunctor::set = 0;//reset counter after previous test
    //should trigger once, because it is starting at HIGH
    PWM<testFunctor> PWMtest9(0, BOTH, HIGH);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest9();
    }
    assert(testFunctor::set == 1);


    testFunctor::set = 0;//reset counter after previous test
    //should not trigger
    PWM<testFunctor> PWMtest10(255, BOTH, HIGH);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest10();
    }
    assert(testFunctor::set == 0);


    testFunctor::set = 0;//reset counter after previous test
    //should not trigger
    PWM<testFunctor> PWMtest11(127, LOW_TO_HIGH, LOW);
    for (size_t i = 0; i < 512; ++i) {
        PWMtest11();
        if (i == 1) {
            assert(testFunctor::set == 1);
        }
        if (i == 256) {
            assert(testFunctor::set == 2);
        }
    }
    return 0;
}
