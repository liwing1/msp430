#include <msp430.h> 
#include <stdint.h>
#include "kernel.h"
#include "tasks.h"
#include "clock.h"


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    clockInit();
	PM5CTL0 &= ~LOCKLPM5;

	registerTask(taskA, 2, 2);

	registerTask(taskB, 2, 1);

	startBRTOS();
}
