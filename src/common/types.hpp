#pragma once

#include <types.h>

static_assert(sizeof(char) == 1, "Primitive type size check error");
static_assert(sizeof(short) == 2, "Primitive type size check error");
static_assert(sizeof(int) == 4, "Primitive type size check error");
static_assert(sizeof(long) == 8, "Primitive type size check error");

static_assert(sizeof(uchar) == 1, "Primitive type size check error");
static_assert(sizeof(ushort) == 2, "Primitive type size check error");
static_assert(sizeof(uint) == 4, "Primitive type size check error");
static_assert(sizeof(ulong) == 8, "Primitive type size check error");

static_assert(sizeof(float) == 4, "Primitive type size check error");
#if defined(HOST) || defined(DEVICE_DOUBLE)
static_assert(sizeof(double) == 8, "Primitive type size check error");
#endif


template <typename T, typename S>
struct pair {
    T first;
    S second;
    pair() = default;
    pair(T f, S s) : first(f), second(s) {}
};
template <typename T, typename S>
constexpr pair<T, S> make_pair(T f, S s) {
    return pair<T, S>(f, s);
}