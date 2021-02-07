/*
 * YNV_LIS3DSH.c
 *
 *  Created on: Jan 11, 2021
 *      Author: Arthur Chevrier
 */

#include "YNV_LIS3DSH.h"

//--------------------------------------------------GLOABAL VARIAL--------------------------------------------//

int16_t acc_Raw[3] = {0, 0, 0};
float axis_acceleration[3] = {0,0,0};
int16_t acc_Offset[3] = {0, 0, 0};
float convertion_Factor = 0;

//------------------------------------------------------------------------------------------------------------//

LIS3DSH_Status_t LIS3DSH_WriteReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t dataW){
	uint8_t Data[2] = {reg_addr,dataW};
	Data[0] &= 0x7F; //Set MSB to 0 for WRITE instruction

	CS_LOW;
	if (HAL_SPI_Transmit(spi, Data, 2, 1000)== HAL_OK){
		CS_HIGH;
		return LIS3DSH_OK;
	}
	else{
		CS_HIGH;
		return LIS3DSH_ERROR;
	}
};

LIS3DSH_Status_t LIS3DSH_ReadReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t *dataR,
		uint8_t size){

	uint8_t reg[1] = {reg_addr};
	uint8_t buffer[4] = {0};
	reg[0] |= 0x80; // 0b1XXX XXXX

	CS_LOW;
	if(HAL_SPI_TransmitReceive(spi, reg , buffer, size, 150)== HAL_OK){
		*dataR = buffer[1];
		CS_HIGH;
		return LIS3DSH_OK;
	}
	else{
		CS_HIGH;
		return LIS3DSH_ERROR;
	}
};

LIS3DSH_Status_t LIS3DSH_CheckReg(SPI_HandleTypeDef *spi,
		uint8_t reg_addr,
		uint8_t reg_value_expt,
		uint8_t size){

	uint8_t temp = 0;

	if (LIS3DSH_ReadReg(spi, reg_addr, &temp, 2) != LIS3DSH_OK){
		return LIS3DSH_ERROR;
	}
	else{
		if (temp != reg_value_expt){
			return LIS3DSH_ERROR;
		}
		else{
			return LIS3DSH_OK;
		}
	}
};

LIS3DSH_Status_t LIS3DSH_SetDataRate(SPI_HandleTypeDef *spi,
		uint8_t DataRate){
	uint8_t temp = 0;
	if(LIS3DSH_ReadReg(spi, CTRL_REG4, &temp, 2) == LIS3DSH_OK){
		temp = DataRate | temp;
		temp &= (DataRate | 0x0F); //and protect mask on other bit then set to zero
		if (LIS3DSH_WriteReg(spi, CTRL_REG4, temp) == LIS3DSH_OK){
			if(LIS3DSH_CheckReg(spi, CTRL_REG4, temp, 2) == LIS3DSH_OK){
				return LIS3DSH_OK;
			}
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_SetSensitivity(SPI_HandleTypeDef *spi,
		uint8_t Sensitivity){

	uint8_t temp = 0;
	switch (Sensitivity){
	case LIS3DSH_2G :
		convertion_Factor = 0.06;
		break;
	case LIS3DSH_4G :
		convertion_Factor = 0.12;
		break;
	case LIS3DSH_6G :
		convertion_Factor = 0.18;
		break;
	case LIS3DSH_8G :
		convertion_Factor = 0.24;
		break;
	case LIS3DSH_16G :
		convertion_Factor = 0.73;
		break;
	default :
		return LIS3DSH_ERROR;
	}

	if(LIS3DSH_ReadReg(spi, CTRL_REG5, &temp, 2) == LIS3DSH_OK){
		temp = Sensitivity | temp;
		temp &= (Sensitivity | 0xC7); //and protect mask on other bit then set to zero
		if (LIS3DSH_WriteReg(spi, CTRL_REG5, temp) == LIS3DSH_OK){
			if(LIS3DSH_CheckReg(spi, CTRL_REG5, temp, 2) == LIS3DSH_OK){
				return LIS3DSH_OK;
			}
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_Init(SPI_HandleTypeDef *spi,
		uint8_t Sensitivity,
		uint8_t DataRate){

	if(LIS3DSH_SetSensitivity(spi, Sensitivity) == LIS3DSH_OK){
		if(LIS3DSH_SetDataRate(spi, DataRate) == LIS3DSH_OK){
			return LIS3DSH_OK;
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_ReadX(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct){
	uint8_t temp = 0;

	if(LIS3DSH_ReadReg(spi, OUT_X_H, &temp, 2)== LIS3DSH_OK){
		data_struct->Accelerometer_X = (int16_t)(temp << 8);
		if(LIS3DSH_ReadReg(spi, OUT_X_L, &temp, 2)== LIS3DSH_OK){
			data_struct->Accelerometer_X |= (int16_t)temp;
			acc_Raw[X] = (data_struct->Accelerometer_X) - acc_Offset[0];
			axis_acceleration[X] = acc_Raw[X] * convertion_Factor;
			return LIS3DSH_OK;
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_ReadY(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct){
	uint8_t temp = 0;

	if(LIS3DSH_ReadReg(spi, OUT_Y_H, &temp, 2)== LIS3DSH_OK){
		data_struct->Accelerometer_Y = (int16_t)(temp << 8);
		if(LIS3DSH_ReadReg(spi, OUT_Y_L, &temp, 2)== LIS3DSH_OK){
			data_struct->Accelerometer_Y |= (int16_t)temp;
			acc_Raw[Y] = (data_struct->Accelerometer_Y) - acc_Offset[1];
			axis_acceleration[Y] = acc_Raw[Y] * convertion_Factor;
			return LIS3DSH_OK;
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_ReadZ(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct){
	uint8_t temp = 0;

	if(LIS3DSH_ReadReg(spi, OUT_Z_H, &temp, 2)== LIS3DSH_OK){
		data_struct->Accelerometer_Z = (int16_t)(temp << 8);
		if(LIS3DSH_ReadReg(spi, OUT_Z_L, &temp, 2)== LIS3DSH_OK){
			data_struct->Accelerometer_Z |= (int16_t)temp;
			acc_Raw[Z] = (data_struct->Accelerometer_Z) - acc_Offset[2];
			axis_acceleration[Z] = acc_Raw[Z] * convertion_Factor;
			return LIS3DSH_OK;
		}
	}
	return LIS3DSH_ERROR;
};

LIS3DSH_Status_t LIS3DSH_ReadAll(SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct){
	if(LIS3DSH_ReadX(spi, data_struct) == LIS3DSH_OK){
		if(LIS3DSH_ReadY(spi, data_struct) == LIS3DSH_OK){
			if(LIS3DSH_ReadZ(spi, data_struct) == LIS3DSH_OK){
				return LIS3DSH_OK;
			}
		}
	}
	return LIS3DSH_ERROR;
}

void LIS3DSH_Axis_Calibration (SPI_HandleTypeDef *spi,
		LIS3DSH_data_struct* data_struct){
	uint16_t i,j = 0;
	uint16_t max_samples = 50;
	for (i = 0; i < max_samples; i++){
		if(LIS3DSH_ReadAll(spi, data_struct) == LIS3DSH_OK){
			acc_Offset[X] += data_struct->Accelerometer_X;
			acc_Offset[Y] += data_struct->Accelerometer_Y;
			j++;
		}
		HAL_Delay(10);
		if (j == 50){
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			j=0;
		}
	}
	acc_Offset[X] /= max_samples;
	acc_Offset[Y] /= max_samples;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15,DISABLE);
};


