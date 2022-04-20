/*
 * BME280.hpp
 *
 *  Created on: 2022年4月18日
 *      Author: CYTENA
 */

#ifndef INC_BME280_HPP_
#define INC_BME280_HPP_

#include "stm32f4xx.h"

#define BME280_ADDR (0x76 << 1)

extern I2C_HandleTypeDef hi2c2;

void BME280_read(struct bme280_dev*);
void BME280_init(struct bme280_dev*);
void BME280_setting(struct bme280_settings*);
// read sensor data
void BME280_read_sensor(struct bme280_sensor_data*);
void BME280_read_calib_data(struct bme280_calib_data*);
void BME280_print(struct bme280_sensor_data*);

void BME280_compensate_T(struct bme280_sensor_data*, struct bme280_calib_data*, struct bme280_sensor_data*);
void BME280_compensate_P(struct bme280_sensor_data*, struct bme280_calib_data*, struct bme280_sensor_data*);
void BME280_compensate_H(struct bme280_sensor_data*, struct bme280_calib_data*, struct bme280_sensor_data*);
void check_HAL_status(HAL_StatusTypeDef ret, uint8_t address);



struct bme280_calib_data
{
	double dig_T1;
	double dig_T2;
	double dig_T3;
	double dig_P1;
	double dig_P2;
	double dig_P3;
	double dig_P4;
	double dig_P5;
	double dig_P6;
	double dig_P7;
	double dig_P8;
    double dig_P9;
    double dig_H1;
    double dig_H2;
    double dig_H3;
    double dig_H4;
    double dig_H5;
    double dig_H6;

    int32_t t_fine;
};

struct bme280_settings
{
    uint8_t osr_p;
    uint8_t osr_t;
    uint8_t osr_h;
    uint8_t filter;
    uint8_t standby_time;
    uint8_t mode;
};

struct bme280_sensor_data
{
	double temperature;
	double pressure;
	double humidity;
};


struct bme280_dev
{
    uint8_t chip_id;
    struct bme280_calib_data calib_data;
    struct bme280_settings settings;
    struct bme280_sensor_data sensor_data;
    struct bme280_sensor_data compensated_sensor_data;

};


#endif /* INC_BME280_HPP_ */
