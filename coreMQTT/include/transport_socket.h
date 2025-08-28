#ifndef TRANSPORT_SOCKET_H
#define TRANSPORT_SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include "coremqtt_client.h"

typedef struct {
    int socket;
} TransportSocket_t;

typedef struct NetworkContext {
    TransportSocket_t *socket;
} NetworkContext_t;

int32_t Transport_Connect(TransportSocket_t *transport, MQTTConfig_t *config);
int32_t Transport_Disconnect(TransportSocket_t *transport);
int32_t Transport_Send(NetworkContext_t *context, const void *data, size_t size);
int32_t Transport_Recv(NetworkContext_t *context, void *buffer, size_t size);

#endif // TRANSPORT_SOCKET_H