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

#ifndef Loop_SyncRecv_UNIX_H
#define Loop_SyncRecv_UNIX_H


#include "Loop/Loop/Loop.h"
#include "ECore/Core/SmartBuffer.h"
#include <sys/socket.h>
#include <errno.h>


EXTERN_C_BEGIN


int loop_sync_raw_recv(int fd, char* buf, int size);


int loop_sync_tcp_recv(int fd, char* buf, int size);


EXTERN_C_END


template <class T, std::size_t SZ>
inline int loop_sync_tcp_recv_stream(int fd, Core::SmartBuffer<T, SZ>* sbuf, unsigned int block_size)
{
    int n = 0;
    int rc = 0;
    char block_buf[block_size];
    sbuf->clear();
    do 
    {
        rc = loop_sync_tcp_recv(fd, block_buf, block_size);
        if (rc > 0)
        {
            n += rc;
            sbuf->append(block_buf, rc);
        }
    } while (rc > 0);

    return n;
}


#endif  // Loop_SyncRecv_UNIX_H
