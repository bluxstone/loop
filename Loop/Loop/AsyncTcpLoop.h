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

#ifndef Loop_AsyncTcpLoop_H
#define Loop_AsyncTcpLoop_H


#include "Loop/Loop/AsyncTcpConnection.h"
#include "Loop/Loop/AsyncNotifier.h"
#include "Loop/Loop/AsyncLoop.h"
#include "Loop/Loop/SocketOption.h"
#include "ECore/Core/Condition.h"
#include "ECore/Core/SharedPtr.h"
#include "ECore/Core/Thread.h"
#include "ECore/Core/Runnable.h"
#include <list>
#include <vector>


namespace Loops {
namespace Loop {


class AsyncTcpLoop: public Core::Runnable
{
public:
    typedef std::list<AsyncTcpConnection::Ptr> TCPConnectionSharedList;
    typedef Core::SharedPtr<Core::Thread> ThreadPtr;
    typedef std::vector<ThreadPtr> ThrdPool;

    AsyncTcpLoop(AsyncTcpConnection::Ptr srv, int maxfd, int maxListen, int maxThread, int maxQueue);
    virtual ~AsyncTcpLoop();

    bool onConnectionAccept(AsyncTcpConnection::Ptr& conn);
    bool onConnectionRead(AsyncTcpConnection::Ptr& conn);
    bool onConnectionWrite(AsyncTcpConnection::Ptr& conn);
    
    virtual AsyncConnection* createConnection(int fd, struct sockaddr* ads, int addr_len);
    virtual void process(AsyncTcpConnection::Ptr& conn) = 0;
    virtual bool garbage(AsyncTcpConnection::Ptr& conn);
    
    bool appendConnection(AsyncTcpConnection::Ptr& conn);

    void start();
    void stop();

    void exec();

protected:
    void init();
    void acceptConnection(TCPConnectionSharedList& lst);
    bool posted(AsyncTcpConnection::Ptr& conn);
    void garbageDelay(AsyncTcpConnection::Ptr& conn, int timeout_ms = 0);    // timeout_ms == 0, delete immediately.

    virtual void processEvents(AsyncTcpConnection::Ptr& conn);

    void processGarbage();

    AsyncLoop& loop();
    AsyncTcpConnection::Ptr srv();

private:
    virtual void run();     // worker process.
    
private:
    typedef AsyncNotifier<AsyncConnection::Ptr> ConnectionNotifier;

    AsyncLoop _loop;
    volatile int _exit;
    AsyncConnection::Ptr _srv;
    TCPConnectionSharedList _connects;
    Core::FastMutex _lock;
    
    ThrdPool _thds;
    int _maxfd;
    int _maxListen;
    int _maxThread;
    ConnectionNotifier _notif;

    Core::Timestamp _gcTS;
};


//
// inlines
//
inline AsyncTcpLoop::AsyncTcpLoop(AsyncTcpConnection::Ptr srv, int maxfd, int maxListen, int maxThread, int maxQueue):
    _loop(maxfd),
    _exit(false),
    _srv(srv),
    _maxfd(maxfd),
    _maxListen(maxListen),
    _maxThread(maxThread),
    _notif(maxQueue)
{
    init();
}

inline AsyncTcpLoop::~AsyncTcpLoop()
{
    stop();
}

inline AsyncLoop& AsyncTcpLoop::loop()
{
    return _loop;
}

inline AsyncTcpConnection::Ptr AsyncTcpLoop::srv()
{
    return _srv;
}


}}  // namespace Loops::Loop


#endif  // Loop_AsyncTcpLoop_H
