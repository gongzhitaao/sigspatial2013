#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int bst(const std::vector<int> &a, int k)
{
    int b = 0, m = 0, e = a.size()-1;

    if (k >= a[e]) return -1;
    if (k <= a[0]) return 0;

    while (b < e) {
        m = (b+e)/2;
        if (a[m] > k) e = m;
        else b = m + 1;
    }

    return b;
}

TEST(bst, random)
{
    int size = 20;
    std::vector<int> a(size);
    unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch().count();
    std::mt19937 eng(seed);

    std::uniform_int_distribution<int> dis(0, 100);

    for (int i = 0; i < 100; ++i) {
        generate(a.begin(), a.end(), [&]{ return dis(eng); });

        sort(a.begin(), a.end());

        int key = dis(eng);
        int j = bst(a, key);

        EXPECT_TRUE(j > 0 ? (a[j-1] <= key && key <= a[j])
                    : 0 == j ? key <= a[j] : true)
            << "case " << i << ": "
            << key << ' ' << a[j] << std::endl;
    }
}
