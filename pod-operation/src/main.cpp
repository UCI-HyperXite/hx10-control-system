#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <linux/i2c.h>
#include "components.hpp"
#include <future>
#include <thread>


int main() {

	MPU6050 gyro(0x68, false);
	std::future<void> orientation = std::async(std::launch::async, readGyro, &gyro);

	vl6180 pod_height = vl6180_initialise(1);
	std::future<void> height = std::async(std::launch::async, readPodHeight, &pod_height);

	orientation.wait();
	height.wait();

	return 0;
}