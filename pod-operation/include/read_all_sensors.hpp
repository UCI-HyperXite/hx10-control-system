#ifndef READ_ALL_SENSORS_HPP
#define READ_ALL_SENSORS_HPP

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "initialize_sensors.hpp"

boost::json::object read_all_sensors(boost::asio::io_context& io_context, SensorInitializer& sensors);

#endif // READ_ALL_SENSORS_HPP

