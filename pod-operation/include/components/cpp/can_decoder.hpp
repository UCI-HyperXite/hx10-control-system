#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

// Represents the driving direction of the pod
enum class DrivingDirection {
    Neutral = 0,
    Forward = 1,
    Reverse = 2,
    Unknown
};

// Structure to hold the decoded data from CAN frame ID 0x10F8109A
struct ControllerState {
    DrivingDirection direction = DrivingDirection::Unknown;
    int motor_rpm = 0;
    int error_code = 0;
};

// Structure to hold the decoded data from CAN frame ID 0x10F8108D
struct ControllerTelemtry {
    float battery_voltage = 0.0f;
    float motor_current = 0.0f;
    float motor_temp_c = 0.0f;
    float controller_temp_c = 0.0f;
};

// A union to hold either of the two possible message types
union DecodedMessage {
    ControllerState state;
    ControllerTelemtry telemetry;
};

/**
 * @class CANDecoder
 * @brief Decodes raw CAN frames from a Kelly KHA controller.
 *
 * This class provides functionality to parse a string from the 'candump' utility
 * and decode it into structured data based on the Kelly KHA CAN protocol.
 */
class CANDecoder {
public:
    /**
     * @brief Parses a raw string line from the 'candump' utility.
     * @param candump_line A single line of output from candump.
     * @return An std::optional containing the decoded message if the ID is recognized,
     * otherwise an empty optional. The specific message type can be determined
     * by checking the returned can_id.
     */
    std::optional<std::pair<uint32_t, DecodedMessage>> decode(const std::string& candump_line);

private:
    /**
     * @brief Decodes the Controller State message (ID 0x10F8109A).
     * @param data The 8-byte data payload from the CAN frame.
     * @return A populated ControllerState struct.
     */
    ControllerState decode_controller_state(const std::vector<uint8_t>& data);

    /**
     * @brief Decodes the Controller Telemetry message (ID 0x10F8108D).
     * @param data The 8-byte data payload from the CAN frame.
     * @return A populated ControllerTelemtry struct.
     */
    ControllerTelemtry decode_controller_telemetry(const std::vector<uint8_t>& data);
};
