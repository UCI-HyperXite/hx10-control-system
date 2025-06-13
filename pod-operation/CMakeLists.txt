cmake_minimum_required(VERSION 3.15)
project(PodOperation LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(pod_c_sources
    src/components/c/brakes.c
    src/components/c/high_voltage_system.c
    src/components/c/pod_height.c
    src/components/c/yaw.c
    src/utils/gpio.c
)

add_library(pod_c STATIC ${pod_c_sources})
set_target_properties(pod_c PROPERTIES LINKER_LANGUAGE C)

set(pod_operation_sources
    src/components/cpp/Adafruit_ADS1015.cpp
    src/components/cpp/current_sensor.cpp
    src/components/cpp/gyro.cpp
    src/components/cpp/inverter.cpp
    src/components/cpp/pressure_transducer.cc
    src/components/cpp/signal_light.cpp
    src/components/cpp/voltage.cpp
    src/components/cpp/wheel_encoder.cpp
    

    src/components.cpp
    src/demo.cpp
    src/main.cpp
    src/state_machine.cpp
    src/utils/mock.cpp
)

add_executable(pod_operation ${pod_operation_sources})
target_link_libraries(pod_operation
    pod_c
    pthread
    m
    wiringPi
    i2c
)