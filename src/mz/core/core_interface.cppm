export module mz.core.interface;

import std;

namespace mz {

    export class ICastable 
    {
    public:
        virtual ~ICastable() = default;

        template<class T>
        bool is() const {  return dynamic_cast<const T*>(this) != nullptr; }

        template<class T>
        T* asPtr() {  return dynamic_cast<T*>(this); }

        template<class T>
        const T* asPtr() const {  return dynamic_cast<const T*>(this); }

        template<class T>
        std::expected<std::reference_wrapper<T>, std::string> asRef()
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected("Invalid Type");
            return *casted;
        }

        template<class T>
        std::expected<std::reference_wrapper<const T>, std::string> asRef() const
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected("Invalid Type");
            return *casted;
        }

        template<class T>
        std::expected<T, std::string> asCopy() const
        {
            auto casted = asPtr<T>();
            if (!casted) return std::unexpected("Invalid Type");
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
        T asCopyUnchecked() const { return T(*asPtrUnchecked<T>()); }
    };

}

