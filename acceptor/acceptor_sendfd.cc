/******************************************************************************/
/*! @brief      send file descriptor via socket
    @date       created(Jun 13, 2019)
    @par        Copyright 2019 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "acceptor.h"

int acceptor_sendfd(int sock, const int *fd) {
    struct msghdr msghdr;
    char nothing = '*';
    struct iovec nothing_ptr[1];
    struct cmsghdr *cmsg;
    int i;

    union {
        struct cmsghdr h;
        char   control[CMSG_SPACE(sizeof(*fd))];
    } buffer;

    memset(&buffer, 0, sizeof(struct cmsghdr) + sizeof(*fd));

    nothing_ptr[0].iov_base = &nothing;
    nothing_ptr[0].iov_len = sizeof(nothing);
    msghdr.msg_name = NULL;
    msghdr.msg_namelen = 0;
    msghdr.msg_iov = &nothing_ptr[0];
    msghdr.msg_iovlen = 1;
    msghdr.msg_flags = 0;
    msghdr.msg_control = buffer.control;
    msghdr.msg_controllen = sizeof(struct cmsghdr) + sizeof(*fd);
    cmsg = CMSG_FIRSTHDR(&msghdr);
//    cmsg = (struct cmsghdr*)buffer;
    cmsg->cmsg_len = msghdr.msg_controllen;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    ((int*)CMSG_DATA(cmsg))[0] = *fd;
    //
    return(sendmsg(sock, &msghdr, 0) >= 0 ? 0 : -1);
}
