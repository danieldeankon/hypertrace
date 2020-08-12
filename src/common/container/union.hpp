#pragma once

#include <cassert>

#include "utils.hpp"


// Union for types with non-trivial ctors/dtors
template <typename ...Elems>
class Union final {
private:
    struct __attribute__((aligned(container::common_align<Elems...>()))) {
        char bytes[container::common_size<Elems...>()];
    } data;
#ifdef DEBUG
    bool stored_ = false;
#endif // DEBUG

public:
    Union() = default;

    Union(const Union &) = delete;
    Union &operator=(const Union &) = delete;

    Union(Union &&u) :
        data(u.data)
#ifdef DEBUG
        ,stored_(u.stored_)
#endif // DEBUG
    {
#ifdef DEBUG
        u.stored_ = false;
#endif // DEBUG
    }
    Union &operator=(Union &&u) {
#ifdef DEBUG
        assert(!this->stored_);
        this->stored_ = u.stored_;
        u.stored_ = false;
#endif // DEBUG
        this->data = u.data;
    }

    ~Union() {
#ifdef DEBUG
        assert(!this->stored_);
#endif // DEBUG
    }

    static constexpr size_t size() {
        return sizeof...(Elems);
    }
#ifdef DEBUG
    bool stored() const {
        return this->stored_;
    }
#endif // DEBUG

    template <size_t P>
    void put(container::nth_type<P, Elems...> &&x) {
#ifdef DEBUG
        assert(!this->stored_);
        this->stored_ = true;
#endif // DEBUG
        new (reinterpret_cast<container::nth_type<P, Elems...> *>(&this->data))
            container::nth_type<P, Elems...>(std::move(x));
    }
    template <size_t P>
    void put(const container::nth_type<P, Elems...> &x) {
        container::nth_type<P, Elems...> cx(x);
        this->template put<P>(std::move(cx));
    }

    template <size_t P>
    const container::nth_type<P, Elems...> &get() const {
#ifdef DEBUG
        assert(this->stored_);
#endif // DEBUG
        return *reinterpret_cast<const container::nth_type<P, Elems...> *>(&this->data);
    }
    template <size_t P>
    container::nth_type<P, Elems...> &get() {
#ifdef DEBUG
        assert(this->stored_);
#endif // DEBUG
        return *reinterpret_cast<container::nth_type<P, Elems...> *>(&this->data);
    }

    template <size_t P>
    static Union create(container::nth_type<P, Elems...> &&x) {
        Union u;
        u.put<P>(std::move(x));
        return u;
    }
    template <size_t P>
    static Union create(const container::nth_type<P, Elems...> &x) {
        container::nth_type<P, Elems...> cx(x);
        return create<P>(std::move(cx));
    }

    template <size_t P>
    container::nth_type<P, Elems...> take() {
#ifdef DEBUG
        assert(this->stored_);
        this->stored_ = false;
#endif // DEBUG
        return std::move(*reinterpret_cast<container::nth_type<P, Elems...> *>(&this->data));
    }
    template <size_t P>
    void destroy() {
        this->template take<P>();
    }
};