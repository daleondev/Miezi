export module mz.core.types;

import std;

namespace mz {

    export class ICastable 
    {
    public:
        enum class CastError { BadCast };

        virtual ~ICastable() = default;

        template<class T>
        bool is() const {  return dynamic_cast<const T*>(this) != nullptr; }

        template<class T>
        T* asPtr() {  return dynamic_cast<T*>(this); }

        template<class T>
        const T* asPtr() const {  return dynamic_cast<const T*>(this); }

        template<class T>
        std::expected<std::reference_wrapper<T>, CastError> asRef()
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected(CastError::BadCast);
            return *casted;
        }

        template<class T>
        std::expected<std::reference_wrapper<const T>, CastError> asRef() const
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected(CastError::BadCast);
            return *casted;
        }

        template<class T>
        std::expected<T, CastError> asCopy() const
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected(CastError::BadCast);
            return *casted;
        }

        template<class T>
        T* asPtrUnchecked() { return static_cast<T*>(this); }

        template<class T>
        const T* asPtrUnchecked() const { return static_cast<const T*>(this); }

        template<class T>
        T& asRefUnchecked() { return *asPtrUnchecked<T>(); }

        template<class T>
        const T& asRefUnchecked() const { return *asPtrUnchecked<T>(); }

        template<class T>
        T asCopyUnchecked() const { return *asPtrUnchecked<T>(); }
    };

    export template<typename T>
    class IIterable 
    {
    public:
        ~IIterable() = default;

        virtual std::ranges::iterator_t<std::span<T>> begin() = 0;
        virtual std::ranges::iterator_t<std::span<const T>> begin() const = 0;

        virtual std::ranges::iterator_t<std::span<T>> end() = 0;
        virtual std::ranges::iterator_t<std::span<const T>> end() const = 0;

        virtual std::size_t size() const = 0;

        virtual T* data() = 0;
        virtual const T* data() const = 0;

        virtual T& operator[](const std::size_t i) = 0;
        virtual const T& operator[](const std::size_t i) const = 0;
    };

    export template<typename T> 
    requires std::default_initializable<T>
    class ManagedDynamicArray
    {
    public:
        ManagedDynamicArray() noexcept = default;
        ManagedDynamicArray(const std::size_t size) : m_data{ std::make_unique<T[]>(size) }, m_size{ size } {}
        ManagedDynamicArray(const std::size_t size, const T& val) : m_data{ std::make_unique<T[]>(size) }, m_size{ size } 
        {
            std::ranges::fill(*this, val);
        }
        ManagedDynamicArray(const ManagedDynamicArray& other) : m_data{ std::make_unique<T[]>(other.m_size) }, m_size{ other.m_size } 
        {
            std::ranges::copy(other, begin());
        }
        ManagedDynamicArray(ManagedDynamicArray&& other) noexcept : m_data{ std::move(other.m_data) }, m_size{ other.m_size } 
        { 
            other.m_size = 0; 
        }
        ManagedDynamicArray(const std::initializer_list<T>& data) noexcept : m_data{ std::make_unique<T[]>(data.size()) }, m_size{ data.size() } 
        {
            std::ranges::move(data, begin());
        }

        ManagedDynamicArray& operator=(const ManagedDynamicArray& other)
        {
            if (this != &other) {
                ManagedDynamicArray temp(other);
                std::swap(m_data, temp.m_data);
                std::swap(m_size, temp.m_size);
            }
            return *this;
        }

        ManagedDynamicArray& operator=(ManagedDynamicArray&& other) noexcept
        {
            if (this != &other) {
                m_data = std::move(other.m_data);
                m_size = other.m_size;
                other.m_size = 0;
            }
            return *this;
        }

        void clear() noexcept
        {
            m_data.reset();
            m_size = 0;
        }

        void resize(const std::size_t size, const T& val = T{}) 
        {
            auto newData = std::make_unique<T[]>(size);

            const std::size_t copySize = std::min(size, m_size);
            std::move(begin(), begin()+copySize, newData.get());

            if (size > copySize)
                std::fill(newData.get()+copySize, newData.get()+size, val);

            m_data.swap(newData);
            m_size = size;
        }

        constexpr bool empty() const { return m_size == 0; } 

        constexpr std::size_t size() const { return m_size; } 

        constexpr auto begin() noexcept { return std::span{data(), m_size}.begin(); }
        constexpr auto begin() const noexcept { return std::span{data(), m_size}.begin(); }

        constexpr auto end() noexcept { return std::span{data(), m_size}.end(); }
        constexpr auto end() const noexcept { return std::span{data(), m_size}.end(); }

        constexpr T& operator[](const std::size_t i) noexcept { return m_data[i]; }
        constexpr const T& operator[](const std::size_t i) const noexcept { return m_data[i]; }

        constexpr T& at(std::size_t i) 
        { 
            if (i >= m_size) throw std::out_of_range("ManagedDynamicArray index out of range");
            return m_data[i]; 
        }

        constexpr const T& at(std::size_t i) const
        { 
            if (i >= m_size) throw std::out_of_range("ManagedDynamicArray index out of range");
            return m_data[i]; 
        }

        constexpr T* data() noexcept {
            return m_data.get();
        }

        constexpr const T* data() const noexcept {
            return m_data.get();
        }
        
    private:
        std::unique_ptr<T[]> m_data;
        std::size_t m_size;

    };

    static_assert(std::ranges::forward_range<ManagedDynamicArray<int>>);
    static_assert(std::ranges::forward_range<const ManagedDynamicArray<int>>);

    export template<std::ranges::contiguous_range T>
    class FlatContainer;

    export template<typename T>
    using DynamicArray = FlatContainer<ManagedDynamicArray<T>>;

    export template<typename T, std::size_t N>
    using Array = FlatContainer<std::array<T, N>>;

    export template<typename T>
    using Vector = FlatContainer<std::vector<T>>;

    export template<std::ranges::contiguous_range T>
    class FlatContainer : public T, public IIterable<std::ranges::range_value_t<T>>
    {
    public:
        using value_type = std::ranges::range_value_t<T>;
        using T::T;

        operator T&() { return static_cast<T&>(*this); }
        operator const T&() const { return static_cast<T&>(*this); }

        DynamicArray<value_type> toDynamicArray()
        {
            DynamicArray<value_type> arr(T::size());
            std::ranges::copy(*this, arr.begin());
            return arr;
        }

        template<std::size_t N>
        Array<value_type, N> toArray()
        {
            Array<value_type, N> arr;
            const std::size_t copySize = std::min(N, T::size());
            std::copy(begin(), begin()+copySize, arr.begin());
            return arr;
        }

        Vector<value_type> toVector()
        {
            Vector<value_type> vec(T::size());
            std::ranges::copy(*this, vec.begin());
            return vec;
        }

        std::ranges::iterator_t<std::span<value_type>> begin() override { return T::begin(); }
        std::ranges::iterator_t<std::span<const value_type>> begin() const override { return T::begin(); }

        std::ranges::iterator_t<std::span<value_type>> end() override { return T::end(); }
        std::ranges::iterator_t<std::span<const value_type>> end() const override { return T::end(); }

        std::size_t size() const  override { return T::size(); }

        value_type* data() override { return T::data(); }
        const value_type* data() const override { return T::data(); }

        value_type& operator[](const std::size_t i) override { return T::operator[](i); }
        const value_type& operator[](const std::size_t i) const override { return T::operator[](i); }
    };

}

