#include <cstddef>

#include <set>
#include <stdexcept>
#include <utility>

#include <gtest/gtest.h>

#include "alg/divide_and_conquer/strassens_matrix_multiplication.h"
#include "alg/numeric/matrix.h"

namespace algtest {

TEST(StrassensMatrix, Multiplication) {
    constexpr int n = 8;

    alg::numeric::Matrix<int> A = {
        {269, 499, 659, 7, 71, 647, 569, 577},
        {599, 607, 61, 277, 563, 241, 353, 631},
        {467, 461, 419, 541, 373, 151, 421, 181},
        {13, 547, 347, 367, 397, 239, 389, 439},
        {317, 223, 37, 571, 47, 673, 227, 457},
        {509, 521, 11, 2, 683, 641, 307, 197},
        {67, 173, 271, 619, 349, 587, 293, 593},
        {89, 337, 557, 19, 199, 229, 491, 163}
    };

    alg::numeric::Matrix<int> B = {
        {257, 449, 661, 59, 167, 31, 109, 677},
        {41, 73, 383, 97, 281, 601, 29, 127},
        {433, 43, 709, 79, 83, 101, 617, 331},
        {431, 263, 193, 503, 613, 131, 463, 719},
        {487, 107, 379, 311, 701, 283, 211, 409},
        {157, 139, 103, 251, 191, 17, 53, 3},
        {401, 313, 359, 643, 23, 113, 5, 137},
        {149, 523, 479, 233, 653, 691, 179, 443}
    };

    int expected[n][n] = {
        {828254, 764784, 1411712, 804642, 807346, 869810, 609036, 833192},
        {872220, 922978, 1392306, 847956, 1306320, 1065160, 495062, 1260850},
        {954666, 690972, 1281492, 844274, 992130, 685498, 694488, 1193244},
        {786458, 584264, 1059960, 801710, 1029210, 875994, 578260, 867860},
        {640404, 719014, 818910, 766452, 933828, 588568, 456746, 920518},
        {743517, 628875, 1073633, 699049, 970459, 705277, 293187, 826439},
        {876416, 737946, 1004118, 935944, 1212476, 766128, 678486, 1049434},
        {640104, 385566, 939834, 564560, 468408, 492368, 457698, 522634}
    };

    alg::numeric::Matrix<int> C = alg::dnc::strassens_matrix_multiply(A, B);
    std::pair<std::size_t, std::size_t> d = C.dimension();

    ASSERT_EQ(n, d.first);
    ASSERT_EQ(n, d.second);

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            ASSERT_EQ(expected[i][j], C[i][j]);
        }
    }
}

TEST(StrassensMatrix, ValidateEqualDimension) {
    alg::numeric::Matrix<int> A(4, 4);
    alg::numeric::Matrix<int> B(8, 8);

    ASSERT_THROW(alg::dnc::strassens_matrix_multiply(A, B), std::invalid_argument);
}

TEST(StrassensMatrix, ValidateSquareMatrix) {
    alg::numeric::Matrix<int> A(4, 8);
    alg::numeric::Matrix<int> B(2, 4);

    ASSERT_THROW(alg::dnc::strassens_matrix_multiply(A, B), std::invalid_argument);
    ASSERT_THROW(alg::dnc::strassens_matrix_multiply(B, A), std::invalid_argument);
}

TEST(StrassensMatrix, ValidatePowerOfTwo) {
    using alg::dnc::strassens_matrix_multiply;

    std::set<std::size_t> powers = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

    for (std::size_t i = 1; i <= 512; ++i) {
        alg::numeric::Matrix<int> A(i, i);

        if (powers.find(i) == powers.end()) {
            ASSERT_THROW(strassens_matrix_multiply(A, A), std::invalid_argument);
        } else {
            ASSERT_NO_THROW(strassens_matrix_multiply(A, A));
        }
    }
}

}
