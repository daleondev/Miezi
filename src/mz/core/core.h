#pragma once

//------------------------------------------------------
//                      General
//------------------------------------------------------

#define MZ_BIT(i)               (1 << i)
#define MZ_UNUSED(x)            (void)x

//------------------------------------------------------
//                      Logging
//------------------------------------------------------

#define METHOD_NAME                 mz::functionToLocation(std::source_location::current().function_name())

#define MZ_TRACE_LOC(loc, fmt, ...) mz::g_logger.trace(true, loc, fmt, ##__VA_ARGS__)
#define MZ_INFO_LOC(loc, fmt, ...)  mz::g_logger.info(true, loc, fmt, ##__VA_ARGS__)
#define MZ_WARN_LOC(loc, fmt, ...)  mz::g_logger.warn(true, loc, fmt, ##__VA_ARGS__)
#define MZ_ERROR_LOC(loc, fmt, ...) mz::g_logger.error(true, loc, fmt, ##__VA_ARGS__)
#define MZ_FATAL_LOC(loc, fmt, ...) mz::g_logger.fatal(true, loc, fmt, ##__VA_ARGS__)

#define MZ_TRACE(fmt, ...)		    mz::g_logger.trace(true, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_INFO(fmt, ...)		    mz::g_logger.info(true, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_WARN(fmt, ...)		    mz::g_logger.warn(true, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_ERROR(fmt, ...)		    mz::g_logger.error(true, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_FATAL(fmt, ...)		    mz::g_logger.fatal(true, METHOD_NAME, fmt, ##__VA_ARGS__)

#define MZ_TRACE_NOW(fmt, ...)	    mz::g_logger.trace(false, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_INFO_NOW(fmt, ...)	    mz::g_logger.info(false, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_WARN_NOW(fmt, ...)	    mz::g_logger.warn(false, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_ERROR_NOW(fmt, ...)	    mz::g_logger.error(false, METHOD_NAME, fmt, ##__VA_ARGS__)
#define MZ_FATAL_NOW(fmt, ...)	    mz::g_logger.fatal(false, METHOD_NAME, fmt, ##__VA_ARGS__)

#ifdef MZ_ENABLE_ASSERTS
    #ifdef MZ_OS_WINDOWS
        #include <intrin.h>
        #define MZ_DEBUG_BREAK() __debugbreak()
    #elif MZ_OS_LINUX
        #include <csignal>
        #define MZ_DEBUG_BREAK() std::raise(SIGTRAP)
    #else
        #define MZ_DEBUG_BREAK() std::abort()
    #endif

    #define MZ_ASSERT(x, msg, ...) \
        do { \
            if (!(x)) { \
                MZ_FATAL_NOW("Assertion failed: " msg, ##__VA_ARGS__); \
                MZ_DEBUG_BREAK(); \
            } \
        } while (false)
#else
    #define MZ_ASSERT(x, msg, ...) ((void)0)
#endif

//------------------------------------------------------
//                      Types
//------------------------------------------------------

#define ITERABLE_CONTAINER_PTR(container, value_type)   std::ranges::iterator_t<std::span<value_type>> begin() override { return container->begin(); } \
                                                        std::ranges::iterator_t<std::span<const value_type>> begin() const override { return container->begin(); } \
                                                        \
                                                        std::ranges::iterator_t<std::span<value_type>> end() override { return container->end(); } \
                                                        std::ranges::iterator_t<std::span<const value_type>> end() const override { return container->end(); } \
                                                        \
                                                        bool empty() const override { return container->empty(); } \
                                                        std::size_t size() const override { return container->size(); } \
                                                        \
                                                        value_type* data() override { return container->data(); } \
                                                        const value_type* data() const override { return container->data(); } \
                                                        \
                                                        value_type& operator[](const std::size_t i) override { return (*container)[i]; } \
                                                        const value_type& operator[](const std::size_t i) const override { return (*container)[i]; }

#define ITERABLE_CONTAINER(container, value_type)   IIterable<value_type>::Iterator begin() override { return container.begin(); } \
                                                    IIterable<value_type>::ConstIterator begin() const override { return container.begin(); } \
                                                    \
                                                    IIterable<value_type>::Iterator end() override { return container.end(); } \
                                                    IIterable<value_type>::ConstIterator end() const override { return container.end(); } \
                                                    \
                                                    bool empty() const override { return container.empty(); } \
                                                    std::size_t size() const override { return container.size(); } \
                                                    \
                                                    value_type* data() override { return container.data(); } \
                                                    const value_type* data() const override { return container.data(); } \
                                                    \
                                                    value_type& operator[](const std::size_t i) override { return container[i]; } \
                                                    const value_type& operator[](const std::size_t i) const override { return container[i]; }
