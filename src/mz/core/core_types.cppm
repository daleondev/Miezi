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

        virtual T* begin() = 0;
        virtual T* end() = 0;
        virtual const T* begin() const = 0;
        virtual const T* end() const = 0;

        virtual constexpr std::size_t size() const = 0;

        virtual T* data() = 0;
        virtual const T* data() const = 0;

        virtual T& operator[](const std::size_t i) = 0;
        virtual const T& operator[](const std::size_t i) const = 0;
    };

    // template<typename T>
    // concept DynamicArrayType = std::is_unbounded_array_v<T>;

    // template<typename T>
    // concept UniquePtrArrayType = requires(T t) {
    //     typename T::element_type;
    //     { t.get() } -> std::same_as<typename T::element_type*>;
    // } && std::derived_from<T, std::unique_ptr<typename T::element_type[]>>;

    export template<typename T>
    class Array : public std::unique_ptr<T[]>, public IIterable<T>
    {
    public:
        using Base = std::unique_ptr<T[]>;

        Array() = default;
        Array(const std::size_t size) : Base(new T[size]), m_size{ size } {}
        Array(const std::size_t size, const T val) : Base(new T[size]), m_size{ size } 
        {
            std::fill(begin(), end(), val);
        }

        void clear() 
        {
            Base::reset();
            m_size = 0;
        }

        void resize(const std::size_t size, const T& val = T{}) 
        {
            T* newArray = new T[size];

            const std::size_t copySize = (size < m_size) ? size : m_size;
            std::move(begin(), begin()+copySize, newArray);

            if (size > copySize)
                std::fill(newArray+copySize, newArray+size, val);

            Base::reset(newArray);
            m_size = size;
        }

        T* begin() override { return Base::get(); }
        T* end() override { return Base::get() + m_size; }
        const T* begin() const override { return Base::get(); }
        const T* end() const override { return Base::get() + m_size; }

        constexpr std::size_t size() const override { return m_size; } 

        T* data() override { return Base::get(); }
        const T* data() const override { return Base::get(); }

        T& operator[](const std::size_t i) override { return Base::operator[](i); }
        const T& operator[](const std::size_t i) const override { return Base::operator[](i); }
        
    private:
        std::size_t m_size;

    };





    // template <typename T>
    // concept ContainerType = std::ranges::range<T> || UniquePtrArray<T>;

    // export template<UniquePtrArray T>
    // class ContainerWrapper : public T//, public IIterable<typename T::value_type>
    // {
    // public:
    //     using T::T;

    //     // std::unique_ptr<T> toArray() const
    //     // {
    //     //     std::unique_ptr<T[]> arr(new T[T::size()]);
    //     //     std::memcpy(arr.get(), T::data(), T::size()*sizeof(T));
    //     //     return arr;
    //     // }

    //     std::vector<T> toVector()
    //     {
    //         std::vector<T> vec(T::size());
    //         std::memcpy(vec.data(), T::data(), T::size()*sizeof(T));
    //         return vec;
    //     }

    //     // T::value_type* begin() override { return T::data(); }
    //     // T::value_type* end() override { return T::data() + T::size(); }
    //     // const T::value_type* begin() const override { return T::data(); }
    //     // const T::value_type* end() const override { return T::data() + T::size(); }

    //     // constexpr std::size_t size() const override { return T::size(); }

    //     // T::value_type* data() override { return T::data(); }
    //     // const T::value_type* data() const override { return T::data(); }

    //     // T::value_type& operator[](const std::size_t i) override { return T::operator[](i); }
    //     // const T::value_type& operator[](const std::size_t i) const override { return T::operator[](i); }
    // };

}

