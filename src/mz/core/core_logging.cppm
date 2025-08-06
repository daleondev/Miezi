export module mz.core.logging;

import std;

import mz.util.thread;
import mz.util.time;

namespace mz {

    enum class LogLevel 
    {
        Trace,
        Info,
        Warn,
        Error,
        Fatal
    };

    enum class ConsoleColor : std::uint16_t 
    {
        Standard = 0,
        Red = 31,
        Green = 32,
        Yellow = 33
    };

    class Logger : public Thread
    {
    public:
        Logger()
        {
            std::println("logger created");
            start(&Logger::loggingTask, this);
        }
        ~Logger()
        {
            shutdown();
            std::println("logger destroyed");
        }

        void shutdown() override
        {
            m_running = false;
            m_condition.notify_all();
            if (m_thread.joinable())
                m_thread.join();
        }

        template<typename... T>
        void trace(const bool background, std::string location, std::format_string<T...> fmt, T&&... args)
        {
            background ?
                addMessage(LogLevel::Trace, Timestamp{}, std::move(location), std::format(fmt, std::forward<T>(args)...)) :
                printMessage(LogLevel::Trace, Timestamp{}, location, std::format(fmt, std::forward<T>(args)...));
        }

        template<typename... T>
        void info(const bool background, std::string location, std::format_string<T...> fmt, T&&... args)
        {
            background ?
                addMessage(LogLevel::Info, Timestamp{}, std::move(location), std::format(fmt, std::forward<T>(args)...)) :
                printMessage(LogLevel::Info, Timestamp{}, location, std::format(fmt, std::forward<T>(args)...));
        }

        template<typename... T>
        void warn(const bool background, std::string location, std::format_string<T...> fmt, T&&... args)
        {
            background ?
                addMessage(LogLevel::Warn, Timestamp{}, std::move(location), std::format(fmt, std::forward<T>(args)...)) :
                printMessage(LogLevel::Warn, Timestamp{}, location, std::format(fmt, std::forward<T>(args)...));
        }

        template<typename... T>
        void error(const bool background, std::string location, std::format_string<T...> fmt, T&&... args)
        {
            background ?
                addMessage(LogLevel::Error, Timestamp{}, std::move(location), std::format(fmt, std::forward<T>(args)...)) :
                printMessage(LogLevel::Error, Timestamp{}, location, std::format(fmt, std::forward<T>(args)...));
        }

        template<typename... T>
        void fatal(const bool background, std::string location, std::format_string<T...> fmt, T&&... args)
        {
            background ?
                addMessage(LogLevel::Fatal, Timestamp{}, std::move(location), std::format(fmt, std::forward<T>(args)...)) :
                printMessage(LogLevel::Fatal, Timestamp{}, location, std::format(fmt, std::forward<T>(args)...));
        }

    private:
        void addMessage(const LogLevel level, const Timestamp& timestamp, std::string&& location, std::string&& message)
        {
            m_queue.push(std::make_tuple(level, timestamp, std::move(location), std::move(message)));
            m_condition.notify_all();
        }

        void printMessage(const LogLevel level, const Timestamp& timestamp, const std::string& location, const std::string& message)
        {
            std::println("[{}] <{}>: {}", timestamp.timeStr(), location, message);
        }

        void loggingTask()
        {
            std::mutex mutex;
            std::unique_lock<std::mutex> lock(mutex);
            while (m_running) {
                lock.unlock();
                while (!m_queue.empty()) {
                    const auto[level, timestamp, location, message] = *m_queue.pop();
                    printMessage(level, timestamp, location, message);
                }
                lock.lock();

                m_condition.wait(lock, [this]() { return !m_queue.empty() || !m_running; });
            }
        }

        std::condition_variable m_condition;
        ConcurrentQueue<std::tuple<LogLevel, Timestamp, std::string, std::string>> m_queue;
    };

    export Logger g_logger;

    export std::string functionToLocation(const std::string& func)
    {
        std::smatch match;
        if (std::regex_search(func, match, std::regex(R"((?:\b(\w+)\b::)?\b(\w+)\b(?=\s*\())"))) {
            if (match[1].matched) {
                return match[1].str() + "::" + match[2].str();
            }
            else {
                return match[2].str();
            }
        }
        return "";
    }

}