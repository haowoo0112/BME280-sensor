#include "BME280.hpp"
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_if.h"

void BME280_setting(struct bme280_settings *BME280){

	BME280->osr_p = 1;

	BME280->osr_t = 1;

	BME280->osr_h = 1;

	BME280->mode = 3;


}

void BME280_write_setting(bme280_settings *BME280){
	/* register 0xF2
	 * bit 2, 1, 0: oversampling of humidity data
	*/
	uint8_t setting_1[] = {0xF2, BME280->osr_h};
	HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDR, setting_1, 2, 1);

	/* register 0xF4
	 * bit 7, 6, 5: oversampling of temperature data
	 * bit 4, 3, 2: oversampling of pressure data
	 * bit 1, 0: mode: 0:sleep 1: forced mode 2: normal mode
	*/
	uint8_t setting_value = BME280->mode|(BME280->osr_p<<2)|(BME280->osr_t<<5);
	uint8_t setting_2[] = {0xF4, setting_value};
	HAL_I2C_Master_Transmit(&hi2c2, BME280_ADDR, setting_2, 2, 1);
}

void BME280_init(bme280_dev *BME280){

	BME280_setting(&BME280->settings);

	BME280_write_setting(&BME280->settings);

}

void BME280_read_sensor(bme280_sensor_data *BME280){
	uint8_t tmp[10];
	HAL_StatusTypeDef ret;
	//read H
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xFD, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->humidity = (tmp[0]<<8) | tmp[1];
	check_HAL_status(ret, 0xFD);
	//read P
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xF7, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 3, 100);
	BME280->pressure = (tmp[0]<<12) | (tmp[1]<<4) | (tmp[2]>>4);
	check_HAL_status(ret, 0xF7);
	//read T
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xFA, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 3, 100);
	BME280->temperature = (tmp[0]<<12) | (tmp[1]<<4) | (tmp[2]>>4);
	check_HAL_status(ret, 0xFA);
}

void BME280_read_calib_data(bme280_calib_data *BME280){
	uint8_t tmp[10];
	HAL_StatusTypeDef ret;

	// read temperature calib data
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x88, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_T1 = (tmp[1]<<8) | tmp[0];HAL_Delay(10);
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x8A, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_T2 = (tmp[1]<<8) | tmp[0];HAL_Delay(10);
	ret = HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x8C, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_T3 = (tmp[1]<<8) | tmp[0];HAL_Delay(10);
	char DEBUG_cmd[300] = "";
	sprintf(DEBUG_cmd, "t1: %.2f t2: %.2f t3: %.2f\r\n", BME280->dig_T1, BME280->dig_T2, BME280->dig_T3);
	CDC_Transmit_FS((uint8_t*)DEBUG_cmd, strlen(DEBUG_cmd));

	// read pressure calib data
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x8E, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P1 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x90, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P2 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x92, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P3 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x94, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P4 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x96, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P5 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x98, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P6 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x9A, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P7 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x9C, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P8 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0x9E, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_P9 = (tmp[1]<<8) | tmp[0];

	// read humidity calib data
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xA1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 1, 100);
	BME280->dig_H1 = tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xE1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_H2 = (tmp[1]<<8) | tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xE3, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 1, 100);
	BME280->dig_H3 = tmp[0];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xE4, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_H4 = (tmp[0]<<4) | tmp[1];
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xE5, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 2, 100);
	BME280->dig_H5 = (tmp[1]<<4) | (tmp[0]>>4);
	HAL_I2C_Mem_Read(&hi2c2, (uint8_t)BME280_ADDR, 0xE7, I2C_MEMADD_SIZE_8BIT, (uint8_t*)tmp, 1, 100);
	BME280->dig_H6 = tmp[0];

}


void BME280_read(bme280_dev *BME280){

	BME280_read_sensor(&BME280->sensor_data);

	BME280_read_calib_data(&BME280->calib_data);

	BME280_compensate_T(&BME280->sensor_data, &BME280->calib_data, &BME280->compensated_sensor_data);
	BME280_compensate_P(&BME280->sensor_data, &BME280->calib_data, &BME280->compensated_sensor_data);
	BME280_compensate_H(&BME280->sensor_data, &BME280->calib_data, &BME280->compensated_sensor_data);

}
void BME280_print(struct bme280_sensor_data *compensated_sensor_data){
	char DEBUG_cmd[300] = "";
	sprintf(DEBUG_cmd, "BME_H: %.2f\r\n", compensated_sensor_data->humidity);
	CDC_Transmit_FS((uint8_t*)DEBUG_cmd, strlen(DEBUG_cmd));
}

void BME280_compensate_T(bme280_sensor_data *sensor_data, bme280_calib_data *calib_data, bme280_sensor_data *compensated_sensor_data){
	double var1, var2;

	var1 = (((double)sensor_data->temperature)/16384.0 - ((double)calib_data->dig_T1)/1024.0) * ((double)calib_data->dig_T2);
	var2 = ((((double)sensor_data->temperature)/131072.0 - ((double)calib_data->dig_T1)/8192.0) * (((double)sensor_data->temperature)/131072.0 - ((double) calib_data->dig_T1)/8192.0)) * ((double)calib_data->dig_T3);
	calib_data->t_fine = (int32_t)(var1 + var2);
	compensated_sensor_data->temperature = (var1 + var2) / 5120.0;

	char DEBUG_cmd[300] = "";
	sprintf(DEBUG_cmd, "var1: %.2f var2: %.2f temperature: %.2f\r\n", var1, var2, compensated_sensor_data->temperature);
	CDC_Transmit_FS((uint8_t*)DEBUG_cmd, strlen(DEBUG_cmd));
}
void BME280_compensate_P(bme280_sensor_data *sensor_data, bme280_calib_data *calib_data, bme280_sensor_data *compensated_sensor_data){
	double var1, var2;
	double p;

	var1 = ((double)calib_data->t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)calib_data->dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)calib_data->dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)calib_data->dig_P4) * 65536.0);
	var1 = (((double)calib_data->dig_P3) * var1 * var1 / 524288.0 + ((double)calib_data->dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)calib_data->dig_P1);
	if (var1 == 0.0){
		return; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)sensor_data->pressure;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)calib_data->dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)calib_data->dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)calib_data->dig_P7)) / 16.0;
	compensated_sensor_data->pressure = p;

}

void BME280_compensate_H(bme280_sensor_data *sensor_data, bme280_calib_data *calib_data, bme280_sensor_data *compensated_sensor_data){
	double var_H;

	var_H = (((double)calib_data->t_fine) - 76800.0);
	var_H = (sensor_data->humidity - (((double)calib_data->dig_H4) * 64.0 + ((double)calib_data->dig_H5) / 16384.0 *
	var_H)) * (((double)calib_data->dig_H2) / 65536.0 * (1.0 + ((double)calib_data->dig_H6) / 67108864.0 * var_H * (1.0 + ((double)calib_data->dig_H3) / 67108864.0 * var_H)));
	var_H = var_H * (1.0 - ((double)calib_data->dig_H1) * var_H / 524288.0);
	if (var_H > 100.0)
		var_H = 100.0;
	else if (var_H < 0.0)
		var_H = 0.0;

	compensated_sensor_data->humidity = var_H;
}

void check_HAL_status(HAL_StatusTypeDef ret, uint8_t address){
	char DEBUG_cmd[50] = {};
	if(ret != HAL_OK){
		strncpy(DEBUG_cmd, "", sizeof(DEBUG_cmd));
		sprintf(DEBUG_cmd, "BME address %d don't receive data.\r\n", address);
		CDC_Transmit_FS((uint8_t*)DEBUG_cmd, strlen(DEBUG_cmd));
	}
}
