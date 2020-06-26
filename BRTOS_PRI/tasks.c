#include <msp430.h>
#include <stdint.h>
#include "kernel.h"

void taskA(void);
void taskB(void);

void taskA(void){
    P1DIR |= BIT0;
    while(1){
        //volatile uint32_t i = 200000;
        //while(i--); // em intervalos periódicos
        //wait(500);
        P1OUT ^= BIT0;
    }
}

void taskB(void){
    P1DIR |= BIT1;
    while(1){
        //volatile uint32_t i = 100000;
        //while(i--); // em intervalos periódicos
        //wait(250);
        P1OUT ^= BIT1;
    }
}
