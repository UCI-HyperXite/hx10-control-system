#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <linux/i2c.h>
#include "include/components.hpp"
#include <future>
#include <thread>


int main() {

	MPU6050 gyro(0x68, false);
	std::future<void> orientation = std::async(std::launch::async, readGyro, &gyro);

	vl6180 pod_height = vl6180_initialise(1);
	std::future<void> height = std::async(std::launch::async, readPodHeight, &pod_height);

	PressureTransducer pneumatic_downstream(0.1f, 3.2f, 0x40, Reference::downstream());

	PressureTransducer pneumatic_upstream(0.1f, 3.2f, 0x41, Reference::upstream());

	PressureTransducer left_coolant_manifold(0.1f, 3.2f, 0x44, Reference::left_Manifold());

	PressureTransducer right_coolant_manifold(0.1f, 3.2f, 0x45, Reference::right_Manifold());

	orientation.wait();
	height.wait();

	return 0;
}