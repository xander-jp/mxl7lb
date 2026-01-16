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

#include "nat_shared_memory_config.hh"


int main(int argc, char *argv[]){
    unsigned ipv4[4] = {0};
    unsigned ipv4_32 = 0;
    if (argc != 4){
        printf("%s <add/del/list> <ipv4 address> <mask bit>\n", argv[0]);
        printf("\tex a). %s add 192.168.0.10 32\n", argv[0]);
        printf("\tex b). %s add 192.168.10.0 24\n", argv[0]);
        printf("\tex c). %s del 192.168.10.0 24\n", argv[0]);
        return(1);
    }
    if (strcmp(argv[1], "add") != 0 &&
        strcmp(argv[1], "del") != 0 &&
        strcmp(argv[1], "list") != 0){
        printf("command required add/del\n");
        return(1);
    }

    if (sscanf(argv[2], "%u.%u.%u.%u", &ipv4[0], &ipv4[1], &ipv4[2], &ipv4[3]) != 4){
        printf("invalid ip address format\n");
        return(1);
    }
    ((unsigned char*)&ipv4_32)[0] = ((unsigned char)ipv4[0]);
    ((unsigned char*)&ipv4_32)[1] = ((unsigned char)ipv4[1]);
    ((unsigned char*)&ipv4_32)[2] = ((unsigned char)ipv4[2]);
    ((unsigned char*)&ipv4_32)[3] = ((unsigned char)ipv4[3]);

    nat::SharedMemoryConfig config_shm("/ipc_acceptor", nat::SharedMemoryBase::CHILD);
    nat::U128 cfg[nat::CONFIG_ITEM_COUNT] = {0ULL};
    // read current config.
    if (config_shm.Read(cfg, nat::CONFIG_ITEM_COUNT) != nat::OK){
        return(1);
    }
    if (strcmp(argv[1], "del") == 0){
        for(unsigned n = 0;n < nat::CONFIG_ITEM_COUNT;n++){
            if(!cfg[n].hi || !cfg[n].low) { continue; }

            auto in_address = ((uint32_t)(cfg[n].hi >> 16));
            auto prefix = ((uint8_t)(cfg[n].low >> 8));

            if (memcmp(&in_address, &ipv4_32, 4) == 0 && prefix == atoi(argv[3])){
                cfg[n].hi = 0;
                cfg[n].low = 0;
                //
                printf("<<del>%u>\n%02x %02x %02x %02x : %02x\n",
                       n,
                       ((unsigned char*)&in_address)[0],
                       ((unsigned char*)&in_address)[1],
                       ((unsigned char*)&in_address)[2],
                       ((unsigned char*)&in_address)[3],
                       ((unsigned char*)&prefix)[0]
                );
                break;
            }
        }
        if (config_shm.Update(cfg, nat::CONFIG_ITEM_COUNT) != nat::OK){
            printf("failed. SharedMemoryConfig::Update");
            return(-1);
        }
    }else if (strcmp(argv[1], "add") == 0){
        for(unsigned n = 0;n < nat::CONFIG_ITEM_COUNT;n++){
            if(!cfg[n].hi || !cfg[n].low) {
                cfg[n].hi = nat::U64((nat::U64(*((uint32_t*)&ipv4_32)) << 16));
                cfg[n].low = nat::U64((nat::U64(atoi(argv[3])) << 8));

                auto in_address = ((uint32_t)(cfg[n].hi >> 16));
                auto prefix = ((uint8_t)(cfg[n].low >> 8));

                printf("<<add>%u>\n%02x %02x %02x %02x : %02x\n",
                       n,
                       ((unsigned char*)&in_address)[0],
                       ((unsigned char*)&in_address)[1],
                       ((unsigned char*)&in_address)[2],
                       ((unsigned char*)&in_address)[3],
                       ((unsigned char*)&prefix)[0]
                );
                break;
            }
        }
        if (config_shm.Update(cfg, nat::CONFIG_ITEM_COUNT) != nat::OK){
            printf("failed. SharedMemoryConfig::Update");
            return(-1);
        }
    }else if (strcmp(argv[1], "list") == 0){
        printf("<<list>>\n");
        for(unsigned n = 0;n < nat::CONFIG_ITEM_COUNT;n++){
            if(!cfg[n].hi || !cfg[n].low) { continue; }
            auto in_address = ((uint32_t)(cfg[n].hi >> 16));
            auto prefix = ((uint8_t)(cfg[n].low >> 8));

            printf("%02x %02x %02x %02x : %02x\n",
                   ((unsigned char*)&in_address)[0],
                   ((unsigned char*)&in_address)[1],
                   ((unsigned char*)&in_address)[2],
                   ((unsigned char*)&in_address)[3],
                   ((unsigned char*)&prefix)[0]
            );
        }
    }
    return(0);
}
