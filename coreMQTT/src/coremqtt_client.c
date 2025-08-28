#include <stdio.h>
#include <string.h>
#include "coremqtt_client.h"
#include "core_mqtt.h"
#include "FreeRTOS.h"
#include "task.h"

#define KEEP_ALIVE_SECONDS      60
#define RECONNECT_DELAY_MS      2000

uint32_t getTimeMs(void) {
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

void vKeepAliveTask(void *pvParameters) {
    
    uint16_t keepAliveMs = KEEP_ALIVE_SECONDS * 1000;
   
    while (1) {
        uint32_t now = getTimeMs();
        if ((now - mqttContext.lastPacketTxTime) >= keepAliveMs) {
            MQTTStatus_t MQTTStatus = MQTT_Ping(&mqttContext);
            if (MQTTStatus != MQTTSuccess) {
                printf("Fout bij vesturen PINGREQ: %d\n", MQTTStatus);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Controleer elke seconde
    }
}

MQTTStatus_t connectToMQTTBroker(MQTTConfig_t *config) {
    MQTTConnectInfo_t connectInfo = {
        .cleanSession = true,
        .keepAliveSeconds = 60, 
        .pClientIdentifier = config->client_id,
        .clientIdentifierLength = strlen(config->client_id)
    };

    if(config->broker_port == 1884) {
        connectInfo.pPassword = config->username;
        connectInfo.userNameLength = strlen(config->username);
        connectInfo.pPassword = config->password;
        connectInfo.passwordLength= strlen(config->password);
    }

    bool sessionPresent = false;
    MQTTStatus_t status = MQTT_Connect(&mqttContext, &connectInfo, NULL, 5000, &sessionPresent);
    if (status != MQTTSuccess) {
        printf("MQTT_Connect failed: %d\n", status);
    }
    return status;
}

void subscribeToTopic(const char *topic) {
    if (topic == NULL || strlen(topic) == 0) {
        printf("Fout: Ongeldig topic!\n");
        return;
    }

    MQTTSubscribeInfo_t subscribeInfo[1] = {{
        .qos = MQTTQoS0,
        .pTopicFilter = topic,
        .topicFilterLength = (uint16_t)strlen(topic)
    }};

    uint16_t packetId = MQTT_GetPacketId(&mqttContext);
    if (packetId == 0) {
        printf("Fout: Geen beschikbaar packet ID. %d\n", packetId);
        return;
    }

    MQTTStatus_t status = MQTT_Subscribe(&mqttContext, subscribeInfo, 1, packetId);   
    if (status != MQTTSuccess) {
        printf("Fout bij abonneren op '%s': %d\n", topic, status);
    } else {
        printf("Succesvol geabonneerd op '%s' (packet ID: %d)\n", topic, packetId);
    }
}