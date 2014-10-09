/* 
 * File:   data.h
 * Author: doltsa
 *
 * Created on 18. septembre 2014, 16:39
 */

#ifndef DATA_H
#define	DATA_H

#include <stdint.h>

void data_init(void);
uint8_t data_get(uint8_t addr);
void data_set(uint8_t addr, uint8_t value);



#define REG_M1_DONE (0x10)
#define REG_EN_M1 (0x11)
#define REG_DIR_M1 (0x12)
#define REG_PWM_M1 (0x13)

#define REG_M2_DONE (0x20)
#define REG_EN_M2 (0x21)
#define REG_DIR_M2 (0x22)
#define REG_PWM_M2 (0x23)

#define SONAR_VALUE (0x30)
#define SONAR_ENABLE (0x31)

#define SERVO_DONE (0x40)
#define SERVO_ANGLE (0x41)

#define FUNC_GO (0x50)
#define FUNC_GO_DIR (0x51)

#define LF_EN (0x60)
#define LF_VAL1 (0x61)
#define LF_VAL2 (0x62)
#define LF_VAL3 (0x63)
#define LF_VAL4 (0x64)
#define LF_VAL5 (0x65)

#define STOP_TO_LINE (0x70)
#define LINE_FOLLOW (0x71)


#endif	/* DATA_H */

