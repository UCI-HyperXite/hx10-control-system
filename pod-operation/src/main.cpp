#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <linux/i2c.h>
#include <future>
#include <thread>

#include "../include/components.hpp"

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

	// Adafruit_ADS1115 ads1(0x48);
	// std::future<void> adsFut1 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads1);

	Adafruit_ADS1115 ads2(0x49);
	std::future<void> adsFut2 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads2);

	Adafruit_ADS1115 ads3(0x4A);
	std::future<void> adsFut3 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads3);

	Adafruit_ADS1115 ads4(0x4B);
	std::future<void> adsFut4 = std::async(std::launch::async, readADS1015ThermistorLoop, &ads4);

	orientation.wait();
	height.wait();
	pressure_downstream.wait();
	pressure_upstream.wait();
	pressure_left_coolant.wait();
	pressure_right_coolant.wait();
	// adsFut1.wait();
	adsFut2.wait();
	adsFut3.wait();
	adsFut4.wait();

	return 0;
}