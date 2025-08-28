# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(coreMQTT C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# coreMQTT setup
set(COREMQTT_PATH "${USERHOME}/.lib/coreMQTT")
set(COREMQTT_CMAKE_DIR ${COREMQTT_PATH})
include("${COREMQTT_CMAKE_DIR}/mqttFilePaths.cmake")

file(TO_CMAKE_PATH "${COREMQTT_PATH}/source/" COREMQTT_SOURCES_PATH)
file(GLOB COREMQTT_SOURCES "${COREMQTT_SOURCES_PATH}/*.c")

# Add executable. Default name is the project name, version 0.1
add_executable(coreMQTT coreMQTT.c 
        src/transport_socket.c
        src/coremqtt_client.c
        ${COREMQTT_SOURCES}
)

pico_set_program_name(coreMQTT "coreMQTT")
pico_set_program_version(coreMQTT "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(coreMQTT 1)
pico_enable_stdio_usb(coreMQTT 0)

target_compile_definitions(coreMQTT PRIVATE
    PICO_DEFAULT_UART_RX_PIN=17
    PICO_DEFAULT_UART_TX_PIN=16
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
    NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)
    LWIP_SOCKET=1       # we need the socket API (generally this would be in your lwipopts.h)
    PING_USE_SOCKETS=1
)

#FreeRTOS setup
set(FREERTOS_KERNEL_PATH "${USERHOME}/.lib/FreeRTOS-Kernel")
set(FREERTOS_PORT_PATH "${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040")
include_directories( "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel")
add_subdirectory(${FREERTOS_PORT_PATH} freertos_port)



# Add the standard library to the build
target_link_libraries(coreMQTT
        pico_cyw43_arch_lwip_sys_freertos
        FreeRTOS-Kernel-Heap4
        pico_stdlib
)

# Add the standard include files to the build
target_include_directories(coreMQTT PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include    
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${MQTT_INCLUDE_PUBLIC_DIRS}        
)

# Add any user requested libraries
target_sources(coreMQTT PRIVATE 
    ${MQTT_SOURCES}
        
)

pico_add_extra_outputs(coreMQTT)