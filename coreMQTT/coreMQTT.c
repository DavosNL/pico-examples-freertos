#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#undef poll
#include "pico/async_context.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "pico/cyw43_arch.h"
#include "transport_socket.h"
#include "core_mqtt.h"
#include "coremqtt_client.h"
#include "FreeRTOS.h"
#include "task.h"

MQTTConfig_t mqttConfig = {
    .broker_url = "test.mosquitto.org",
    .broker_port = 1883,
    .client_id = "pico_client",
    .username = "rw",
    .password = "readwrite",
    .topic = "pico/test-topic",
    .payload = "Hallo vanaf Pico W!"
};

MQTTContext_t mqttContext;
uint8_t mqttBuffer[2048];

// Callback voor MQTT-events (bijv. inkomende berichten)
void mqttEventCallback(MQTTContext_t *pContext,
                       MQTTPacketInfo_t *pPacketInfo,
                       MQTTDeserializedInfo_t *pDeserializedInfo) {
    
    if (pPacketInfo->type == MQTT_PACKET_TYPE_PUBLISH && pDeserializedInfo != NULL) {
        MQTTPublishInfo_t *publishInfo = pDeserializedInfo->pPublishInfo;
        printf("Bericht ontvangen op '%.*s': %.*s\n",
               publishInfo->topicNameLength, publishInfo->pTopicName,
               publishInfo->payloadLength, publishInfo->pPayload);
    }
    if (pPacketInfo->type == MQTT_PACKET_TYPE_SUBACK) {
        printf("SUBACK ontvangen\n");
    }
}

void mqtt_task(void *params){

    // WiFi init, in FreeRTOS this has to be done in a task  
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    if(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD , CYW43_AUTH_WPA2_AES_PSK, 30000)){
        printf("WiFi connection failed\n");
        vTaskDelete(NULL);
    };    
    printf("Local IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

    // Retrieve the IP address for the specified broker URL.
    struct hostent *host_entry = gethostbyname(mqttConfig.broker_url);
    if (host_entry == NULL){
        printf("DNS lookup failed for %s\n", mqttConfig.broker_url);
        vTaskDelete(NULL);
    } 
    struct in_addr **addr_list = (struct in_addr **)host_entry->h_addr_list;
    mqttConfig.broker_ip = inet_ntoa(*addr_list[0]);

    TransportSocket_t transportSocket;
    NetworkContext_t networkContext = { .socket = &transportSocket};

    if (Transport_Connect(&transportSocket, &mqttConfig) != 0) {
        printf("Transport connect failed\n");
        vTaskDelete(NULL);
    }

    TransportInterface_t transport = {
        .recv = Transport_Recv,
        .send = Transport_Send,
        .pNetworkContext = &networkContext
    };

    MQTTFixedBuffer_t fixedBuffer = {
        .pBuffer = mqttBuffer,
        .size = sizeof(mqttBuffer)
    };

    memset(&mqttContext, 0, sizeof(MQTTContext_t));

    MQTTStatus_t status = MQTT_Init(&mqttContext, &transport, getTimeMs, mqttEventCallback, &fixedBuffer);
    if (status != MQTTSuccess) {
        printf("MQTT_Init failed: %d\n", status);
        vTaskDelete(NULL);
    }

    status = connectToMQTTBroker(&mqttConfig);
    if (status != MQTTSuccess) {
        printf("MQTT_Connect failed: %d\n", status);
        vTaskDelete(NULL);
    }

    subscribeToTopic(mqttConfig.topic);

    MQTTPublishInfo_t publishInfo = {
        .qos = MQTTQoS0,
        .pTopicName = mqttConfig.topic,
        .topicNameLength = strlen(mqttConfig.topic),
        .pPayload = mqttConfig.payload,
        .payloadLength = strlen(mqttConfig.payload),
        .retain = false,
        .dup = false
    };

    status = MQTT_Publish(&mqttContext, &publishInfo, MQTT_GetPacketId(&mqttContext));

    xTaskCreate(vKeepAliveTask, "KeepAliveTask", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    while(true) {
        MQTT_ProcessLoop(&mqttContext);
        vTaskDelay(pdMS_TO_TICKS(100));       
    }      
}

void vLaunch(void) {
    TaskHandle_t task;
    xTaskCreate(mqtt_task, "TestMainThread", 1024, NULL, 1, &task);

#if NO_SYS && configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    // (note we only do this in NO_SYS mode, because cyw43_arch_freertos
    // takes care of it otherwise)
    vTaskCoreAffinitySet(task, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main()
{
    stdio_init_all();
    
    /* Configure the hardware ready to run the demo. */
#if ( configNUMBER_OF_CORES == 2 )
    printf("Starting FreeRTOS %s SMP on both cores:\n", tskKERNEL_VERSION_NUMBER);
    vLaunch();
#elif ( RUN_FREERTOS_ON_CORE == 1 )
    printf("Starting FreeRTOS %s  on core 1:\n", tskKERNEL_VERSION_NUMBER);
    multicore_launch_core1(vLaunch);
    while (true);
#else
    printf("Starting FreeRTOS %s on core 0:\n", tskKERNEL_VERSION_NUMBER);
    vLaunch();
#endif

    return 0;
}