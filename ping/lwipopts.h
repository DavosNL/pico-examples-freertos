#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// Generally you would define your own explicit list of lwIP options
// (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html)
//
// This example uses a common include to avoid repetition
#include "lwipopts_examples_common.h"
#include <stdio.h>

#define PING_ADDR        "1.1.1.1"     // Target IP address
#define PING_DELAY       5000          // Delay between pings (ms)
#define PING_RCV_TIMEO   2000          // Timeout for reply (ms)
#define PING_DATA_SIZE   32            // Extra payload size
#define PING_DEBUG       LWIP_DBG_ON   // Enable ping debug output

#define LWIP_DEBUG 1
#define PING_DEBUG LWIP_DBG_ON
#define LWIP_PLATFORM_DIAG(x) do {printf x;} while(0)
#define PING_RESULT(ping_ok) \
    printf("Ping %s - RTT: %lu ms\n", (ping_ok) ? "OK" : "FAIL", (unsigned long)(sys_now() - ping_time))


#if !NO_SYS
#define TCPIP_THREAD_STACKSIZE 1024
#define DEFAULT_THREAD_STACKSIZE 1024
#define DEFAULT_RAW_RECVMBOX_SIZE 8
#define DEFAULT_UDP_RECVMBOX_SIZE 8
#define DEFAULT_TCP_RECVMBOX_SIZE 8
#define TCPIP_MBOX_SIZE 8
#define LWIP_TIMEVAL_PRIVATE 0

// not necessary, can be done either way
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1

#define LWIP_SO_RCVTIMEO 1
#endif

#endif
