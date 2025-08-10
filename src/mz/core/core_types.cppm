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

        virtual constexpr std::ranges::iterator_t<std::span<T>> begin() = 0;
        virtual constexpr std::ranges::iterator_t<std::span<T>> end() = 0;
        virtual constexpr const std::ranges::iterator_t<std::span<T>> begin() const = 0;
        virtual constexpr const std::ranges::iterator_t<std::span<T>> end() const = 0;

        virtual constexpr std::size_t size() const = 0;

        virtual constexpr T* data() = 0;
        virtual constexpr const T* data() const = 0;

        virtual constexpr T& operator[](const std::size_t i) = 0;
        virtual constexpr const T& operator[](const std::size_t i) const = 0;
    };

    export template<typename T> 
    requires std::default_initializable<T>
    class DynamicArray
    {
    public:
        DynamicArray() noexcept = default;
        DynamicArray(const std::size_t size) : m_data{ std::make_unique<T[]>(size) }, m_size{ size } {}
        DynamicArray(const std::size_t size, const T& val) : m_data{ std::make_unique<T[]>(size) }, m_size{ size } 
        {
            std::ranges::fill(*this, val);
        }
        DynamicArray(const DynamicArray& other) : m_data{ std::make_unique<T[]>(other.m_size) }, m_size{ other.m_size } 
        {
            std::ranges::copy(other, begin());
        }
        DynamicArray(DynamicArray&& other) noexcept : m_data{ std::move(other.m_data) }, m_size{ other.m_size } 
        { 
            other.m_data = nullptr; 
            other.m_size = 0; 
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

        constexpr std::size_t size() const { return m_size; } 

        constexpr auto begin() noexcept { return std::span{data(), m_size}.begin(); }
        constexpr auto begin() const noexcept { return std::span{data(), m_size}.begin(); }

        constexpr auto end() noexcept { return std::span{data(), m_size}.end(); }
        constexpr auto end() const noexcept { return std::span{data(), m_size}.end(); }

        constexpr T& operator[](const std::size_t i) noexcept { return m_data[i]; }
        constexpr const T& operator[](const std::size_t i) const noexcept { return m_data[i]; }

        constexpr T& at(std::size_t i) 
        { 
            if (i >= m_size) throw std::out_of_range("Array index out of range");
            return m_data[i]; 
        }

        constexpr const T& at(std::size_t i) const
        { 
            if (i >= m_size) throw std::out_of_range("Array index out of range");
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

    template<typename T>
    concept KeyValueContainer = std::ranges::range<T> &&
        requires { typename std::ranges::range_value_t<T>; } &&
        requires {
            typename std::tuple_element_t<0, std::ranges::range_value_t<T>>;
            typename std::tuple_element_t<1, std::ranges::range_value_t<T>>;
        };

    template<typename T>
    concept FlatContainer = std::ranges::range<T> && !KeyValueContainer<T>;

    template <typename T>
    concept ContainerType = KeyValueContainer<T> || FlatContainer<T>;

    export template<ContainerType T>
    class Container : public T, public IIterable<std::ranges::range_value_t<T>>
    {
    public:
        using value_type = std::ranges::range_value_t<T>;
        using T::T;

        operator T&() { return static_cast<T&>(*this);; }
        operator const T&() const { return static_cast<T&>(*this);; }

        DynamicArray<value_type> toArray() requires FlatContainer<T>
        {
            DynamicArray<value_type> arr(T::size());
            std::ranges::copy(*this, arr.begin());
            return arr;
        }

        std::vector<value_type> toVector() requires FlatContainer<T>
        {
            std::vector<value_type> vec(T::size());
            std::ranges::copy(*this, vec.begin());
            return vec;
        }

        auto keys() requires KeyValueContainer<T>
        {
            return std::views::keys(*this);
        }

        auto values() requires KeyValueContainer<T>
        {
            return std::views::values(*this);
        }

        constexpr value_type* begin() override { return &(*T::begin()); }
        constexpr value_type* end() override { return &(*T::end()); }
        constexpr const value_type* begin() const override { return &(*T::begin()); }
        constexpr const value_type* end() const override { return &(*T::end()); }
        constexpr std::size_t size() const  override { return T::size(); }
        constexpr value_type* data() override { return T::data(); }
        constexpr const value_type* data() const override { return T::data(); }
        constexpr value_type& operator[](const std::size_t i) override
        { 
            if constexpr (FlatContainer<T>) {
                return T::operator[](i);
            }
            else {
                auto it = T::begin();
                std::ranges::advance(it, i); 
                return *it;
            }    
        }
        constexpr const value_type& operator[](const std::size_t i) const override
        { 
            if constexpr (FlatContainer<T>) {
                return T::operator[](i);
            }
            else {
                auto it = T::begin();
                std::ranges::advance(it, i); 
                return *it;
            }
        }
    };

    export template<typename T>
    using Array = Container<DynamicArray<T>>;

    export template<typename T>
    using Vector = Container<std::vector<T>>;

    export template<typename Key, typename T>
    using Map = Container<std::map<Key, T>>;

    static_assert(std::ranges::forward_range<DynamicArray<int>>);
    static_assert(std::ranges::forward_range<const DynamicArray<int>>);

    static_assert(KeyValueContainer<std::map<int, double>>);
    static_assert(!KeyValueContainer<std::vector<int>>);

    static_assert(!FlatContainer<std::map<int, double>>);
    static_assert(FlatContainer<std::vector<int>>);

    static_assert(ContainerType<std::map<int, double>>);
    static_assert(ContainerType<std::vector<int>>);

}

