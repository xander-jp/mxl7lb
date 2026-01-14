/******************************************************************************/
/*! @brief      TCP server setup
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"


void acceptor_setup_tcpsrv(context_ptr ctx){
    int on = 1;

    if ((ctx->accept_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        throw std::runtime_error("socket..");
    }
    if (setsockopt(ctx->accept_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
        throw std::runtime_error("SO_REUSEADDR..");
    }
    ctx->addr.sin_family    = AF_INET;
    ctx->addr.sin_port      = htons(ctx->listen_port);
    inet_pton(AF_INET, ctx->bind_ip_addr ,&ctx->addr.sin_addr.s_addr);
    //
    if (bind(ctx->accept_sock, (struct sockaddr*)&ctx->addr, sizeof(ctx->addr))) {
        throw std::runtime_error("bind..");
    }
    auto flags = fcntl(ctx->accept_sock,F_GETFL,0);
    if (flags < 0){
        throw std::runtime_error("fcntl(F_GETFL)..");
    }
    fcntl(ctx->accept_sock,F_SETFL,flags|O_NONBLOCK);
    on = 1;
    if (setsockopt(ctx->accept_sock, SOL_TCP, TCP_NODELAY, &on, sizeof(on)) < 0){
        throw std::runtime_error("nagle off..");
    }
    on = (1024*1024*1);
    if (setsockopt(ctx->accept_sock, SOL_SOCKET, SO_RCVBUF, &on, sizeof(on)) < 0){
        throw std::runtime_error("recieve buffer size..");
    }
    on = (1024*1024*1);
    if (setsockopt(ctx->accept_sock, SOL_SOCKET, SO_SNDBUF, &on, sizeof(on)) < 0){
        throw std::runtime_error("send buffer size..");
    }
    if (listen(ctx->accept_sock, 1024) < 0){
        throw std::runtime_error("listen..");
    }
    //
    if (!(ctx->event_base = event_base_new())){
        throw std::runtime_error("malloc event base..");
    }
    ctx->recv_event     = event_new(ctx->event_base, ctx->accept_sock, EV_READ|EV_PERSIST, acceptor_handler_tcpsrv, ctx);
    ctx->timeout_event  = event_new(ctx->event_base, -1, EV_TIMEOUT|EV_PERSIST, acceptor_timeout_tcpsrv, ctx);

    struct timeval now; // interrupt for halt.
    now.tv_sec  = 1;
    now.tv_usec = 0;
    //
    if (!ctx->recv_event || !ctx->timeout_event){
        throw std::runtime_error("malloc event..");
    }
    if (event_add(ctx->recv_event, NULL) || event_add(ctx->timeout_event, &now)) {
        throw std::runtime_error("event_add..");
    }
    LOG(LOG_INFO, "done tcp server setup.");
}

