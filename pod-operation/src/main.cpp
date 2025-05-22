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
	std::future<void> pressure_downstream = std::async(std::launch::async, readPneumaticPressure, &pneumatic_downstream);

	PressureTransducer pneumatic_upstream(0.1f, 3.2f, 0x41, Reference::upstream());
	std::future<void> pressure_upstream = std::async(std::launch::async, readPneumaticPressure, &pneumatic_upstream);

	PressureTransducer left_coolant_manifold(0.1f, 3.2f, 0x44, Reference::left_Manifold());
	std::future<void> pressure_left_coolant = std::async(std::launch::async, readCoolantPressure, &left_coolant_manifold);

	PressureTransducer right_coolant_manifold(0.1f, 3.2f, 0x45, Reference::right_Manifold());
	std::future<void> pressure_right_coolant = std::async(std::launch::async, readCoolantPressure, &right_coolant_manifold);

	Adafruit_ADS1115 ads(0x4B);
	std::future<void> ads_temp = std::async(std::launch::async, readADS1015ThermistorLoop, &ads);

	orientation.wait();
	height.wait();
	pressure_downstream.wait();
	pressure_upstream.wait();
	pressure_left_coolant.wait();
	pressure_right_coolant.wait();

	return 0;
}