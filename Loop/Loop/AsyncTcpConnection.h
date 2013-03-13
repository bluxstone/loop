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

#ifndef Loop_AsyncTcpConnection_H
#define Loop_AsyncTcpConnection_H


#include "Loop/Loop/AsyncConnection.h"
#include "ECore/Core/Timestamp.h"


namespace Loops {
namespace Loop {


class AsyncTcpConnection: public AsyncConnection
{
public:
    AsyncTcpConnection(bool keep_alive = false);
    AsyncTcpConnection(const char* host, int port, bool keep_alive = false);
    AsyncTcpConnection(struct sockaddr* ads, int addr_len, bool keep_alive = false);
    AsyncTcpConnection(int fd, struct sockaddr* ads, int addr_len, bool keep_alive = false);
    AsyncTcpConnection(const Core::Net::SocketAddress& address, bool keep_alive = false);
    AsyncTcpConnection(int fd, const Core::Net::SocketAddress& address, bool keep_alive = false);
    virtual ~AsyncTcpConnection();

    void keeping();

public:
    Core::Timestamp timeout;
    bool keepAlive;
};


//
// inlines
//
inline AsyncTcpConnection::AsyncTcpConnection(bool keep_alive):
    AsyncConnection(),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::AsyncTcpConnection(const char* host, int port, bool keep_alive):
AsyncConnection(host, port),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::AsyncTcpConnection(struct sockaddr* ads, int addr_len, bool keep_alive):
    AsyncConnection(ads, addr_len),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::AsyncTcpConnection(int fd, struct sockaddr* ads, int addr_len, bool keep_alive):
    AsyncConnection(fd, ads, addr_len),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::AsyncTcpConnection(const Core::Net::SocketAddress& address, bool keep_alive):
    AsyncConnection(address),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::AsyncTcpConnection(int fd, const Core::Net::SocketAddress& address, bool keep_alive):
    AsyncConnection(fd, address),
    timeout(0),
    keepAlive(keep_alive)
{
    impl()->ptr = this;
}

inline AsyncTcpConnection::~AsyncTcpConnection()
{
}

inline void AsyncTcpConnection::keeping()
{
    timeout.update();
}


}}  // namespace Loops::Loop


#endif  // Loop_AsyncTcpConnection_H
