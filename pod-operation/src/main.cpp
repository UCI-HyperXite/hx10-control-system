#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <linux/i2c.h>
#include "include/components.h"



int main() {

	MPU6050 gyro(0x68, false);
	std::thread gyroThread(readGyro, &gyro);

	vl6180 pod_height = vl6180_initialise(1);
	std::thread podHeightThread(readPodHeight, &pod_height);

    // Keep main alive
	gyroThread.join();
    podHeightThread.join();

	return 0;
}