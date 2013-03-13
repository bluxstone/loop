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

#ifndef Loop_AsyncLoop_UNIX_H
#define Loop_AsyncLoop_UNIX_H


#include "Loop/Loop/AsyncEvent.h"
#include "Loop/Loop/AsyncConnection.h"
#include "ECore/Core/Timespan.h"


namespace Loops {
namespace Loop {


class AsyncLoopImpl
{
public:
    AsyncLoopImpl(int maxfd);
    ~AsyncLoopImpl();

public:
    int loop(ConnectionList& readList, ConnectionList& writeList, ConnectionList& exceptList, int maxListen, const Core::Timespan& timeout);
        /// multi-thread safe.

    bool addConnection(AsyncConnection::Ptr& conn);
    bool delConnection(AsyncConnection::Ptr& conn);
    bool modConnection(AsyncConnection::Ptr& conn);

private:
    void init();
    void release();

public:
    int _epfd;    // server epoll fd;
    int _maxfd;
};


//
// inlines
//
inline AsyncLoopImpl::AsyncLoopImpl(int maxfd):
    _maxfd(maxfd)
{
    init();
}

inline AsyncLoopImpl::~AsyncLoopImpl()
{
    release();
}


}}  // namespace Loops::Loop


#endif  // Loop_AsyncLoop_UNIX_H
