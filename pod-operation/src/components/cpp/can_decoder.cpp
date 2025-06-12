#include "can_decoder.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

// Anonymous namespace for helper functions local to this file
namespace {
    // Helper to convert a two-character hex string to a byte
    uint8_t hex_to_byte(const std::string& hex) {
        return static_cast<uint8_t>(std::stoul(hex, nullptr, 16));
    }
}

std::optional<std::pair<uint32_t, DecodedMessage>> CANDecoder::decode(const std::string& candump_line) {
    std::stringstream ss(candump_line);
    std::string interface, id_str, data_str;
    char bracket;

    // Example candump line: (1627993993.434858) can0 10F8109A#0000B80B00000000
    // We parse out the ID and the data payload
    ss >> bracket >> interface >> bracket >> interface >> id_str;

    // Find the data part after the '#'
    size_t hash_pos = candump_line.find('#');
    if (hash_pos == std::string::npos) {
        return std::nullopt; // Not a valid data frame line
    }
    data_str = candump_line.substr(hash_pos + 1);

    // Parse the CAN ID
    uint32_t id;
    try {
        id = std::stoul(id_str, nullptr, 16);
    } catch (const std::invalid_argument& e) {
        return std::nullopt; // Invalid ID format
    }

    // Parse the data bytes
    if (data_str.length() % 2 != 0) {
        return std::nullopt; // Data must be in whole bytes
    }
    std::vector<uint8_t> data;
    data.reserve(data_str.length() / 2);
    for (size_t i = 0; i < data_str.length(); i += 2) {
        data.push_back(hex_to_byte(data_str.substr(i, 2)));
    }

    if (data.size() != 8) {
       return std::nullopt; // Kelly controller always sends 8-byte frames
    }

    // Decode based on the CAN ID from the manual
    DecodedMessage message;
    if (id == 0x10F8109A) {
        message.state = decode_controller_state(data);
        return std::make_pair(id, message);
    } else if (id == 0x10F8108D) {
        message.telemetry = decode_controller_telemetry(data);
        return std::make_pair(id, message);
    }

    return std::nullopt; // Return empty if the ID is not recognized
}

ControllerState CANDecoder::decode_controller_state(const std::vector<uint8_t>& data) {
    ControllerState state;

    // Byte 1 (index 0): Driving direction (bits 0-1)
    uint8_t direction_raw = data[0] & 0x03; // Mask for the first two bits
    if (direction_raw == 0x00) state.direction = DrivingDirection::Neutral;
    else if (direction_raw == 0x01) state.direction = DrivingDirection::Forward;
    else if (direction_raw == 0x02) state.direction = DrivingDirection::Reverse;
    else state.direction = DrivingDirection::Unknown;

    // Bytes 2 & 3 (indices 1, 2): Motor RPM (little-endian)
    state.motor_rpm = data[1] | (data[2] << 8);

    // Byte 4 (index 3): Error code
    state.error_code = data[3];

    return state;
}

ControllerTelemtry CANDecoder::decode_controller_telemetry(const std::vector<uint8_t>& data) {
    ControllerTelemtry telemetry;

    // Bytes 1 & 2 (indices 0, 1): Battery Voltage (0.1V/bit)
    telemetry.battery_voltage = (data[0] | (data[1] << 8)) * 0.1f;

    // Bytes 3 & 4 (indices 2, 3): Motor Current (0.1A/bit)
    telemetry.motor_current = (data[2] | (data[3] << 8)) * 0.1f;

    // Bytes 5 & 6 (indices 4, 5): Motor Temp (0.1°C/bit)
    telemetry.motor_temp_c = (data[4] | (data[5] << 8)) * 0.1f;

    // Bytes 7 & 8 (indices 6, 7): Controller Temp (0.1°C/bit)
    telemetry.controller_temp_c = (data[6] | (data[7] << 8)) * 0.1f;

    return telemetry;
}
