/******************************************************************************/
/*! @brief      acceptor initialization
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

void acceptor_initialize(int argc, char *argv[], context_ptr ctx){
    int         ch,have_option = 0;
    //
    TAILQ_INIT(&ctx->uds_clients);
    TAILQ_INIT(&ctx->tcp_clients);
    pthread_mutex_init(&ctx->uds_clients_mutex, NULL);
    pthread_mutex_init(&ctx->tcp_clients_mutex, NULL);
    pthread_mutex_init(&ctx->available_address_mutex, NULL);

    ctx->addrlen = sizeof(ctx->addr);

    while ( (ch = getopt(argc, argv, "i:p:u:vhd")) != -1) {
        switch(ch){
            case 'i': /* bind ip address */
                if (optarg != NULL){
                    bzero(ctx->bind_ip_addr, sizeof(ctx->bind_ip_addr));
                    strncpy(ctx->bind_ip_addr, optarg, MIN(strlen(optarg), sizeof(ctx->bind_ip_addr)-1));
                    ctx->bind_ip_addr[strlen(ctx->bind_ip_addr)] = '\0';
                }
                break;
            case 'p': /* listen port number */
                if (optarg != NULL){
                    ctx->listen_port = atoi(optarg);
                }
                break;
            case 'u': /* unix domain socket name */
                if (optarg != NULL){
                    bzero(ctx->unix_domain_sock, sizeof(ctx->unix_domain_sock));
                    strncpy(ctx->unix_domain_sock, optarg, MIN(strlen(optarg), sizeof(ctx->unix_domain_sock)-1));
                    ctx->unix_domain_sock[strlen(ctx->unix_domain_sock)] = '\0';
                }
                break;
            case 'h':/* help */
                usage();
                exit(0);
                break;
            case 'v':/* verbose */
                ctx->verbose++;
                break;
            case 'd':/* debug acceptor */
                ctx->debug_acceptor++;
                break;
            default: /* unknown option */
                printf("Option %c is not defined.", ch);
                usage();
                exit(0);
        }
    }
    // invalid args.
    if (!ctx->bind_ip_addr[0] || !ctx->unix_domain_sock[0] || !ctx->listen_port){
        printf("missing option.(X)\n");
        usage();
        exit(0);
    }
}
