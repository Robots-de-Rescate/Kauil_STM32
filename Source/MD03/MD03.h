#include "stm32f30x_syscfg.h"
#include "stm32f30x_exti.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_spi.h"
#include "stm32f30x_i2c.h"
#include "stm32f30x.h"

#define LEFT_MOTOR_ADDRESS 0xB1
#define RIGHT_MOTOR_ADDRESS 0xB2

/****************** PIN ASSIGNMENT **********************/
#define MD03_I2C                       I2C2
#define MD03_I2C_CLK                   RCC_APB1Periph_I2C2

#define MD03_I2C_SCK_PIN               GPIO_Pin_1                  /* PF.1 */
#define MD03_I2C_SCK_GPIO_PORT         GPIOF                       /* GPIOF */
#define MD03_I2C_SCK_GPIO_CLK          RCC_AHBPeriph_GPIOF
#define MD03_I2C_SCK_SOURCE            GPIO_PinSource1
#define MD03_I2C_SCK_AF                GPIO_AF_4

#define MD03_I2C_SDA_PIN               GPIO_Pin_10                 /* PA.10 */
#define MD03_I2C_SDA_GPIO_PORT         GPIOA                       /* GPIOA */
#define MD03_I2C_SDA_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define MD03_I2C_SDA_SOURCE            GPIO_PinSource10
#define MD03_I2C_SDA_AF                GPIO_AF_4

void MD03_Init(void);
uint16_t MD03_Write(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer);
uint16_t MD03_Read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer, uint16_t NumByteToRead);
void set_motor_speed(uint8_t MD03_Address, uint8_t speed, uint8_t direction);
void set_motors_speed(int32_t left_speed, int32_t right_speed);
