#ifndef GPIO_H
#define GPIO_H

enum GPIOPins{
    I2C_SDA = 2,
    I2C_SCL = 3,
    VL53L0X_FRONT = 5,
    VL53L0X_BACK = 6,
    WHEEL_ENCODER_A = 14,
    WHEEL_ENCODER_B = 15,
    VL6180X = 16,
    CONTACTOR_RELAY = 20,
	SIGNAL_LIGHT_RELAY = 21,
	PNEUMATICS_RELAY = 26,
};

void initailizeGPIOs(void);

#endif