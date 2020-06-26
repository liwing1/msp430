/*
 * kernel.h
 *
 *  Created on: 17 de jan de 2020
 *      Author: user
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include <stdint.h>

void registerTask(void *, uint8_t, uint8_t);
void startBRTOS(void);
void wait(uint32_t);


#endif /* KERNEL_H_ */
