/* 
 * File:   timer.h
 * Author: daniil
 *
 * Created on March 3, 2018, 7:13 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>

unsigned int time = 0;

void increaseTimeCount(void) {
    time++;
}

unsigned int readTimer(void) {
    return time;
}


#endif	/* TIMER_H */
