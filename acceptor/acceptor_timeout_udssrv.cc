/******************************************************************************/
/*! @brief      Unix domain socket server timeout handler
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"
#include "nat_shared_memory_config.hh"

void acceptor_timeout_udssrv(evutil_socket_t sock, short what, void *arg) {
    auto ctx = (context_ptr)arg;
    auto shm = (nat::SharedMemoryConfig*)ctx->acceptor_shm;
    std::map<uint32_t, uint32_t>    available_address;
    nat::U128  cfg[nat::CONFIG_ITEM_COUNT] = {0ULL};

    // if there is modify.
    if (shm->Modified() != nat::OK) {
        return;
    }
    if (shm->Read(cfg, nat::CONFIG_ITEM_COUNT) != nat::OK) {
        return;
    }
    for(unsigned n = 0;n < nat::CONFIG_ITEM_COUNT;n++) {
        if(!cfg[n].hi || !cfg[n].low) { continue; }
        auto in_vlan = ((uint16_t)(cfg[n].hi >> 48));
        auto in_address = ((uint32_t)(cfg[n].hi >> 16));
        auto prefix = ((uint8_t)(cfg[n].low >> 8));
        //
        if (prefix < 16){
            continue;
        }
        //
        for(int i=0; i<(1<<(32 - prefix)); i++){
            available_address[in_address + htonl(i)] = 0;
        }
    }
    // debug print.
    for(auto it = available_address.begin();it != available_address.end();++it){
        LOG(LOG_INFO, "[%02x.%02x.%02x.%02x]",
            ((unsigned char*)&(it->first))[0],
            ((unsigned char*)&(it->first))[1],
            ((unsigned char*)&(it->first))[2],
            ((unsigned char*)&(it->first))[3]
        );
    }


    pthread_mutex_lock(&ctx->available_address_mutex);
    *((std::map<uint32_t, uint32_t>*)ctx->available_address) = available_address;
    pthread_mutex_unlock(&ctx->available_address_mutex);
}
