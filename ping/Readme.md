# Pico W FreeRTOS Wi-Fi Ping Example

This project demonstrates how to connect a Raspberry Pi Pico W to a Wi-Fi network using FreeRTOS (SMP) and perform ICMP ping requests using lwIP — all in pure C, without Arduino or MicroPython layers.

## ✅ Features

- Runs FreeRTOS SMP on both cores of the Pico W
- Connects to a Wi-Fi network using the CYW43 driver
- Sends ICMP echo requests (ping) using lwIP's raw API
- Logs ping results with round-trip time (RTT)
- Fully configurable ping interval and target address