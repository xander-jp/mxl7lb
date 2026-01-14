/******************************************************************************/
/*! @brief      acceptor common definitions
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef __ACCEPTOR_INCLUDE__
#define __ACCEPTOR_INCLUDE__

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <err.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <event2/event.h>
#include <event2/thread.h>
#ifdef __linux__
#include "queue.h"
#include <errno.h>
#else
#include <sys/queue.h>
#endif
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

// stl.
#include <stdexcept>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <map>



#ifndef MIN
#define MIN(a,b) (a>b?b:a)              /*!< min */
#endif

#ifndef SOL_TCP
#define SOL_TCP (IPPROTO_TCP)
#endif


#define LOG(l, ...)  _LOG(l, __func__, __VA_ARGS__, "dummy")
#define _LOG(l, func, format, ...) __LOG(l, func, format "%.0s", __VA_ARGS__)
static inline void __LOG(const int level, const char *funcname , const char *format, ...){
    char funcnm_bf[20] = {0};
    char msg_bf[512] = {0};
    memcpy(funcnm_bf, funcname, MIN(strlen(funcname), sizeof(funcnm_bf)-1));
    va_list ap;
    va_start(ap, format);
    vsnprintf(msg_bf, sizeof(msg_bf)-1,format, ap);
    syslog(level,"[  %20s]%s", funcnm_bf, msg_bf);
    va_end(ap);
}


typedef struct client {
    TAILQ_ENTRY(client) link;
    pthread_t           thread;
    int                 sock;
    char                remote[32];
    unsigned            port;
    struct context*     ctx;
    struct event*       client_event;
    char                proxy_packet[64];
}client_t,*client_ptr;


typedef struct context{
    char        bind_ip_addr[64];
    char        unix_domain_sock[64];
    unsigned    listen_port;
    unsigned    verbose;
    unsigned    debug_acceptor;
    //
    struct event_base*  event_base;
    struct event*       recv_event;
    struct event*       timeout_event;
    int         accept_sock;
    struct sockaddr_in  addr;
    unsigned    addrlen;

    struct event_base*  event_base_uds;
    struct event*       recv_event_uds;
    struct event*       timeout_event_uds;
    int         domain_sock;
    //
    pthread_t   tcpsrv_thread;
    pthread_t   udssrv_thread;
    //
    client_ptr                  roundrobin_client;
    pthread_mutex_t             uds_clients_mutex;
    TAILQ_HEAD(uds_clients, client) uds_clients;

    pthread_mutex_t             tcp_clients_mutex;
    TAILQ_HEAD(tcp_clients, client) tcp_clients;

    void*       acceptor_shm;
    pthread_mutex_t available_address_mutex;
    void*       available_address;
}context_t,*context_ptr;


typedef struct proxy_header{
    unsigned char sig[12];
    unsigned char ver_cmd;
    unsigned char fam;
    unsigned short len;
}proxy_header_t,*proxy_header_ptr;

extern unsigned __halt;

extern void usage(void);
extern void acceptor_initialize(int argc, char *argv[], context_ptr);
extern void acceptor_setup_udssrv(context_ptr);
extern void acceptor_setup_tcpsrv(context_ptr);
extern void acceptor_handler_tcpsrv(evutil_socket_t, short, void*);
extern void acceptor_handler_udssrv(evutil_socket_t, short, void*);
extern void acceptor_loop(context_ptr);
extern void acceptor_timeout_tcpsrv(evutil_socket_t, short, void *);
extern void acceptor_timeout_udssrv(evutil_socket_t, short, void *);
extern int acceptor_sendfd(int , const int*);
extern int acceptor_recieve_req(int , char*);



#endif /* __ACCEPTOR_INCLUDE__ */
