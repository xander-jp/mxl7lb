/******************************************************************************/
/*! @brief      Unix domain socket server setup
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"


void acceptor_setup_udssrv(context_ptr ctx){
    int on = 1;
    struct sockaddr_un address;
    int socket_fd, connection_fd;
    socklen_t address_length  = sizeof(address);

    ctx->domain_sock = socket(PF_UNIX, SOCK_STREAM, 0);
    if(ctx->domain_sock < 0) {
        throw std::runtime_error("socket..(unix domain socket)");
    }
    if (setsockopt(ctx->domain_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
        throw std::runtime_error("SO_REUSEADDR..");
    }
    unlink(ctx->unix_domain_sock);
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,sizeof(address.sun_path)-1,"%s", ctx->unix_domain_sock);

    if (bind(ctx->domain_sock, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) != 0) {
        throw std::runtime_error("bind..");
    }
    if (listen(ctx->domain_sock, 16) != 0) {
        throw std::runtime_error("listen..");
    }
    //
    if (!(ctx->event_base_uds = event_base_new())){
        throw std::runtime_error("malloc event base..");
    }
    ctx->recv_event_uds     = event_new(ctx->event_base_uds, ctx->domain_sock, EV_READ|EV_PERSIST, acceptor_handler_udssrv, ctx);
    ctx->timeout_event_uds  = event_new(ctx->event_base_uds, -1, EV_TIMEOUT|EV_PERSIST, acceptor_timeout_udssrv, ctx);

    struct timeval now; // interrupt for halt.
    now.tv_sec  = 3;
    now.tv_usec = 0;
    //
    if (!ctx->recv_event_uds || !ctx->timeout_event_uds){
        throw std::runtime_error("malloc event..");
    }
    if (event_add(ctx->recv_event_uds, NULL) || event_add(ctx->timeout_event_uds, &now)) {
        throw std::runtime_error("event_add..");
    }
    LOG(LOG_INFO, "done unix domain socket - server setup.");
}

