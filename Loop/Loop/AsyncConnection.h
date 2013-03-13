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

#ifndef Loop_AsyncConnection_H
#define Loop_AsyncConnection_H


#include "Loop/Loop/AsyncEvent.h"
#include "Loop/Loop/SocketOption.h"
#include "ECore/Net/SocketAddress.h"
#include "ECore/Core/RefCountedObject.h"
#include "ECore/Core/AutoPtr.h"


namespace Loops {
namespace Loop {


class AsyncConnection: public AsyncEvent, public Core::RefCountedObject
{
public:
    typedef Core::AutoPtr<AsyncConnection> Ptr;

    AsyncConnection();
    AsyncConnection(const char* host, int port);
    AsyncConnection(struct sockaddr* ads, int addr_len);
    AsyncConnection(int fd, struct sockaddr* ads, int addr_len);
    AsyncConnection(const Core::Net::SocketAddress& address);
    AsyncConnection(int fd, const Core::Net::SocketAddress& address);
    virtual ~AsyncConnection();

    void shutdown();

public:
    volatile int fd;
    Core::Net::SocketAddress addr;
};


//
// inlines
//
inline AsyncConnection::AsyncConnection():
    fd(CORE_INVALID_SOCKET)
{
    impl()->ptr = this;
}

inline AsyncConnection::AsyncConnection(const char* host, int port):
fd(CORE_INVALID_SOCKET),
    addr(host, port)
{
    impl()->ptr = this;
}

inline AsyncConnection::AsyncConnection(struct sockaddr* ads, int addr_len):
    fd(CORE_INVALID_SOCKET),
    addr(ads, addr_len)
{
    impl()->ptr = this;
}

inline AsyncConnection::AsyncConnection(int fd, struct sockaddr* ads, int addr_len):
    fd(fd),
    addr(ads, addr_len)
{
    impl()->ptr = this;
}

inline AsyncConnection::AsyncConnection(const Core::Net::SocketAddress& address):
    fd(CORE_INVALID_SOCKET),
    addr(address)
{
    impl()->ptr = this;
}

inline AsyncConnection::AsyncConnection(int nfd, const Core::Net::SocketAddress& address):
    fd(nfd),
    addr(address)
{
    core_assert_printf(fd >= 0, "fd >= 0");
    impl()->ptr = this;
}

inline AsyncConnection::~AsyncConnection()
{
    shutdown();
}

inline void AsyncConnection::shutdown()
{
    if (fd >= 0)
    {
        loop_close(fd);
        fd = -1;
    }
}


//////////////////////////////////////////////////////////////////////////
typedef std::vector<AsyncConnection::Ptr> ConnectionList;


}}  // namespace Loops::Loop


#endif  // Loop_AsyncConnection_H
