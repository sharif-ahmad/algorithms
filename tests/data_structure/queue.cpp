#include <array>

#include <gtest/gtest.h>

#include "alg/common/exception.h"
#include "alg/data_structure/queue.h"

namespace algtest {

TEST(Queue, Empty) {
    alg::ds::Queue<int> q(2);

    ASSERT_THROW(q.dequeue(), alg::except::BufferEmpty);
}

TEST(Queue, Full) {
    alg::ds::Queue<int> q(2);

    ASSERT_NO_THROW(q.enqueue(2));
    ASSERT_NO_THROW(q.enqueue(3));
    ASSERT_THROW(q.enqueue(5), alg::except::BufferFull);
}

TEST(Queue, EnqueueDequeue) {
    constexpr int n = 10;
    std::array<int, n> data = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    alg::ds::Queue<int> q(n - 2);

    ASSERT_EQ(0, q.count());

    for (int i = 0; i < n / 2; ++i) {
        q.enqueue(data[i]);
    }

    ASSERT_EQ(n / 2, q.count());

    for (int i = 0; i < n / 5; ++i) {
        ASSERT_EQ(data[i], q.dequeue());
    }

    ASSERT_EQ(n / 2 - n / 5, q.count());

    for (int i = n / 2; i < n; ++i) {
        q.enqueue(data[i]);
    }

    ASSERT_EQ(n - 2, q.count());
    ASSERT_THROW(q.enqueue(31), alg::except::BufferFull);

    for (int i = n / 5; i < n; ++i) {
        ASSERT_EQ(data[i], q.dequeue());
    }

    ASSERT_EQ(0, q.count());
    ASSERT_THROW(q.dequeue(), alg::except::BufferEmpty);
}

}
