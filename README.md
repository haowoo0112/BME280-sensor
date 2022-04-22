# BME280-sensor

## Introduction
can use this package in stm32

## File information
- BME280.hpp : This header file contains the declarations of the sensor driver APIs.
- BME280.cpp : This source file contains the definitions of the sensor driver APIs.

## Example
```c=
#include "BME280.hpp"

bme280_dev BME280;

BME280_init(&BME280);

BME280_read(&BME280);

```