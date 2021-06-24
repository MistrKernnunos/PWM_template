//
// Created by machavi on 6/24/21.
//
#pragma once
#include <iostream>

enum STATE { HIGH,
             LOW };

enum TRIGGER { LOW_TO_HIGH,
               HIGH_TO_LOW,
               BOTH };
/**
 * template class for PWM, calls function on transitions or states
 * @tparam callback calls this functor on transition or state defined in constructor
 */
template<typename callback>
class PWM {
public:
    /**
     * construct PWM template that triggers callback functor on every tick when in activeState
     * @param activeState State when the callback is triggered
     */
    PWM(uint8_t duty, STATE activeState) : m_Duty(duty), m_ActiveState(activeState){};
    /**
     * construct instance that triggers callback only on state changes. Starts at LOW
     * @param duty length of HIGH state
     * @param trigger state changes when to trigger callback (low to high, high to low, both)
     * @param startState starting state of PWM
     */
    PWM(uint8_t duty, TRIGGER trigger, STATE startState = LOW) : m_Duty(duty), m_Trigger(trigger), m_TriggerOnChange(true), m_CurrState(startState){};


    /*
     * increments internal counter and call callback if set transition occurs or is in active state
     */
    void operator()();

private:
    callback m_Callback;
    const uint8_t m_Duty;

    const STATE m_ActiveState = HIGH;
    const TRIGGER m_Trigger = LOW_TO_HIGH;
    bool m_TriggerOnChange = false;

    volatile uint8_t m_CurrStep = 0;
    volatile STATE m_CurrState = HIGH;
    volatile STATE m_PrevState = LOW;
};
template<typename callback>

void PWM<callback>::operator()() {

    m_PrevState = m_CurrState;

    if (m_CurrStep >= m_Duty && m_Duty != 255) {
        m_CurrState = LOW;
    } else {
        m_CurrState = HIGH;
    }

    if (m_TriggerOnChange) {
        if (m_PrevState != m_CurrState) {
            if (m_PrevState == LOW && (m_Trigger == LOW_TO_HIGH || m_Trigger == BOTH)) {
                m_Callback();
            } else if (m_PrevState == HIGH && (m_Trigger == HIGH_TO_LOW || m_Trigger == BOTH)) {
                m_Callback();
            }
        }
    } else {
        if (m_CurrState == m_ActiveState) {
            m_Callback();
        }
    }
    ++m_CurrStep;
}
