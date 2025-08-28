#include "transport_socket.h"
#include "lwip/sockets.h"
#include <string.h>
#include <stdio.h>

int32_t Transport_Connect(TransportSocket_t *transport, MQTTConfig_t *config) {
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config->broker_port);
    serverAddr.sin_addr.s_addr = inet_addr(config->broker_ip);  // Of gebruik DNS-resolutie indien nodig

    int sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Transport_Connect: socket failed\n");
        return -1;
    }

    if (lwip_connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Transport_Connect: connect failed\n");
        lwip_close(sock);
        return -1;
    }

    transport->socket = sock;
    return 0;
}

int32_t Transport_Disconnect(TransportSocket_t *transport) {
    if (transport->socket >= 0) {
        lwip_close(transport->socket);
        transport->socket = -1;
    }
    return 0;
}

int32_t Transport_Recv(NetworkContext_t *context, void *buffer, size_t size) {
    int ret = lwip_recv(context->socket->socket, buffer, size, 0);
    if (ret > 0) {
        return ret;
    } else if (ret == 0) {
        // Verbinding gesloten
        return -1;
    } else {
        // Fout
        return -1;
    }
}

int32_t Transport_Send(NetworkContext_t *context, const void *data, size_t size) {
    int ret = lwip_send(context->socket->socket, data, size, 0);
    if (ret >= 0) {
        return ret;
    } else {
        return -1;
    }
}
