/******************************************************************************/
/*! @brief      acceptor main entry point
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"
#include "nat_shared_memory_config.hh"

unsigned __halt = 0;
static std::map<uint32_t, uint32_t> available_address;


void cleanTasks(int signo){
    __halt++;
    LOG(LOG_INFO, "cleanTasks(%u)\n", __halt);
}

void usage(void){
    printf("-i bind ip address\n");
    printf("-p listen port number\n");
    printf("-u unix domain socket name(ex. /tmp/Xaccepted_socket)\n");
    printf("-v verbose\n");
    printf("-h help\n");
    printf("-d debug acceptor\n");
}

int main(int argc, char *argv[]){
    context_t   ctx;

    LOG(LOG_INFO, "start[%s]", argv[0]);
    bzero(&ctx, sizeof(ctx));
    //
    acceptor_initialize(argc, argv, &ctx);
    //
    signal(SIGINT,		cleanTasks);
    signal(SIGTERM,		cleanTasks);
    signal(SIGHUP,		cleanTasks);

    openlog(argv[0], LOG_PERROR|LOG_PID,LOG_LOCAL2);
#ifdef	EVTHREAD_USE_PTHREADS_IMPLEMENTED
    evthread_use_pthreads();
#endif

    nat::SharedMemoryConfig config_shm("/ipc_acceptor", nat::SharedMemoryBase::PARENT);
    ctx.acceptor_shm = &config_shm;
    ctx.available_address = &available_address;
    //
    acceptor_setup_udssrv(&ctx);
    acceptor_setup_tcpsrv(&ctx);
    acceptor_loop(&ctx);

    // main thread loop.
    while(!__halt){
        usleep(10000);
    }
    if (ctx.event_base){
        event_base_loopexit(ctx.event_base, NULL);
        event_base_free(ctx.event_base);
    }
    closelog();
    //
    return(0);
}
