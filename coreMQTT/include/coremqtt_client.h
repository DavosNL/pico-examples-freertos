#ifndef COREMQTT_CLIENT_H
#define COREMQTT_CLIENT_H
#include <stdbool.h>
#include "core_mqtt.h"

extern MQTTContext_t mqttContext;
extern uint8_t mqttBuffer[];

uint32_t getTimeMs(void);
void vKeepAliveTask(void *pvParameters);

typedef struct {
    const char *broker_url;
    uint16_t broker_port;
    const char *broker_ip;
    const char *client_id;
    const char *username;
    const char *password;
    const char *topic;
    const char *payload;
} MQTTConfig_t;


MQTTStatus_t connectToMQTTBroker(MQTTConfig_t *config);
void subscribeToTopic(const char *topic);

#endif
