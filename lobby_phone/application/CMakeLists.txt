cmake_minimum_required(VERSION 3.15.2)
project(core)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(CURL REQUIRED)

link_directories("libs")

include_directories(
    include
    ../lib/ioctl
    ../lib/rapidjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/smart_home
    ${PAHO_MQTT_CPP_INCLUDE_DIRS}
    #${CURL_INCLUDE_DIR}
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/oled.cpp
    src/keyPad.cpp
    src/systemManager.cpp
    src/buzzer.cpp
    src/mqtt_iface.cpp
    src/event_handler.cpp
    src/event_driven.cpp
    src/image_sender.cpp
    src/config_manager.cpp
    src/utils.cpp)

target_link_libraries(
    ${PROJECT_NAME}
    OpenSSL::SSL
    OpenSSL::Crypto
    spdlog::spdlog
    PahoMqttCpp::paho-mqttpp3
    Boost::thread
    /usr/lib/arm-linux-gnueabihf/libev.so
    yaml-cpp
    ${OpenCV_LIBS}
    #   ${CURL_LIBRARIES}
)
