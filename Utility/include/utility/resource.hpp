
#pragma once

#include "utility/utility.hpp"

#include <typeinfo>
#include <iostream>

/// RAII wrapper around any closeable resource
template <typename T, template <typename> typename CreationPolicy, template <typename> typename DestructionPolicy>
class Resource
{
private:
    T *ptr;
public:
    template<typename... Args>
    Resource(Args&&... args) : ptr{CreationPolicy<T>::create(std::forward<Args>(args)...)}
    { }

    Resource(Resource const &) = delete;

    Resource &operator=(Resource const &) = delete;

    ~Resource()
    {
        if (ptr) DestructionPolicy<T>::destroy(ptr);
        ptr = nullptr;
    }
};

template <typename T>
struct FbxDestructionPolicy
{
    static void destroy(T *ptr) {
        ptr->Destroy();
    }
};

template <typename T>
struct FbxCreationPolicy
{
    template<typename... Args>
    static T *create(Args&&... args) {
        return nullThrow(T::Create(std::forward<Args>(args)...), std::string("FbxCreationPolicy::create failed for type ") + typeid(T).name());
    }
};

/// Resource specialised for FBX SDK objects
template <typename T>
class FbxResource : public Resource<T, FbxCreationPolicy, FbxDestructionPolicy>
{ };
