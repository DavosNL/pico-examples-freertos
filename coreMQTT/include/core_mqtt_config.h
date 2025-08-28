#ifndef CORE_MQTT_CONFIG_H
#define CORE_MQTT_CONFIG_H

// Logging (optioneel)
#define MQTT_ENABLE_DEBUG_LOGS    0

// Buffergroottes
#define MQTT_SEND_BUFFER_SIZE     1024
#define MQTT_RECV_BUFFER_SIZE     1024

// Keep-alive en tijdsinstellingen
#define MQTT_KEEP_ALIVE_INTERVAL_SECONDS    60
#define MQTT_STATE_CURSOR_MAX_SIZE          128

// QoS en features
#define MQTT_SUPPORT_QOS1                   1
#define MQTT_SUPPORT_QOS2                   0

// Retry instellingen
#define MQTT_RETRY_ENABLED                  0

// #define MQTT_USE_CUSTOM_CONFIG              1
// #define MQTT_RECV_VECTORED_IO 0
// #define MQTT_SEND_VECTORED_IO 0



#endif /* CORE_MQTT_CONFIG_H */