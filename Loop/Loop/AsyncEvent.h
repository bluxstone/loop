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

#ifndef Loop_AsyncEvent_H
#define Loop_AsyncEvent_H


#ifndef _WIN32
#include "AsyncEvent_UNIX.h"
#endif
#include <vector>


namespace Loops {
namespace Loop {


class AsyncEvent
{
public:
    AsyncEvent();
    virtual ~AsyncEvent();

public:
    AsyncEvent& operator = (const AsyncEvent& other);

    AsyncEventImpl* impl() const;

private:
    AsyncEventImpl _impl;
};


typedef std::vector<AsyncEvent> EventList;


//
// inlines
//
inline AsyncEvent::AsyncEvent()
{
    impl()->ptr = this;
}

inline AsyncEvent::~AsyncEvent()
{
}

inline AsyncEvent& AsyncEvent::operator = (const AsyncEvent& other)
{
    if (&other != this)
    {
        _impl = *other.impl();
    }
    return *this;
}

inline AsyncEventImpl* AsyncEvent::impl() const
{
    return (AsyncEventImpl*)&_impl;
}


}}  // namespace Loops::Loop


#endif  // Loop_AsyncEvent_H
