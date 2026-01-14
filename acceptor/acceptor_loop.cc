/******************************************************************************/
/*! @brief      acceptor event loop
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

static void* _tcpsrv_acceptor_thread(void* arg){
    auto ctx = (context_ptr)arg;
    //
    while(!__halt){
        auto r = event_base_loop(ctx->event_base, EVLOOP_ONCE);
        if (r==0){
            // noop
        }else if (r < 0){
            LOG(LOG_ERR, "failed.event_base_loop-tcp(%p:%d)", ctx->event_base, r);
        }else{
            usleep(10);
        }
    }
    return(NULL);
}

static void* _udssrv_acceptor_thread(void* arg){
    auto ctx = (context_ptr)arg;
    //
    while(!__halt){
        auto r = event_base_loop(ctx->event_base_uds, EVLOOP_ONCE);
        if (r==0){
            // noop
        }else if (r < 0){
            LOG(LOG_ERR, "failed.event_base_loop-uds(%p:%d)", ctx->event_base_uds, r);
        }else{
            usleep(10);
        }
    }
    return(NULL);
}

void acceptor_loop(context_ptr ctx){
    if (pthread_create(&ctx->tcpsrv_thread, NULL, _tcpsrv_acceptor_thread, ctx)) {
        throw std::runtime_error("pthread_create..(run tcp server accept loop.)");
    }
    if (pthread_create(&ctx->udssrv_thread, NULL, _udssrv_acceptor_thread, ctx)) {
        throw std::runtime_error("pthread_create..(run unix domain server accept loop.)");
    }
}
