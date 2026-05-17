// =============================================================================
// fast_math_test.c
//
// Tier-1 native tests for the saturating-arithmetic helpers in
// Library/fast_math.h (qadd8, qsub8, sqrt16). The helpers are header-only
// `static inline` functions — the test file includes the header directly
// and exercises the math without any platform surface.
//
// These helpers drive ARGB.c's HSV blending math (e.g. ARGB.c:621 sqrt16 in
// HSV-to-RGB, :659 qadd8 in saturation overflow protection); a regression
// in their saturating behavior corrupts every LED frame on a bpm44 robot.
// =============================================================================

#include <test_deps.h>

#include "../fast_math.h"

TEST_GROUP(fast_math);

TEST_SETUP(fast_math)    { /* helpers are stateless */ }
TEST_TEAR_DOWN(fast_math) { }

// -----------------------------------------------------------------------------
// qadd8: clamp i+j at 0xFF.
// -----------------------------------------------------------------------------
TEST(fast_math, qadd8_zero_plus_zero_is_zero)
{
    TEST_ASSERT_EQUAL_UINT8(0, qadd8(0, 0));
}

TEST(fast_math, qadd8_below_saturation_is_normal_addition)
{
    TEST_ASSERT_EQUAL_UINT8(100, qadd8(40, 60));
}

TEST(fast_math, qadd8_at_exactly_255_does_not_clamp)
{
    // 200 + 55 == 255 → no saturation; exact value returned.
    TEST_ASSERT_EQUAL_UINT8(255, qadd8(200, 55));
}

TEST(fast_math, qadd8_overflows_clamp_to_255)
{
    TEST_ASSERT_EQUAL_UINT8(255, qadd8(200, 100));   // 300 → 255
    TEST_ASSERT_EQUAL_UINT8(255, qadd8(255, 1));     // 256 → 255
    TEST_ASSERT_EQUAL_UINT8(255, qadd8(255, 255));   // 510 → 255
}

// -----------------------------------------------------------------------------
// qsub8: clamp i-j at 0.
// -----------------------------------------------------------------------------
TEST(fast_math, qsub8_zero_minus_zero_is_zero)
{
    TEST_ASSERT_EQUAL_UINT8(0, qsub8(0, 0));
}

TEST(fast_math, qsub8_above_floor_is_normal_subtraction)
{
    TEST_ASSERT_EQUAL_UINT8(20, qsub8(50, 30));
    TEST_ASSERT_EQUAL_UINT8(0,  qsub8(50, 50));   // exact zero — not clamped
}

TEST(fast_math, qsub8_underflow_clamps_to_zero)
{
    TEST_ASSERT_EQUAL_UINT8(0, qsub8(0, 1));
    TEST_ASSERT_EQUAL_UINT8(0, qsub8(10, 50));
    TEST_ASSERT_EQUAL_UINT8(0, qsub8(0, 255));
}

// -----------------------------------------------------------------------------
// sqrt16: 16-bit integer square root, returning floor(sqrt(x)) clamped at 255.
// Per FastLED's implementation, x in [0..1] returns x itself; otherwise
// binary search bounded by hi = (x>>5)+8 for x≤7904, else hi=255.
// -----------------------------------------------------------------------------
TEST(fast_math, sqrt16_zero_returns_zero)
{
    TEST_ASSERT_EQUAL_UINT8(0, sqrt16(0));
}

TEST(fast_math, sqrt16_one_returns_one_via_early_return)
{
    // The `if (x <= 1) return x` shortcut.
    TEST_ASSERT_EQUAL_UINT8(1, sqrt16(1));
}

TEST(fast_math, sqrt16_perfect_squares_return_exact_root)
{
    TEST_ASSERT_EQUAL_UINT8(2,   sqrt16(4));
    TEST_ASSERT_EQUAL_UINT8(3,   sqrt16(9));
    TEST_ASSERT_EQUAL_UINT8(10,  sqrt16(100));
    TEST_ASSERT_EQUAL_UINT8(16,  sqrt16(256));
    TEST_ASSERT_EQUAL_UINT8(100, sqrt16(10000));
}

TEST(fast_math, sqrt16_non_perfect_squares_return_floor)
{
    // 5 is between 4 (=2²) and 9 (=3²) → floor(sqrt(5)) = 2.
    TEST_ASSERT_EQUAL_UINT8(2,   sqrt16(5));
    TEST_ASSERT_EQUAL_UINT8(2,   sqrt16(8));
    // 50 is between 49 (=7²) and 64 (=8²) → floor = 7.
    TEST_ASSERT_EQUAL_UINT8(7,   sqrt16(50));
    // 9999 is between 9801 (=99²) and 10000 (=100²) → floor = 99.
    TEST_ASSERT_EQUAL_UINT8(99,  sqrt16(9999));
}

TEST(fast_math, sqrt16_at_branch_boundary_x_eq_7904)
{
    // The `x > 7904` branch picks hi=255; x≤7904 picks hi=(x>>5)+8.
    // Test exactly at the boundary.  88² = 7744, 89² = 7921 → floor(sqrt(7904)) = 88.
    TEST_ASSERT_EQUAL_UINT8(88, sqrt16(7904));
}

TEST(fast_math, sqrt16_above_branch_boundary_uses_255_hi)
{
    // x = 8000 > 7904 → hi=255 path.  89² = 7921, 90² = 8100 → floor = 89.
    TEST_ASSERT_EQUAL_UINT8(89, sqrt16(8000));
}

TEST(fast_math, sqrt16_max_uint16_clamps_at_255)
{
    // sqrt(65535) = 255.99... → 255 (also the explicit "if mid == 255 return 255" guard).
    TEST_ASSERT_EQUAL_UINT8(255, sqrt16(65535));
    TEST_ASSERT_EQUAL_UINT8(255, sqrt16(65025));   // exactly 255²
}

TEST(fast_math, sqrt16_just_below_max_returns_254)
{
    // 254² = 64516; 255² = 65025. sqrt16(65024) → 254.
    TEST_ASSERT_EQUAL_UINT8(254, sqrt16(65024));
}

TEST_GROUP_RUNNER(fast_math)
{
    RUN_TEST_CASE(fast_math, qadd8_zero_plus_zero_is_zero);
    RUN_TEST_CASE(fast_math, qadd8_below_saturation_is_normal_addition);
    RUN_TEST_CASE(fast_math, qadd8_at_exactly_255_does_not_clamp);
    RUN_TEST_CASE(fast_math, qadd8_overflows_clamp_to_255);
    RUN_TEST_CASE(fast_math, qsub8_zero_minus_zero_is_zero);
    RUN_TEST_CASE(fast_math, qsub8_above_floor_is_normal_subtraction);
    RUN_TEST_CASE(fast_math, qsub8_underflow_clamps_to_zero);
    RUN_TEST_CASE(fast_math, sqrt16_zero_returns_zero);
    RUN_TEST_CASE(fast_math, sqrt16_one_returns_one_via_early_return);
    RUN_TEST_CASE(fast_math, sqrt16_perfect_squares_return_exact_root);
    RUN_TEST_CASE(fast_math, sqrt16_non_perfect_squares_return_floor);
    RUN_TEST_CASE(fast_math, sqrt16_at_branch_boundary_x_eq_7904);
    RUN_TEST_CASE(fast_math, sqrt16_above_branch_boundary_uses_255_hi);
    RUN_TEST_CASE(fast_math, sqrt16_max_uint16_clamps_at_255);
    RUN_TEST_CASE(fast_math, sqrt16_just_below_max_returns_254);
}

TEST_RUNNER(fast_math)
