/* 
 * File:   enums.h
 * Author: Daniil Orekhov
 *
 * Created on January 17, 2018, 5:35 PM
 */

#ifndef ENUMS_H
#define	ENUMS_H

enum machineMode {
    MODE_STANDBY,
    MODE_INPUT,
    MODE_RUNNING,
    MODE_LOGS  
};

enum inputMode {
    MODE_NO_INPUT,
    MODE_EMPTY_INPUT,
    MODE_INPUT_DESTINATION,
    MODE_INPUT_DIET,
    MODE_INPUT_DIET_NUM,
    MODE_INPUT_PROMPT,
    MODE_SHOW_INPUT,
    MODE_INPUT_COMPLETE
};

enum logsMode {
    MODE_OPERATIONS_COMPLETE, //do you want to log operations or view logs?
    MODE_SAVE_OPERATIONS,
    MODE_LOGS_PROMPT, //not enough memory, do you want to clean EEPROM?
    MODE_LOGGING, //logs are being saved... 
    MODE_LOGGING_COMPLETE, //logs are saved! Do you want to view them?
    MODE_VIEW_LOGS,
    MODE_VIEW_LOGS_GLCD, //attach to PC, press ? to begin transfer
    MODE_TRANSFER_LOGS,
    MODE_TRANSFERRING_LOGS, //transferring logs... on complete go to standby
    MODE_RETURN
};

enum diet {
    R,      // 0/1
    F,      // 1/2
    L,      // 2/3
    RF,     // 3/4
    RL,     // 4/5
    FL,     // 5/6
    RRF,    // 6/7
    RRL,    // 7/8
    RFF,    // 8/9
    RLL,    // 9/10
    RFL,    // 10/11
    FFL,    // 11/12
    FLL,    // 12/13
    RRFL,   // 13/14
    RFFL,   // 14/15
    RFLL,   // 15/16
    RLLL,   // 16/17
    FLLL    // 17/18
};

static const char *DIETS[] = {
    "R\0\0\0", "F\0\0\0", "L\0\0\0", "RF\0\0", "RL\0\0", "FL\0\0", "RRF\0", "RRL\0", "RFF\0", "RLL\0", "RFL\0", "FFL\0",
    "FLL\0", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"
};

#endif	/* ENUMS_H */
