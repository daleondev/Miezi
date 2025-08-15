module;
#include "mz/core/core.h"
export module mz.util;

import std;

import mz.core.types;
import mz.core.logging;

import mz.math.geometry;

namespace mz {

    export using UUID = std::uint64_t;
    export constexpr UUID UUID_NULL = std::numeric_limits<UUID>::min();

    export UUID uuid()
    {
        static UUID id = UUID_NULL;
        return ++id;
    }

    export Vector<std::string> splitString(std::string str, const char* delims)
    {
        Vector<std::string> parts;

        std::size_t prev = 0, pos;
        while ((pos = str.find_first_of(delims, prev)) != std::string::npos) {
            if (pos > prev)
                parts.push_back(str.substr(prev, pos - prev));
            prev = pos + 1;
        }
        if (prev < str.size())
            parts.push_back(str.substr(prev, std::string::npos));

        return parts;
    }

    export std::string readFile(const std::filesystem::path& filePath)
    {
        // open file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return "";
        }

        // get file size
        file.seekg(0, std::ios::end);
        const std::size_t size = file.tellg();    
        if (size < 1) {
            return "";
        }

        // read file content
        std::string content(size, ' ');
        file.seekg(0, std::ios::beg);   
        file.read(content.data(), size);
        return content;
    }

    export std::uint32_t vecToRGBA(const Vec4& color)
    {
        const std::uint8_t r = static_cast<std::uint8_t>(color.r * 255.0f);
        const std::uint8_t g = static_cast<std::uint8_t>(color.g * 255.0f);
        const std::uint8_t b = static_cast<std::uint8_t>(color.b * 255.0f);
        const std::uint8_t a = static_cast<std::uint8_t>(color.a * 255.0f);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }

    export constexpr bool const_strcmp(const char* a, const char* b) {
        for (;*a || *b;){
            if (*a++!=*b++)
                return false;
        }
        return true;
    }

    export template <typename T>
    class EdgeDetector
    {
    public:
        EdgeDetector() = default;
        EdgeDetector(const T& val) : m_val{val} {}
        ~EdgeDetector() = default;

        EdgeDetector& operator()()
        {
            if constexpr (std::is_same<T, bool>::value) {
                m_rising = m_val && !m_prev;
                m_falling = !m_val && m_prev;
                m_edge = m_rising || m_falling;
            }
            else if constexpr (std::is_arithmetic<T>()) {
                m_rising = m_val > m_prev;
                m_falling = m_val < m_prev;
                m_edge = m_rising || m_falling;
            }
            else {
                m_rising = false;
                m_falling = false;
                m_edge = m_val != m_prev;
            }
            m_prev = m_val;
            return *this;
        }

        T& val() { return m_val; }
        const T& val() const { return m_val; }

        bool edge() const { return m_edge; };
        bool rising() const { return m_rising; };
        bool falling() const { return m_falling; };

        operator T&() { return m_val; } 
        operator const T&() const { return m_val; } 

    private:
        T m_val;
        T m_prev;

        bool m_edge;
        bool m_rising;
        bool m_falling;

    };

    export template<typename T>
    void printIterable(IIterable<T>* iterable)
    {
        std::stringstream ss;
        for (std::size_t i = 0; i < iterable->size(); ++i)
            ss << "\n" << i << ": " << (*iterable)[i];

        MZ_TRACE("{}", ss.str());
    }

}