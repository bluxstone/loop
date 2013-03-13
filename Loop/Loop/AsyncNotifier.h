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

#ifndef Loop_AsyncNotifier_H
#define Loop_AsyncNotifier_H


#include "ECore/Core/Mutex.h"
#include "ECore/Core/Condition.h"
#include <deque>


namespace Loops {
namespace Loop {


template <class T>
class AsyncNotifier
{
public:
    AsyncNotifier(std::size_t max):
        _max(max)
    {
    }

    ~AsyncNotifier()
    {
        clear();
    }

public:
    bool enqueue(T& ev)
    {
        Core::FastMutex::ScopedLock lock(_lock);

        bool rc = false;
        if (_q.size() < _max)
        {
            _q.push_back(ev);
            _event.signal();
            rc = true;
        }
        
        return rc;
    }

    bool dequeue(T& ev)
    {
        Core::FastMutex::ScopedLock lock(_lock);

        return dequeueOne(ev);
    }

    bool waitDequeue(T& ev)
    {
        Core::FastMutex::ScopedLock lock(_lock);

        if (dequeueOne(ev))
        {
            return true;
        }
        else
        {
            _event.wait(_lock);
            return dequeueOne(ev);
        }
    }

    bool waitDequeue(T& ev, long milliseconds)
    {
        Core::FastMutex::ScopedLock lock(_lock);

        if (dequeueOne(ev))
        {
            return true;
        }
        else
        {
            if (_event.tryWait(_lock, milliseconds))
            {
                return dequeueOne(ev);
            }
            return false;
        }
    }

    bool empty()
    {
        return 0 == size();
    }

    std::size_t size()
    {
        Core::FastMutex::ScopedLock lock(_lock);
        return _q.size();
    }

    void clear()
    {
        Core::FastMutex::ScopedLock lock(_lock);
        _q.clear();
    }

    void wakeUpAll()
    {
        _event.broadcast();
    }

protected:
    bool dequeueOne(T& ev)
    {
        bool rc = false;
        if (!_q.empty())
        {
            ev = _q.front();
            _q.pop_front();
            rc = true;
        }

        return rc;
    }

private:
    typedef std::deque<T> NfQueue;

    volatile std::size_t _max;

    Core::FastMutex _lock;
    Core::Condition _event;

    NfQueue _q;
};


}}  // namespace Loops::Loop


#endif  // Loop_AsyncNotifier_H
