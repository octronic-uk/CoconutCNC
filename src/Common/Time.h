#pragma once

#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace Coconut
{
    class Time
    {
    public:
        static inline long GetCurrentTime()
        {
    		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        }
    };
}
