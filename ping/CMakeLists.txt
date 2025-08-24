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

project(ping C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(PING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
file(MAKE_DIRECTORY ${PING_DIR})

if(NOT EXISTS ${PING_DIR}/ping.c)
    file(DOWNLOAD
    https://raw.githubusercontent.com/particle-iot/lwip-contrib/master/apps/ping/ping.c
    ${CMAKE_CURRENT_SOURCE_DIR}/external/ping.c
    )
endif()

if(NOT EXISTS ${PING_DIR}/ping.h)
    file(DOWNLOAD
    https://raw.githubusercontent.com/particle-iot/lwip-contrib/master/apps/ping/ping.h
    ${CMAKE_CURRENT_SOURCE_DIR}/external/ping.h
    )
endif()

# Add executable. Default name is the project name, version 0.1
add_executable(ping main.c
                    external/ping.c
)

pico_set_program_name(ping "ping")
pico_set_program_version(ping "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ping 1)
pico_enable_stdio_usb(ping 0)

target_compile_definitions(ping PRIVATE
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

# Add libraries to the build
target_link_libraries(ping
        pico_cyw43_arch_lwip_sys_freertos
        FreeRTOS-Kernel-Heap4
        pico_stdlib
        )

# Add the standard include files to the build
target_include_directories(ping PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/external
        ${CMAKE_CURRENT_LIST_DIR}/..
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(ping)