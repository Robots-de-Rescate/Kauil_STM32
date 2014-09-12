#include "MD03.h"

void MD03_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  
  /* Enable the I2C periph */
  RCC_APB1PeriphClockCmd(MD03_I2C_CLK, ENABLE);
  
  /* Enable SCK and SDA GPIO clocks */
  RCC_AHBPeriphClockCmd(MD03_I2C_SCK_GPIO_CLK | MD03_I2C_SDA_GPIO_CLK , ENABLE);
  
  GPIO_PinAFConfig(MD03_I2C_SCK_GPIO_PORT, MD03_I2C_SCK_SOURCE, MD03_I2C_SCK_AF);
  GPIO_PinAFConfig(MD03_I2C_SDA_GPIO_PORT, MD03_I2C_SDA_SOURCE, MD03_I2C_SDA_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* I2C SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = MD03_I2C_SCK_PIN;
  GPIO_Init(MD03_I2C_SCK_GPIO_PORT, &GPIO_InitStructure);
  
  /* I2C SDA pin configuration */
  GPIO_InitStructure.GPIO_Pin =  MD03_I2C_SDA_PIN;
  GPIO_Init(MD03_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  /* I2C configuration -------------------------------------------------------*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x00902025;
  
  /* Apply MD03_I2C configuration after enabling it */
  I2C_Init(MD03_I2C, &I2C_InitStructure);
  
  /* MD03_I2C Peripheral Enable */
  I2C_Cmd(MD03_I2C, ENABLE);
  
}  

uint16_t MD03_Write(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer)
{  
  ///* Test on BUSY Flag */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_BUSY) != RESET)
  {
  //  if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(MD03_I2C, DeviceAddr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_TXIS) == RESET)   
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(MD03_I2C, (uint8_t) RegAddr);
  
  /* Wait until TCR flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_TCR) == RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(MD03_I2C, DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
       
  /* Wait until TXIS flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_TXIS) == RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }  
    
  /* Write data to TXDR */
  I2C_SendData(MD03_I2C, *pBuffer);
      
  /* Wait until STOPF flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_STOPF) == RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(MD03_I2C, I2C_ICR_STOPCF);
  
  return 0;
}

uint16_t MD03_Read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{    
  /* Test on BUSY Flag */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_BUSY) != RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(MD03_I2C, DeviceAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_TXIS) == RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  if(NumByteToRead>1)
      RegAddr |= 0x80;

  
  /* Send Register address */
  I2C_SendData(MD03_I2C, (uint8_t)RegAddr);
  
  /* Wait until TC flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_TC) == RESET)
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(MD03_I2C, DeviceAddr, NumByteToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until all data are received */
  while (NumByteToRead)
  {   
    /* Wait until RXNE flag is set */
    //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_RXNE) == RESET)    
    {
      //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
    }
    
    /* Read data from RXDR */
    *pBuffer = I2C_ReceiveData(MD03_I2C);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
    
    /* Decrement the read bytes counter */
    NumByteToRead--;
  } 
  
  /* Wait until STOPF flag is set */
  //LSM303DLHC_Timeout = LSM303DLHC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(MD03_I2C, I2C_ISR_STOPF) == RESET)   
  {
    //if((LSM303DLHC_Timeout--) == 0) return LSM303DLHC_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(MD03_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK */
  return 0;  
}  

void set_motor_speed(uint8_t MD03_Address, uint8_t speed, uint8_t direction) {

    MD03_Write(MD03_Address, 0x02, &speed);
    MD03_Write(MD03_Address, 0x00, &direction);
    
}

void motors_speed(int32_t left_speed, int32_t right_speed){
    uint8_t left_direction = 1;
    uint8_t right_direction = 1;

    if ( left_speed<0 ) {
        left_speed += 255;
        left_direction = 2;
    }

    set_motor_speed(LEFT_MOTOR_ADDRESS, left_speed, left_direction);

    if ( right_speed<0 ) {
        right_speed += 255;
        right_direction = 2;
    }

    set_motor_speed(RIGHT_MOTOR_ADDRESS, right_speed, right_direction);
}

