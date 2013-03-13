/*
The LGPL License (LGPL)

Copyright (c) 2012-2013 moon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef Loop_SocketOption_UNIX_H
#define Loop_SocketOption_UNIX_H


#include "Loop/Loop/Loop.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


EXTERN_C_BEGIN


int loop_set_blocking(int fd, bool flag);


int loop_set_nodelay(int fd, bool flag);


int loop_set_reuseaddress(int fd, bool flag);


int loop_set_reuseport(int fd, bool flag);


int loop_set_deferaccept(int fd, int ms);


int loop_set_linger(int fd, bool on, int seconds);


int loop_set_send_timeout(int fd, long milliseconds);


int loop_get_send_timeout(int fd, long* milliseconds);


int loop_set_recv_timeout(int fd, long milliseconds);


int loop_get_recv_timeout(int fd, long* milliseconds);


int loop_set_send_buffer_size(int fd, int size);


int loop_get_send_buffer_size(int fd, int* size);


int loop_set_recv_buffer_size(int fd, int size);


int loop_get_recv_buffer_size(int fd, int* size);



//////////////////////////////////////////////////////////////////////////
int loop_get_address(int fd, struct sockaddr* addr, socklen_t* addr_len);


//////////////////////////////////////////////////////////////////////////
int loop_tcp_socket();


int loop_udp_socket();


int loop_connect(int fd, const struct sockaddr* addr, socklen_t addr_len);


int loop_bind(int fd, const struct sockaddr* addr, socklen_t len);


int loop_listen(int fd, int backlog);


int loop_close(int fd);


EXTERN_C_END


#endif  // Loop_SocketOption_UNIX_H
