/*
 * Build obj linking with cc_library NDGrid Class.
 *
 * Copyright Bazel organization
 *
 */

#include <stdexcept>
#include <utility>
#include <random>

template <typename T, size_t N>
NDGrid<T, N>::NDGrid(size_t size)
{
    resize(size);
}

template <typename T, size_t N>
void NDGrid<T, N>::resize(size_t newSize)
{
    mElements.resize(N == 3 ? newSize : kDefaultSize);

    // Resizing the vector calls the 0-argument constructor for
    // the NDGrid<T, N-1> elements, which constructs
    // them with the default size. Thus, we must explicitly call
    // resize() on each of the elements to recursively resize all
    // nested Grid elements.
    for (auto& element : mElements) {
        element.resize(kDefaultSize);
    }
}

template <typename T, size_t N>
NDGrid<T, N-1>& NDGrid<T, N>::operator[](size_t x)
{
    return mElements[x];
}

template <typename T, size_t N>
const NDGrid<T, N-1>& NDGrid<T, N>::operator[](size_t x) const
{
    return mElements[x];
}


template <typename T>
NDGrid<T, 1>::NDGrid(size_t size)
{
    resize(size);
}

template <typename T>
void NDGrid<T, 1>::resize(size_t newSize)
{
    mElements.resize(newSize);
}

template <typename T>
T& NDGrid<T, 1>::operator[](size_t x)
{
    return mElements[x];
}

template <typename T>
const T& NDGrid<T, 1>::operator[](size_t x) const
{
    return mElements[x];
}
