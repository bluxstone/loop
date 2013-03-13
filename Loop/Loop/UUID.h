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

#ifndef Loop_UUID_H
#define Loop_UUID_H


#include "ECore/Core/CRC.h"


namespace Loops {
namespace Loop {


class UUID
{
public:
    UUID();
    UUID(char* ptr, int len);
    ~UUID();

    UUID& operator = (const UUID& other);
    bool operator == (const UUID& other) const;
    bool operator != (const UUID& other) const;

    char d[12];
};


//
// inlines
//
inline UUID::UUID()
{
    memset(d, 0, sizeof(d));
}

inline UUID::UUID(char* ptr, int len)
{
    memcpy(d, ptr, len <= sizeof(d) ? len : sizeof(d));
}

inline UUID::~UUID()
{
}

inline UUID& UUID::operator = (const UUID& other)
{
    if (this != &other)
    {
        memcpy(d, other.d, sizeof(d));
    }

    return *this;
}

inline bool UUID::operator == (const UUID& other) const
{
    return 0 == memcmp(d, other.d, sizeof(d));
}

inline bool UUID::operator != (const UUID& other) const
{
    return 0 != memcmp(d, other.d, sizeof(d));
}


//////////////////////////////////////////////////////////////////////////
class DUIDGenerator
{
public:
    DUIDGenerator();
    ~DUIDGenerator();

    UUID create();
    UUID create(const char* base, int len);

private:
    int _seed;
};


}}  // namespace Loops::Loop


//////////////////////////////////////////////////////////////////////////
namespace Core {


inline std::size_t hash(const Loops::Loop::UUID& uid)
{
    std::size_t h = 0;
    int i = 0;
    while (i < sizeof(uid.d))
    {
        h = h * 0xf4243 ^ uid.d[i++];
    }
    return h;
}


}   // namespace Core


#endif  // Loop_UUID_H
