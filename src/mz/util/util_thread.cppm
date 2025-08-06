module;

#ifdef MZ_OS_LINUX
    #include <pthread.h>
#endif

export module mz.util.thread;

import std;

namespace mz {

    export class Thread
    {
    public:
        Thread() = default;
        template<typename Callable, typename... Args>
        explicit Thread(Callable&& func, Args&&... args)
            : m_running{false}, m_thread{}
        {
            run(std::forward<Callable>(func), std::forward<Args>(args)...);
        }

        Thread(Thread&& other) : m_running{other.m_running.load()}, m_thread{std::move(other.m_thread)} {};
        Thread& operator=(Thread&& other) { 
            shutdown();
            m_running.exchange(other.m_running); 
            m_thread = std::move(other.m_thread); 
            return *this;
        };

        virtual ~Thread()
        {
            shutdown();
        }

        template<typename Callable, typename... Args>
        void start(Callable&& func, Args&&... args)
        {
            if (m_running)
                throw std::runtime_error("Thread already running");
            
            m_running = true;
            m_thread = std::thread(std::forward<Callable>(func), std::forward<Args>(args)...);
        }

        virtual void shutdown()
        {
            m_running = false;
            if (m_thread.joinable())
                m_thread.join();
        }

        bool setPriority(const int priority, const int policy = 0)
        {
            return setPriority(m_thread.native_handle(), priority, policy);
        }

    #ifdef MZ_OS_WINDOWS
        static bool setPriority(HANDLE thread, const int policy, const int priority = 0)
        {
            return SetThreadPriority((HANDLE)thread, priority) != 0;
        }
    #elif MZ_OS_LINUX
        static bool setPriority(pthread_t thread, const int priority, const int policy)
        {
            sched_param sch_params;
            sch_params.sched_priority = priority;
            auto ret = pthread_setschedparam(thread, policy, &sch_params);
            // if (ret != 0)
            //     LOG_WARN << "Failed to change priority of thread: " << thread << " with error: " << strerror(ret);

            return ret == 0;
        }
    #endif

    #ifdef MZ_OS_WINDOWS
        inline static HANDLE currentId() { return GetCurrentThread(); }
    #elif MZ_OS_LINUX
        inline static pthread_t currentId() { return pthread_self(); }
    #endif

        inline bool running() const { return m_running.load(); }

    protected:
        std::atomic<bool> m_running;
        std::thread m_thread;

    private:
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

    };

    export class CyclicThread : public Thread
    {
    public:
        CyclicThread() = default;
        template<typename Callable, typename... Args>
        explicit CyclicThread(const std::chrono::steady_clock::duration& cycleTime, Callable&& func, Args&&... args)
        {
            startCyclic(cycleTime, std::forward<Callable>(func), std::forward<Args>(args)...);
        }

        virtual ~CyclicThread()
        {
            shutdown();
        }

        template<typename Callable, typename... Args>
        void startCyclic(const std::chrono::steady_clock::duration& cycleTime, Callable&& func, Args&&... args)
        {
            auto cyclicFunc = std::bind(std::forward<Callable>(func), std::forward<Args>(args)...);
            start([this, cycleTime, cyclicFunc = std::move(cyclicFunc)]() mutable -> void {
                std::mutex mutex;
                std::unique_lock<std::mutex> lock(mutex);
                while (m_running) {
                    const auto cycleEnd = std::chrono::steady_clock::now() + cycleTime;
                    lock.unlock();
                    cyclicFunc();
                    lock.lock();

                    m_condition.wait_until(lock, cycleEnd, [this]() { return !m_running; });
                }
            });
        }

        void shutdown() override
        {
            m_running = false;
            m_condition.notify_all();
            if (m_thread.joinable())
                m_thread.join();
        }

    private:
        CyclicThread(const CyclicThread&) = delete;
        CyclicThread& operator=(const CyclicThread&) = delete;
        CyclicThread(CyclicThread&&) = delete;
        CyclicThread& operator=(CyclicThread&&) = delete;

        std::condition_variable m_condition;

        using Thread::start;
    };

    export template <typename T>
    class ConcurrentQueue
    {
    public:
        ConcurrentQueue() = default;
        ~ConcurrentQueue() = default;

        bool empty() const { std::lock_guard<std::mutex> lock(m_mutex); return m_queue.empty(); }
        std::size_t size() const { std::lock_guard<std::mutex> lock(m_mutex); return m_queue.size(); }
        bool contains(const T& val) const { std::lock_guard<std::mutex> lock(m_mutex); return std::find(m_queue.begin(), m_queue.end(), val) != m_queue.end(); }

        void push(const T& value) { std::lock_guard<std::mutex> lock(m_mutex); m_queue.push_back(value); }
        void push(T&& value) { std::lock_guard<std::mutex> lock(m_mutex); m_queue.push_back(std::move(value)); }

        template< typename... Type>
        decltype(auto) emplace(Type&&... args) { std::lock_guard<std::mutex> lock(m_mutex); return m_queue.emplace_back(std::forward<Type>(args)...); }

        std::optional<T> pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_queue.empty()) {
                return {};
            }
            T temp = std::move(m_queue.front());
            m_queue.pop_front();
            return temp;
        }

    private:
        std::deque<T> m_queue;
        mutable std::mutex m_mutex;

    };

}