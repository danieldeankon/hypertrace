#include "linear.hh"


Linear3 lin3_identity() {
    return r44_one();
}

Linear3 lin3_look_to(real3 dir) {
    if (2*fabs(dir.z) < 1) {
        return lin3_look_to_up(dir, r3_new(0,0,1));
    } else {
        return lin3_look_to_up(dir, r3_new(0,1,0));
    }
}

Linear3 lin3_look_to_up(real3 dir, real3 up) {
    real3 right = normalize(cross(dir, up));
    real3 down = cross(dir, right);
    return Linear3(
        r4_new(right, R0),
        r4_new(down, R0),
        r4_new(dir, R0),
        r4_new(R0)
    );
}

real3 lin3_apply(Linear3 m, real3 v) {
    return r33_dot_mv(m, v);
}

Linear3 lin3_chain(Linear3 a, Linear3 b) {
    return r33_dot(a, b);
}

Linear3 lin3_inverse(Linear3 m) {
    return r33_inverse(m);
}

Linear3 interpolate(Linear3 a, Linear3 b, real t) {
    // FIXME: Use matrix power operation
    return (R1 - t)*a + t*b;
}


#ifdef UNIT_TEST

#include <catch.hpp>


TEST_CASE("Linear transformation", "[linear]") {
    TestRng<real> rng(0xABBA);
    TestRng<real3> vrng(0xBAAB);
    TestRngReal3x3 mrng(0xBEEB);

    SECTION("Linearity") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            Linear3 m = mrng.normal();
            real a = rng.normal();
            real3 x = vrng.normal();

            REQUIRE(lin3_apply(a*m, x) == approx(lin3_apply(m, a*x)));
            REQUIRE(lin3_apply(a*m, x) == approx(a*lin3_apply(m, x)));
        }
    }
    SECTION("Chaining") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            Linear3 a = mrng.normal();
            Linear3 b = mrng.normal();
            real3 c = vrng.normal();

            REQUIRE(lin3_chain(a, lin3_identity()) == approx(a));
            REQUIRE(lin3_chain(lin3_identity(), b) == approx(b));
            REQUIRE(lin3_apply(lin3_chain(a, b), c) == approx(lin3_apply(a, lin3_apply(b, c))));
        }
    }
    SECTION("Inversion") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            Linear3 a = mrng.invertible();
            real3 x = vrng.normal();

            REQUIRE(lin3_chain(a, lin3_inverse(a)) == approx(lin3_identity()));
            REQUIRE(lin3_chain(lin3_inverse(a), a) == approx(lin3_identity()));
            REQUIRE(lin3_apply(lin3_inverse(a), lin3_apply(a, x)) == approx(x));
            REQUIRE(lin3_apply(a, lin3_apply(lin3_inverse(a), x)) == approx(x));
        }
    }
    SECTION("Look to the direction") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            real3 d = vrng.unit();
            Linear3 m = lin3_look_to(d);

            REQUIRE(lin3_apply(m, d) == approx(r3_new(R0, R0, R1)));
        }
    }
};

#endif // UNIT_TEST