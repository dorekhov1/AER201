/* 
 * File:   timer.h
 * Author: daniil
 *
 * Created on March 3, 2018, 7:13 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>

volatile unsigned int timer = 0;

void increaseTimeCount(void) {
    timer++;
}

void setTime(unsigned int t) {
    timer = t;
}


unsigned int readTimer(void) {
    return timer;
}


#endif	/* TIMER_H */
