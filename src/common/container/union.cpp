#include "union.hpp"


#ifdef TEST_UNIT

#include <catch.hpp>
#include <memory>


TEST_CASE("Union", "[union]") {
    SECTION("Primitive") {
        auto a = Union<bool, int, double>::create<1>(123);
        REQUIRE(a.size() == 3);

        REQUIRE(a.get<1>() == 123);
        a.get<1>() = -321;
        REQUIRE(a.take<1>() == -321);
    }
    SECTION("Move") {
        std::unique_ptr<int> ptr = std::make_unique<int>(123);
        auto a = Union<void, std::unique_ptr<int>>::create<1>(std::move(ptr));
        REQUIRE(*a.get<1>() == 123);
        ptr = a.take<1>();
        REQUIRE(*ptr == 123);
    }
    SECTION("Ctor Dtor") {
        std::shared_ptr<int> ptr = std::make_shared<int>(123);
        REQUIRE(ptr.use_count() == 1);
        auto a = Union<void, std::shared_ptr<int>>::create<1>(ptr);
        REQUIRE(ptr.use_count() == 2);
        REQUIRE(*a.get<1>() == 123);
        REQUIRE(ptr.use_count() == 2);
        {
            std::shared_ptr<int> cptr = a.take<1>();
            REQUIRE(*cptr == 123);
            REQUIRE(ptr.use_count() == 2);
        }
        REQUIRE(ptr.use_count() == 1);
    }
}

#endif