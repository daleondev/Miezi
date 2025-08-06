export module mz.core.behaviours;

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
        virtual const T* cbegin() const = 0;
        virtual const T* cend() const = 0;
    };

}

