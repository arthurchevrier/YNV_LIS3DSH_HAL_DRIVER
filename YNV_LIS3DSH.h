/*
 * YNV_LIS3DSH.h
 *
 *  Created on: Jan 11, 2021
 *      Author: Arthur Chevrier
 */

#ifndef INC_YNV_LIS3DSH_H_
#define INC_YNV_LIS3DSH_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/* global variables *Debug* */

extern uint16_t X_Value;
extern int16_t acc_Raw[3];
extern int16_t acc_Offset[3];
extern float axis_acceleration[3];
extern float convertion_Factor;

/*private define */

#define X					0
#define Y					1
#define Z 					2
#define CS_LOW 				HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port,CS_I2C_SPI_Pin,GPIO_PIN_RESET)
#define CS_HIGH 			HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin,GPIO_PIN_SET)

/* register address define  */

#define OUT_T 				0x0C
#define INFO1				0x0D
#define INFO2				0x0E
#define WHO_AM_I			0x0F
#define OFF_X				0x10
#define OFF_Y				0x11
#define OFF_Z				0x12
#define CS_X				0x13
#define CS_Y				0x14
#define CS_Z				0x15
#define LC_L				0x16
#define LC_H				0x17
#define STAT 				0x18
#define PEAK1				0x19
#define PEAK2				0x1A
#define VFC_1				0x1B
#define VFC_2				0x1C
#define VFC_3				0x1D
#define VFC_4				0x1E
#define THRS3				0x1F
#define CTRL_REG4			0x20
#define CTRL_REG1			0x21
#define CTRL_REG2			0x22
#define CTRL_REG3			0x23
#define CTRL_REG5			0x24
#define CTRL_REG6			0x25
#define STATUS				0x27
#define OUT_X_L				0x28
#define OUT_X_H				0x29
#define OUT_Y_L				0x2A
#define OUT_Y_H				0x2B
#define OUT_Z_L				0x2C
#define OUT_Z_H				0x2D
#define FIFO_CTRL			0x2E
#define FIFO_SRC			0x2F
#define ST1_X 				0x40
#define TIM4_1				0x50
#define TIM3_1				0x51
#define TIM2_1				0x52
#define TIM1_1				0x54
#define THRS2_1				0x56
#define THRS1_1				0x57
#define MASK1_B				0x59
#define MASK1_A				0x5A
#define SETT1				0x5B
#define PR1					0x5C
#define TC1					0x5D
#define OUTS1				0x5F
#define ST2_X				0x60
#define TIM4_2				0x70
#define TIM3_2				0x71
#define TIM2_2				0x72
#define TIM1_2				0x74
#define THRS2_2				0x76
#define THRS1_2				0x77
#define DES2				0x78
#define MASK2_B				0x79
#define MASK2_A				0x7A
#define SETT2				0x7B
#define PR2					0x7C
#define TC2					0x7D
#define OUTS2				0x7F

/* Configuration parameters define */

//#define READ_REG 			((uint8_t)0x80)
//#define WRITE_REG			((uint8_t)0x00)

#define LIS3DSH_POW_DOWN    0x00
#define LIS3DSH_RATE_3_125	0x10
#define LIS3DSH_RATE_6_25	0x20
#define LIS3DSH_RATE_12_5	0x30
#define LIS3DSH_RATE_25		0x40
#define LIS3DSH_RATE_50		0x50
#define LIS3DSH_RATE_100	0x60
#define LIS3DSH_RATE_400	0x70
#define LIS3DSH_RATE_800	0x80
#define LIS3DSH_RATE_1600	0x90

#define LIS3DSH_2G			0x00
#define LIS3DSH_4G			0x08
#define LIS3DSH_6G			0x10
#define LIS3DSH_8G			0x18
#define LIS3DSH_16G			0x20

/* Return status define */

#define LIS3DSH_STATUS_OK 	"LIS3DSH OK !\n\r"
#define LIS3DSH_STATUS_ERROR	"LIS3DSH ERROR !\n\r"

/* status enum */

typedef enum
{
	LIS3DSH_OK,
	LIS3DSH_ERROR
}LIS3DSH_Status_t;

/* data struct*/

typedef struct{
	int16_t Accelerometer_X; /*< Accelerometer value X axis */
	int16_t Accelerometer_Y; /*< Accelerometer value Y axis */
	int16_t Accelerometer_Z; /*< Accelerometer value Y axis */
	float   Temperature;       /*< Temperature in degrees */
}LIS3DSH_data_struct;

extern LIS3DSH_data_struct Test_Data_s;

void LIS3DSH_Axis_Calibration(SPI_HandleTypeDef *spi,
LIS3DSH_data_struct* data_struct);

LIS3DSH_Status_t LIS3DSH_WriteReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t dataW);

LIS3DSH_Status_t LIS3DSH_ReadReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t *dataR,
		uint8_t size);

LIS3DSH_Status_t LIS3DSH_CheckReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t reg_value_expt,
		uint8_t size);

LIS3DSH_Status_t LIS3DSH_Init(SPI_HandleTypeDef *spi,
		uint8_t Sensitivity,
		uint8_t DataRate);

LIS3DSH_Status_t LIS3DSH_ReadX(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct);

LIS3DSH_Status_t LIS3DSH_ReadY(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct);

LIS3DSH_Status_t LIS3DSH_ReadZ(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct);

LIS3DSH_Status_t LIS3DSH_ReadAll(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct);

LIS3DSH_Status_t LIS3DSH_SetDataRate(SPI_HandleTypeDef *spi,
		uint8_t DataRate);

LIS3DSH_Status_t LIS3DSH_SetSensitivity(SPI_HandleTypeDef *spi,
		uint8_t Sensitivity);

#endif /* INC_YNV_LIS3DSH_H_ */
