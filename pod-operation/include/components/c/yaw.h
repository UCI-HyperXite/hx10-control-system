#ifndef _TOFLIB_H_
#define _TOFLIB_H_

//
// Read the model and revision of the
// tof sensor
//
int tofGetModel(int *model, int *revision);

//
// Read the current distance in mm
//
int tofReadDistance(void);

//
// Opens a file system handle to the I2C device
// sets the device continous capture mode
//
int tofInit(int iChan, int iAddr, int bLongRange);

#endif // _TOFLIB_H
