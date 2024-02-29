#pragma once

#include <stdint.h>

namespace paper
{

    namespace time
    {
        static constexpr uint64_t NANOSECONDS_PER_HOUR = 3600000000000L;
        static constexpr uint64_t NANOSECONDS_PER_MINUTE = 60000000000L;
        static constexpr uint64_t NANOSECONDS_PER_SECOND = 1000000000;
        static constexpr uint64_t NANOSECONDS_PER_MILLISECOND = 1000000;

        static constexpr double HOUR_IN_NANOSECONDS = 2.777777777E-13;
        static constexpr double MINUTE_IN_NANOSECONDS = 1.666666666E-11;
        static constexpr double SECOND_IN_NANOSECONDS = 1.E-9;
        static constexpr double MILLISECOND_IN_NANOSECONDS = 0.000001;

        using nano = uint64_t;
        using milli = uint32_t;
        using seconds = float;

        inline milli to_milliseconds(nano value) { return value * MILLISECOND_IN_NANOSECONDS; }
        inline seconds to_seconds(nano value) { return value * SECOND_IN_NANOSECONDS; }

        inline nano from_milliseconds(milli value) { return value * NANOSECONDS_PER_MILLISECOND; }
        inline nano from_seconds(seconds value) { return value * NANOSECONDS_PER_SECOND; }

    } // namespace time

} // namespace paper
