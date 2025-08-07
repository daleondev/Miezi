module;
#include <ctime>
export module mz.util.time;

import std;

namespace mz {

    export struct TimeData
    {
        std::int64_t timeSinceEpoch_ms;

        std::uint16_t year;
        std::uint16_t month;
        std::uint16_t day;
        std::uint16_t hour;
        std::uint16_t minute;
        std::uint16_t second;
        std::uint16_t millisecond;
    };

    export class Timestamp
    {
    public:
        Timestamp()
        {
            // local time
            std::tm localtime{};
            const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
#if MZ_OS_WINDOWS
            localtime_s(&localtime, &currentTime);
#elif MZ_OS_LINUX
            localtime_r(&currentTime, &localtime);
#endif

            // milliseconds
            const std::chrono::duration<double> tse = now.time_since_epoch();
            m_data.timeSinceEpoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count();

            // timestamp
            m_data.year			= static_cast<std::uint16_t>(localtime.tm_year + 1900);
            m_data.month		= static_cast<std::uint16_t>(localtime.tm_mon + 1);
            m_data.day			= static_cast<std::uint16_t>(localtime.tm_mday);
            m_data.hour			= static_cast<std::uint16_t>(localtime.tm_hour);
            m_data.minute		= static_cast<std::uint16_t>(localtime.tm_min);
            m_data.second		= static_cast<std::uint16_t>(localtime.tm_sec);
            m_data.millisecond	= static_cast<std::uint16_t>(m_data.timeSinceEpoch_ms % 1000);
        }

        Timestamp(const std::string& timestamp)
        {
            m_data.day			= static_cast<std::uint16_t>(std::stoi(timestamp.substr(0, 2)));
            m_data.month		= static_cast<std::uint16_t>(std::stoi(timestamp.substr(3, 2)));
            m_data.year			= static_cast<std::uint16_t>(std::stoi(timestamp.substr(6, 4)));
            m_data.hour			= static_cast<std::uint16_t>(std::stoi(timestamp.substr(11, 2)));
            m_data.minute		= static_cast<std::uint16_t>(std::stoi(timestamp.substr(14, 2)));
            m_data.second		= static_cast<std::uint16_t>(std::stoi(timestamp.substr(17, 2)));
            m_data.millisecond	= static_cast<std::uint16_t>(std::stoi(timestamp.substr(20, 3)));
        }

        ~Timestamp() = default;

        std::string dateStr() const
        {
            std::stringstream ss;

            m_data.day < 10 ? ss << '0' << m_data.day : ss << m_data.day;
            ss << '.';
            m_data.month < 10 ? ss << '0' << m_data.month : ss << m_data.month;
            ss << '.';
            ss << m_data.year;

            return ss.str();
        }

        std::string timeStr() const
        {
            std::stringstream ss;

            m_data.hour < 10 ? ss << '0' << m_data.hour : ss << m_data.hour;
            ss << ':';
            m_data.minute < 10 ? ss << '0' << m_data.minute : ss << m_data.minute;
            ss << ':';
            m_data.second < 10 ? ss << '0' << m_data.second : ss << m_data.second;
            ss << ':';
            m_data.millisecond < 100 ? m_data.millisecond < 10 ? ss << "00" << m_data.millisecond : ss << '0' << m_data.millisecond : ss << m_data.millisecond;

            return ss.str();
        }

        std::string dateTimeStr() const
        {
            std::stringstream ss;

            ss << dateStr();
            ss << ' ';
            ss << timeStr();

            return ss.str();
        }

        friend std::ostream& operator<<(std::ostream& os, const Timestamp& t)
        {
            os << t.dateTimeStr();
            return os;
        }
        
        friend bool operator!=(const Timestamp& lhs, const Timestamp& rhs)
        {
            return lhs.m_data.year != rhs.m_data.year || lhs.m_data.month != rhs.m_data.month || lhs.m_data.day != rhs.m_data.day
                || lhs.m_data.hour != rhs.m_data.hour || lhs.m_data.minute != rhs.m_data.minute || lhs.m_data.second != rhs.m_data.second || lhs.m_data.millisecond != rhs.m_data.millisecond;
        }

        friend bool operator>(const Timestamp& lhs, const Timestamp& rhs)
        {
            if (lhs.m_data.year > rhs.m_data.year)
                return true;
            if (lhs.m_data.year < rhs.m_data.year)
                return false;

            if (lhs.m_data.month > rhs.m_data.month)
                return true;
            if (lhs.m_data.month < rhs.m_data.month)
                return false;

            if (lhs.m_data.day > rhs.m_data.day)
                return true;
            if (lhs.m_data.day < rhs.m_data.day)
                return false;

            if (lhs.m_data.hour > rhs.m_data.hour)
                return true;
            if (lhs.m_data.hour < rhs.m_data.hour)
                return false;

            if (lhs.m_data.minute > rhs.m_data.minute)
                return true;
            if (lhs.m_data.minute < rhs.m_data.minute)
                return false;

            if (lhs.m_data.second > rhs.m_data.second)
                return true;
            if (lhs.m_data.second < rhs.m_data.second)
                return false;

            if (lhs.m_data.millisecond > rhs.m_data.millisecond)
                return true;

            return false;
        }

        TimeData getData() const { return m_data; }

    private:
        TimeData m_data;

    };

}

