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

#ifndef Loop_LockedHashHolder_H
#define Loop_LockedHashHolder_H


#include "ECore/Core/HashMap.h"
#include "ECore/Core/RWLock.h"
#include <list>


namespace Loops {
namespace Loop {


template < class TKey,
    class TValue,
    class TLock = Core::RWLock,
    class TScopedReadLock  = Core::RWLock::ScopedReadLock,
    class TScopedWriteLock = Core::RWLock::ScopedWriteLock,
    class THashFunc = Core::Hash<TKey> >
class LockedHashHolder
{
public:
    typedef Core::HashMap<TKey, TValue, THashFunc> HashMapType;
    typedef TLock LockType;
    typedef TScopedReadLock ScopedReadLockType;
    typedef TScopedWriteLock ScopedWriteLockType;

    inline LockedHashHolder(LockType& lock):
        _lock(lock)
    {
    }

    inline LockedHashHolder(std::size_t initial_reserve, LockType& lock):
        _lock(lock),
        _hm(initial_reserve)
    {
    }

    inline ~LockedHashHolder()
    {
    }

    inline bool begin(TKey& key, TValue& value)
    {
        typename HashMapType::Iterator it = _hm.begin();
        if (it != _hm.end())
        {
            key = it->first;
            value = it->second;
            return true;
        }
        return false;
    }

    inline bool begin(TKey& key, TValue& value, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        return begin(key, value);
    }

    inline bool insert(const TKey& key, TValue& value)
    {
        return _hm.insert(typename HashMapType::PairType(key, value)).second;
    }

    inline bool insert(const TKey& key, TValue& value, LockType& lk)
    {
        ScopedWriteLockType lock(lk);
        return insert(key, value);
    }

    inline void erase(const TKey& key, LockType& lk)
    {
        ScopedWriteLockType lock(lk);
        erase(key);
    }

    inline void erase(const TKey& key)
    {
        _hm.erase(key);
    }

    inline bool erase(const TKey& key, TValue& value, LockType& lk)
    {
        ScopedWriteLockType lock(lk);
        return erase(key, value);
    }

    inline bool erase(const TKey& key, TValue& value)
    {
        typename HashMapType::Iterator it = _hm.find(key);
        if (it != _hm.end())
        {
            value = it->second;
            _hm.erase(key);
            return true;
        }
        return false;
    }

    inline bool update(const TKey& key, TValue& value, LockType& lk)
    {
        ScopedWriteLockType lock(lk);
        return update(key, value);
    }

    inline bool update(const TKey& key, TValue& value)
    {
        typename HashMapType::Iterator it = _hm.find(key);
        if (it != _hm.end())
        {
            it->second = value;
            return true;
        }

        return false;
    }

    inline bool contain(const TKey& key, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        return contain(key);
    }

    inline bool contain(const TKey& key)
    {
        return _hm.find(key) != _hm.end();
    }


    inline bool find(const TKey& key, TValue& ret, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        return find(key, ret);
    }

    inline bool find(const TKey& key, TValue& ret)
    {
        typename HashMapType::Iterator it = _hm.find(key);
        if (it != _hm.end())
        {
            ret = it->second;
            return true;
        }
        return false;
    }

    inline bool findKey(const TValue& value, TKey& ret, LockType& lk)    // slow.
    {
        ScopedReadLockType lock(lk);
        return findKey(value, ret);
    }

    inline bool findKey(const TValue& value, TKey& ret)    // slow.
    {
        typename HashMapType::Iterator it = _hm.begin();
        while (it != _hm.end())
        {
            if (it->second == value)
            {
                ret = it->first;
                return true;
            }
            ++it;
        }

        return false;
    }

    inline void keys(std::vector<TKey>& keys, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        keys(keys);
    }

    inline void keys(std::vector<TKey>& keys)
    {
        //keys.clear();
        keys.reserve(keys.size + _hm.size());
        typename HashMapType::Iterator it = _hm.begin();

        while (it != _hm.end())
        {
            keys.push_back(it->first);
            ++it;
        }
    }

    inline void values(std::vector<TValue>& vals, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        values(vals);
    }

    inline void values(std::vector<TValue>& vals)
    {
        //vals.clear();
        vals.reserve(vals.size() + _hm.size());
        typename HashMapType::Iterator it = _hm.begin();

        while (it != _hm.end())
        {
            vals.push_back(it->second);
            ++it;
        }
    }

    inline void valuesList(std::list<TValue>& vals, LockType& lk)
    {
        ScopedReadLockType lock(lk);
        valuesList(vals);
    }

    inline void valuesList(std::list<TValue>& vals)
    {
        //vals.clear();
        typename HashMapType::Iterator it = _hm.begin();

        while (it != _hm.end())
        {
            vals.push_back(it->second);
            ++it;
        }
    }

    inline std::size_t size() const
    {
        return _hm.size();
    }

    inline void clear(LockType& lk) // clear all.
    {
        ScopedWriteLockType lock(lk);
        clear();
    }

    inline void clear() // clear all.
    {
        _hm.clear();
    }

    inline const HashMapType& hash() const
    {
        return _hm;
    }

    inline TLock& lock()
    {
        return _lock;
    }

protected:
    LockType& _lock;
    HashMapType _hm;
};


}}  // namespace Loops::Loop


#endif  // Loop_LockedHashHolder_H
