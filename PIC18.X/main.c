/* 
 * File:   main.c
 * Author: Daniil Orekhov
 *
 * Created on January 13, 2018, 3:27 PM
 */

#include "main.h"

Operation operations[8];
unsigned int operationNum;

unsigned int currentMachineMode = MODE_STANDBY;

/* Initialize local variables. */
unsigned char mem[3]; // Initialize array to check for triple-A sequence
unsigned char counter = 0; // Increments each time a byte is sent
unsigned char keypress; // Stores the data corresponding to the last key press
unsigned char data; // Holds the data to be sent/received

unsigned int rTotalCount = 0;
unsigned int fTotalCount = 0;
unsigned int lTotalCount = 0;

unsigned int rPassedRecently = 0;
unsigned int fPassedRecently = 0;
unsigned int lPassedRecently = 0;

unsigned int secondsWithoutR = 0;
unsigned int secondsWithoutF = 0;
unsigned int secondsWithoutL = 0;

unsigned int tapedDrawers[4];
unsigned int tapedDrawersNum = 0;

void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0xFF; // All inputs (this is the default, but is explicated here for learning purposes)
    TRISB = 0xFF;
    TRISC = 0x00;
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    
    /***************************** Timer 0 Module *****************************/
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 0;
    TMR0H = 0x67;
    TMR0L = 0x69;
    T0CONbits.TMR0ON = 1;
    TMR0IE = 1;
    
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Initialize EUSART module for asynchronous 9600/8N1">
    /* Configure the baud rate generator for 9600 bits per second. */
    long baudRate = 9600;
    SPBRG = (unsigned char)((_XTAL_FREQ / (64 * baudRate)) - 1);
    
    /* Configure transmit control register */
    TXSTAbits.TX9 = 0; // Use 8-bit transmission (8 data bits, no parity bit)
    TXSTAbits.SYNC = 0; // Asynchronous communication
    TXSTAbits.TXEN = 1; // Enable transmitter
    __delay_ms(5); // Enabling the transmitter requires a few CPU cycles for stability
    
    /* Configure receive control register */
    RCSTAbits.RX9 = 0; // Use 8-bit reception (8 data bits, no parity bit)
    RCSTAbits.CREN = 1; // Enable receiver
    
    /* Enforce correct pin configuration for relevant TRISCx */
    TRISC6 = 0; // TX = output
    TRISC7 = 1; // RX = input
    
    /* Enable serial peripheral */
    RCSTAbits.SPEN = 1;
    // </editor-fold>
    
    INT1IE = 1; // Enable RB1 (keypad data available) interrupt
    ei(); // Enable all interrupts
    
    initLCD();
    
    I2C_Master_Init(100000);
       
    if (read_octet_eep(1) == 255) cleanEEPROM();
    
    while (1) {
        
        if (currentMachineMode == MODE_STANDBY) enterStandbyMode();
            
        if (currentMachineMode == MODE_INPUT) {
            if (getInputMode() == MODE_EMPTY_INPUT) askForOperationInput();
            else if (getInputMode() == MODE_INPUT_DESTINATION) askForDestination();
            else if (getInputMode() == MODE_INPUT_DIET) askForDietType();
            else if (getInputMode() == MODE_INPUT_DIET_NUM) askForDietNum();
            else if (getInputMode() == MODE_INPUT_PROMPT) showPrompt();
            else if (getInputMode() == MODE_SHOW_INPUT) {
                int opNum = getDisplayedOperationNum();
                Operation operation = operations[opNum];
                showInput(opNum, DIETS[operation.dietType - 1], operation.dietNum, operation.destination);
            }
        }
        
        if (currentMachineMode == MODE_RUNNING) {
            
//            startTopCounters();
//                        
//            int tapeValues[16];
//            readTapes(tapeValues);
//            tapedDrawersNum = processTapes(tapeValues, tapedDrawers);
//            operationNum = discardOperations(tapedDrawers, tapedDrawersNum);
//            optimizeOperationOrder(operationNum);
//            
//            int operationsExecuted = 0;
//            int currentColumn = 4;
//            while (operationsExecuted != operationNum) {
//                if (columnNeedsFood(currentColumn, operationsExecuted)) {
//                    openAllDrawers();
//                    int currentRow = 1;
//                    while (columnNeedsFood(currentColumn, operationsExecuted)) {
//                        if (rowNeedsFood(currentRow, operationsExecuted)) {
//                            Operation o = operations[++operationsExecuted];
//                            determineFoodCount(DIETS[o.dietType], o.dietNum);
//                            countFood();
//                        }
//                        closeDrawer(++currentRow);
//                    }
//                }
//                moveToColumn(currentColumn--);
//            }
            
            //while pills counted recently
            
            //stopTopCounters();
            
            recordTime();
            LATCbits.LATC2 = 0; //enable keypad
            currentMachineMode = MODE_LOGS;
            setLogsMode(MODE_OPERATIONS_COMPLETE);
            setFromStandby(0);
            setLogsSaved(0);

        }
        
        if (currentMachineMode == MODE_LOGS) {
            __lcd_display_control(1, 0, 0)
            if (getLogsMode() == MODE_OPERATIONS_COMPLETE) showOperationsComplete();
            else if (getLogsMode() == MODE_LOGS_PROMPT) showLogsPrompt();
            else if (getLogsMode() == MODE_LOGGING) {
                showLogging();
                logCreatedOperations(getOperationNum());
                logTapedDrawers(tapedDrawers, tapedDrawersNum);
                logCounts(rTotalCount, fTotalCount, lTotalCount);
                logTime();
                completeLogs();
            }
            else if (getLogsMode() == MODE_LOGGING_COMPLETE) showLoggingComplete();
            else if (getLogsMode() == MODE_VIEW_LOGS) showViewLogs();
            else if (getLogsMode() == MODE_TRANSFERRING_LOGS) {
                showTransferringLogs();
                sendLogsToPC();
                completeTransfer();
            }
            else if (getLogsMode() == MODE_RETURN) currentMachineMode = MODE_STANDBY;
            __lcd_display_control(1, 1, 0)
        }
    }
}

void enterStandbyMode(void) {
    setOperationNum(0);
    setArduinoToStandby();
    __lcd_clear();
    
    __lcd_display_control(1, 0, 0);
    printf("Welcome!");
    while (currentMachineMode == MODE_STANDBY);
}

void createOperation(void) { 
    operations[getOperationNum()].destination = getNewDestination();
    operations[getOperationNum()].dietNum = getNewDietNum();
    operations[getOperationNum()].dietType = getNewDietType();
    setOperationNum(getOperationNum() + 1);
    setOperationReady(0);
}

void deleteOperation(int num) {
    for (int i = num; i<getOperationNum(); i++) {
        operations[i].destination = operations[i+1].destination;
        operations[i].dietNum = operations[i+1].dietNum ;
        operations[i].dietType = operations[i+1].dietType;
    }
    setOperationNum(getOperationNum() - 1);
    setDeleteOperation(0);
}

int discardOperations(int* tapedDrawers, int tapedDrawersNum) {
    int operationNum = getOperationNum();
    for (int i=0; i<operationNum; i++) {
        for (int j = 0; j < tapedDrawersNum; j++) {
            if (operations[i].destination == tapedDrawers[j]) {
                operations[i].destination = operations[i+1].destination;
                operations[i].dietNum = operations[i+1].dietNum ;
                operations[i].dietType = operations[i+1].dietType;
                i--;
                operationNum--;
                break;
            }
        }
    }
    return operationNum;
}

void optimizeOperationOrder(int operationNum){
    int accountedOperationNum = 0;
    int order[] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
    
    for (int i=0; i<16; i++) {
        for (int j = accountedOperationNum; j<operationNum; j++) {
            if (operations[j].destination == order[i]) {
                Operation temp = operations[accountedOperationNum];
                operations[accountedOperationNum] = operations[j];
                operations[j] = temp;
                accountedOperationNum++;
                break;
            }
        }
    }
}

int columnNeedsFood(int column, int operationsExecuted) {
    return operations[operationsExecuted].destination % 4 == column;
}

int rowNeedsFood(int row, int operationsExecuted) {
    return operations[operationsExecuted].destination / 4 == row;
}

void logCreatedOperations(int operationNum){
    logOperationNum(operationNum);
    logTime();
    
    for (int i=0; i<operationNum; i++) {
        Operation o = operations[i];        
        logOperation(o.destination, o.dietType, o.dietNum);
    }
}

void interrupt interruptHandler(void){
    if (LATCbits.LATC2 == 0 && INT1IF == 1) {
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        if (currentMachineMode == MODE_STANDBY) {
            if (keypress == 3) {
                currentMachineMode = MODE_INPUT;
                setInputMode(MODE_EMPTY_INPUT);
            }
            else if (keypress == 14) {
                currentMachineMode = MODE_LOGS;
                setLogsMode(MODE_VIEW_LOGS);
                setFromStandby(1);
            }
        }
        else if (currentMachineMode == MODE_INPUT) {
            processInputInterrupt(keypress);
            if (getInputMode() == MODE_NO_INPUT) currentMachineMode = MODE_STANDBY;
            if (isOperationReady()) createOperation();
            if (needToDeleteOperation()) deleteOperation(getDisplayedOperationNum());
            if (getInputMode() == MODE_INPUT_COMPLETE) {
                LATCbits.LATC2 = 1; //disable keypad
                currentMachineMode = MODE_RUNNING;
            }
        }
        else if (currentMachineMode == MODE_LOGS) {
            processLogsInterrupt(keypress);
            if (getLogsMode() == MODE_RETURN) currentMachineMode = MODE_STANDBY;
        }
        INT1IF = 0;
    }
    else if (LATCbits.LATC2 == 1) {
        if (INT0IF == 1) {
            rTotalCount++;
            rPassedRecently = 1;
            secondsWithoutR = 0;
        }
        else if (INT1IF == 1) {
            fTotalCount++;
            fPassedRecently = 1;
            secondsWithoutF = 0;
        }
        else if (INT2IF == 1){
            lTotalCount++;
            lPassedRecently = 1;
            secondsWithoutL = 0;
        }
    }
    else if (TMR0IF == 1) {
        printTimeToGLCD();
        TMR0IF = 0;
        
        if (currentMachineMode == MODE_RUNNING) {
            if (!rPassedRecently) secondsWithoutR++;
            if (!fPassedRecently) secondsWithoutF++;
            if (!lPassedRecently) secondsWithoutL++;
        }
    } 
}