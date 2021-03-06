#pragma once

#include "complex.hh"


typedef real4 quaternion;
#define make_quaternion make_real4
#define q_new make_quaternion

#ifdef OPENCL_INTEROP
typedef real4_pk quaternion_pk;
#define pack_quaternion pack_real4
#define unpack_quaternion unpack_real4
#define q_pack pack_quaternion
#define q_unpack unpack_quaternion
#endif // OPENCL_INTEROP

#define Q0 q_new(R0, R0, R0, R0)
#define Q1 q_new(R1, R0, R0, R0)
#define QI q_new(R0, R1, R0, R0)
#define QJ q_new(R0, R0, R1, R0)
#define QK q_new(R0, R0, R0, R1)


quaternion q_conj(quaternion a);
real q_abs2(quaternion a);
real q_abs(quaternion a);


quaternion q_mul(quaternion a, quaternion b);
quaternion qc_mul(quaternion a, complex b);
quaternion cq_mul(complex a, quaternion b);

quaternion q_inverse(quaternion a);
quaternion q_div(quaternion a, quaternion b);
quaternion qc_div(quaternion a, complex b);
quaternion cq_div(complex a, quaternion b);


#ifdef UNIT_TEST
quaternion rand_q_normal(TestRng &rng);
quaternion rand_q_nonzero(TestRng &rng);
quaternion rand_q_unit(TestRng &rng);
#endif // UNIT_TEST
