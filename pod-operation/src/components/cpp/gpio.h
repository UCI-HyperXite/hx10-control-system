#ifndef GPIO_H
#define GPIO_H

typedef enum{
    I2C_SDA = 2,
    I2C_SCL = 3,
    VL53L0X_FRONT = 5,
    VL53L0X_BACK = 6,
    WHEEL_ENCODER_A = 14,
    WHEEL_ENCODER_B = 15,
    VL6180X = 16,
    CONTACTOR_RELAY = 19,
    SIGNAL_LIGHT_RELAY = 18,
    PNEUMATICS_RELAY = 20,
} GPIOPins;

void initailizeGPIOs(void);
void setHigh(int pin);
void setLow(int pin);

#endif