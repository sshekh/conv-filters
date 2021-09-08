/*
 * Build obj linking with cc_library NDGrid Class.
 *
 * Copyright Bazel organization
 *
 */

#ifndef CH04_SIMPLENET_MAIN_NDGRID_H_
#define CH04_SIMPLENET_MAIN_NDGRID_H_

#include <vector>
#include <optional>
#include <cstddef>

template <typename T, size_t N>
class NDGrid
{
public:
    explicit NDGrid(size_t size = kDefaultSize);
    virtual ~NDGrid() = default;

    NDGrid<T, N-1>& operator[](size_t x);
    const NDGrid<T, N-1>& operator[](size_t x) const;

    void resize(size_t newSize);
    size_t getSize() const { return mElements.size(); }

    static const size_t kDefaultSize = 512;

private:
    std::vector<NDGrid<T, N-1>> mElements;
};


template <typename T>
class NDGrid<T, 1>
{
public:
    explicit NDGrid(size_t size = kDefaultSize);
    virtual ~NDGrid() = default;

    T& operator[](size_t x);
    const T& operator[](size_t x) const;

    void resize(size_t newSize);
    size_t getSize() const { return mElements.size(); }

    static const size_t kDefaultSize = 512;

private:
    std::vector<T> mElements;
};

#include "NDGrid.cc"

#endif  // CH04_SIMPLENET_MAIN_NDGRID_H_
