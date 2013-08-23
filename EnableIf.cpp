#include <type_traits>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "idioms.h"

template <bool, class T = void>
struct enable_if
{
};

template <class T>
struct enable_if < true, T>
{
    typedef T type;
};

struct SimpleStruct
{
    int x;
};

struct NonPodStruct
{

    NonPodStruct(int v)
    : x(v)
    {
    }
    int x;
};

/*
template<typename _T>
_T* copy(const _T& origin, typename enable_if<std::is_pod<_T>::value>::type* = nullptr)
{
    std::cout << "POD" << std::endl;
    _T* p = reinterpret_cast<_T*> (malloc(sizeof (_T)));
    memcpy(p, &origin, sizeof (_T));
    return p;
}

template<typename _T>
_T* copy(const _T& origin, typename enable_if<!std::is_pod<_T>::value && std::is_copy_constructible<_T>::value>::type* = nullptr)
{
    std::cout << "Copy constructor" << std::endl;
    return new _T(origin);
}
*/

template<class _T>
typename enable_if<std::is_pod<_T>::value, _T*>::type copy(const _T& origin)
{
    std::cout << "POD" << std::endl;
    _T* p = reinterpret_cast<_T*> (malloc(sizeof (_T)));
    memcpy(p, &origin, sizeof (_T));
    return p;
}

template<class _T>
typename enable_if<!std::is_pod<_T>::value && std::is_copy_constructible<_T>::value, _T*>::type copy(const _T& origin)
{
    std::cout << "Copy constructor" << std::endl;
    return new _T(origin);
}

void testEnableIf()
{
    SimpleStruct s;
    s.x = 10;

    SimpleStruct* pCopy = copy(s);

    NonPodStruct np(2);
    NonPodStruct* pNonCopy = copy(np);

}