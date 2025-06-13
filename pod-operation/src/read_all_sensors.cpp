#include "read_all_sensors.hpp"
#include <iostream>

namespace json = boost::json;

json::object read_all_sensors(boost::asio::io_context& io_context, SensorInitializer& sensors)
{
    using boost::asio::post;
    auto snapshot = std::make_shared<json::object>();

    post(io_context, [&]() {
        float ax, ay, az, gx, gy, gz;
        sensors.gyro->getAccel(&ax, &ay, &az);
        sensors.gyro->getGyro(&gx, &gy, &gz);
        (*snapshot)["ax"] = ax;
        (*snapshot)["ay"] = ay;
        (*snapshot)["az"] = az;
        (*snapshot)["gx"] = gx;
        (*snapshot)["gy"] = gy;
        (*snapshot)["gz"] = gz;
    });

    post(io_context, [&]() {
        (*snapshot)["pod_height_mm"] = get_distance(*sensors.pod_height) + 10;
    });

    post(io_context, [&]() {
        (*snapshot)["pressure_downstream"] = sensors.downstream->read_pressure_pneumatics();
    });

    post(io_context, [&]() {
        (*snapshot)["pressure_upstream"] = sensors.upstream->read_pressure_pneumatics();
    });

    post(io_context, [&]() {
        (*snapshot)["pressure_left_coolant"] = sensors.left_manifold->read_pressure_coolant();
    });

    post(io_context, [&]() {
        (*snapshot)["pressure_right_coolant"] = sensors.right_manifold->read_pressure_coolant();
    });

    post(io_context, [&]() {
        float v = readDifferentialVoltage(*sensors.ads1, 2, 3);
        float r = getThermistorResistance(v);
        float t = getThermistorTemperator(r);
        (*snapshot)["temp_ads1"] = t;
    });

    post(io_context, [&]() {
        float v = readDifferentialVoltage(*sensors.ads2, 2, 3);
        float r = getThermistorResistance(v);
        float t = getThermistorTemperator(r);
        (*snapshot)["temp_ads2"] = t;
    });

    post(io_context, [&]() {
        float v = readDifferentialVoltage(*sensors.ads3, 2, 3);
        float r = getThermistorResistance(v);
        float t = getThermistorTemperator(r);
        (*snapshot)["temp_ads3"] = t;
    });

    post(io_context, [&]() {
        float v = readDifferentialVoltage(*sensors.ads4, 2, 3);
        float r = getThermistorResistance(v);
        float t = getThermistorTemperator(r);
        (*snapshot)["temp_ads4"] = t;
    });

    post(io_context, [&]() {
        (*snapshot)["encoder_distance"] = sensors.encoder->get_distance();
        (*snapshot)["encoder_velocity"] = sensors.encoder->get_velocity();
    });

    io_context.restart();
    io_context.run();

    return *snapshot;
}

