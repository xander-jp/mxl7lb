/******************************************************************************/
/*! @brief      receive request from client
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

int acceptor_recieve_req(int sock, char* req) {
    struct msghdr msghdr;
    char result = '*';
    struct iovec nothing_ptr[1];

    nothing_ptr[0].iov_base = &result;
    nothing_ptr[0].iov_len = sizeof(result);
    msghdr.msg_name = NULL;
    msghdr.msg_namelen = 0;
    msghdr.msg_iov = &nothing_ptr[0];
    msghdr.msg_iovlen = 1;
    msghdr.msg_flags = 0;
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;
    //
    if (recvmsg(sock, &msghdr, 0) < 0){
        return(-1);
    }
    (*req) = result;
    return(0);
}
