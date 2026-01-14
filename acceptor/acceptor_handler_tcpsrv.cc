/******************************************************************************/
/*! @brief      TCP server event handler
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

static client_ptr  cur;

static void acceptor_handler_tcpsrv_proxy_recv(evutil_socket_t sock, short what, void *arg);

void acceptor_handler_tcpsrv(evutil_socket_t sock, short what, void *arg){
    auto ctx = (context_ptr)arg;
    struct sockaddr_in addr;
    int one = 1;
    socklen_t addrlen = sizeof(addr);
    client_ptr  cur,curtmp;
    char rmt[32]={0};
    //
    auto client = accept(sock, (struct sockaddr*)&addr, &addrlen);
    if (client>0){
        auto flags = fcntl(client,F_GETFL,0);
        if (flags < 0){
            throw std::runtime_error("fcntl(F_GETFL)..");
        }
        fcntl(client,F_SETFL,flags^O_NONBLOCK);
        if (setsockopt(client, SOL_TCP, TCP_NODELAY, &one, sizeof(one)) < 0){
            throw std::runtime_error("nagle off..");
        }
        one = (1024*1024*1);
        if (setsockopt(client, SOL_SOCKET, SO_RCVBUF, &one, sizeof(one)) < 0){
            throw std::runtime_error("recieve buffer size..");
        }
        one = (1024*1024*1);
        if (setsockopt(client, SOL_SOCKET, SO_SNDBUF, &one, sizeof(one)) < 0){
            throw std::runtime_error("send buffer size..");
        }
        inet_ntop(AF_INET, &addr.sin_addr.s_addr, rmt, sizeof(rmt));
        LOG(LOG_INFO, "tcp connected from -> pre read. %s:%u", rmt, addr.sin_port);

        auto p = (client_ptr)malloc(sizeof(client_t));
        auto clievent = event_new(ctx->event_base, client, EV_READ|EV_PERSIST, acceptor_handler_tcpsrv_proxy_recv, p);
        //
        bzero(p, sizeof(client_t));
        p->sock = client;
        memcpy(p->remote, rmt, sizeof(p->remote));
        p->port = addr.sin_port;
        p->ctx = ctx;
        p->client_event = clievent;
        //
        if (clievent) {
            event_add(clievent, NULL);
        }else{
            throw std::runtime_error("event_new..");
        }
    }
}
void acceptor_handler_tcpsrv_proxy_recv(evutil_socket_t client, short what, void *arg){
    auto pcli = (client_ptr)arg;
    int recvlen;
    proxy_header_ptr  proxy_head;
    //
    if ((what & EV_READ) && pcli) {
        proxy_head = (proxy_header_ptr)pcli->proxy_packet;
        // handshake on proxy protocol.
        if ((recvlen = recv(client, proxy_head, sizeof(*proxy_head), 0)) <= 0) {
            LOG(LOG_INFO, "failed proxy protocol read.");
            goto cleanup;
        }
        if (ntohs(proxy_head->len) != 6 && ntohs(proxy_head->len) != 18){
            LOG(LOG_INFO, "invalid length(proxy protocol - header)");
            goto cleanup;
        }
        if ((recvlen = recv(client, &pcli->proxy_packet[sizeof(*proxy_head)], ntohs(proxy_head->len), 0)) <= 0) {
            LOG(LOG_INFO, "failed proxy protocol read. address.");
            goto cleanup;
        }
        LOG(LOG_INFO, " -> distribute to nginx. %s:%u", pcli->remote, pcli->port);
        event_del(pcli->client_event);

        // enqueue
        pthread_mutex_lock(&pcli->ctx->tcp_clients_mutex);
        TAILQ_INSERT_TAIL(&pcli->ctx->tcp_clients, pcli, link);
        pthread_mutex_unlock(&pcli->ctx->tcp_clients_mutex);
        return;
    }
cleanup:
    LOG(LOG_INFO, "cleanup %s:%u", pcli->remote, pcli->port);
    shutdown(client, SHUT_RDWR);
    close(client);
    event_del(pcli->client_event);
    free(pcli);
}
