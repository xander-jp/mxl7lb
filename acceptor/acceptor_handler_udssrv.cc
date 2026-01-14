/******************************************************************************/
/*! @brief      Unix domain socket server event handler
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

typedef struct session_context{
    int         sock;
    pthread_t   thread;
    context_ptr ctx;
}session_context_t,*session_context_ptr;

static void* _udssrv_session(void* arg){
    auto ses = (session_context_ptr)arg;
    std::map<uint32_t, uint32_t> &available_address = *((std::map<uint32_t, uint32_t>*)ses->ctx->available_address);
    client_ptr  cur, curtmp;
    int peer_fd = 0;
    char request_code = 0;
    int sock_err = 0;
    socklen_t sockopt_len = 0;
    int is_exists = 0;
    unsigned address = 0;

    //
    while(!__halt){
        bzero(&peer_fd, sizeof(peer_fd));
        // wait for request.
        if (acceptor_recieve_req(ses->sock , &request_code) != 0){
            LOG(LOG_ERR, "acceptor_recieve_req . failed.(%d)", ses->sock);
            goto fin;
        }
        // delegate tcp sock to nginx.
        // if empty tcp client -> wait.
        // round robin dispatch.
        pthread_mutex_lock(&ses->ctx->tcp_clients_mutex);
        TAILQ_FOREACH_SAFE(cur, &ses->ctx->tcp_clients, link, curtmp) {
            sock_err = 0;
            sockopt_len = sizeof(sock_err);
            if (getsockopt(cur->sock, SOL_SOCKET, SO_ERROR, &sock_err, &sockopt_len) != 0){
                TAILQ_REMOVE(&ses->ctx->tcp_clients, cur, link);
                free(cur);
            }else if (sock_err != 0 && sock_err != EISCONN){
                TAILQ_REMOVE(&ses->ctx->tcp_clients, cur, link);
                free(cur);
            }else{
                is_exists = 1;
                address = (*((uint32_t*)(&cur->proxy_packet[sizeof(proxy_header_t)])));
                pthread_mutex_lock(&ses->ctx->available_address_mutex);
                if (available_address.find(address) == available_address.end()){
                    is_exists = 0;
                }
                pthread_mutex_unlock(&ses->ctx->available_address_mutex);
                //
                if (is_exists){
                    peer_fd = dup(cur->sock);
                    //LOG(LOG_INFO, "%p : %d: %s:%u --> %d", pthread_self(), cur->sock, cur->remote, cur->port, peer_fd);
                    auto p = (client_ptr)malloc(sizeof(client_t));
                    bzero(p, sizeof(client_t));
                    p->sock = cur->sock;
                    memcpy(p->remote, cur->remote, sizeof(p->remote));
                    p->port = cur->port;
                    p->ctx = cur->ctx;
                    p->client_event = cur->client_event;
                    memcpy(p->proxy_packet, cur->proxy_packet, sizeof(p->proxy_packet));
                    //
                    TAILQ_INSERT_TAIL(&ses->ctx->tcp_clients, p, link);
                    TAILQ_REMOVE(&ses->ctx->tcp_clients, cur, link);
                    free(cur);
                    if (peer_fd != 0){
                        break;
                    }
                }else{
                    shutdown(cur->sock, SHUT_RDWR);
                    close(cur->sock);
                    TAILQ_REMOVE(&ses->ctx->tcp_clients, cur, link);
                    free(cur);
                }
            }
        }
        pthread_mutex_unlock(&ses->ctx->tcp_clients_mutex);
        //
        if (peer_fd == 0){
            LOG(LOG_ERR, "empty fd. . failed.(%d)", peer_fd);
            goto fin;
        }
        if (acceptor_sendfd(ses->sock, &peer_fd) != 0){
            LOG(LOG_ERR, "acceptor_sendfd . failed.(%d : %d)", ses->sock, peer_fd);
            goto fin;
        }
//        LOG(LOG_INFO, "succeeded. acceptor_sendfd(%d:%d) .", ses->sock, peer_cnt);
        close(peer_fd);
    }
fin:
    shutdown(ses->sock, SHUT_RDWR);
    close(ses->sock);
    free(ses);
    pthread_detach(pthread_self());
    return(NULL);
}


void acceptor_handler_udssrv(evutil_socket_t sock, short what, void *arg){
    auto ctx = (context_ptr)arg;
    struct sockaddr_in addr;
    int one = 1;
    char rmt[32]={0};
    pthread_t   thread;
    socklen_t addrlen = sizeof(addr);
    //
    auto client = accept(sock, (struct sockaddr*)&addr, &addrlen);
    if (client>0){
        LOG(LOG_INFO, "unix domain socket connected .");

        auto p = (session_context_ptr)malloc(sizeof(session_context_t));
        p->sock = client;
        p->ctx = ctx;
        //
        if (pthread_create(&p->thread, NULL, _udssrv_session, p)) {
            throw std::runtime_error("pthread_create..(run unix domain socket server client loop.)");
        }
    }
}
