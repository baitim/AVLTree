#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "avl.hpp"

struct AVLFixture : public testing::Test {
    avl_tree::avl_t<int> avl{INT32_MAX};

    void SetUp() override {
        for (int i = 0; i < 10; i++) 
            avl.insert(i);
    }
};

TEST(AVL_tree_main, test_simple)
{
    avl_tree::avl_t<int> avl{INT32_MAX};

    avl.insert(10);
    avl.insert(20);
    ASSERT_EQ(avl.check_range(8, 31), 2);
    ASSERT_EQ(avl.check_range(6, 9), 0);
    avl.insert(30);
    avl.insert(40);
    ASSERT_EQ(avl.check_range(15, 40), 3);
}

TEST(AVL_tree_main, test_copy_ctor)
{   
    avl_tree::avl_t<int> avl{INT32_MAX};
    
    if (1) {
        avl_tree::avl_t<int> avl2{INT32_MAX};
        for (int i = 0; i < 10; i++) 
            avl2.insert(i);
        avl = avl2;
    }

    EXPECT_EQ(avl.check_range(2, 3), 2);
    EXPECT_EQ(avl.check_range(8, 6), 0);
    EXPECT_EQ(avl.check_range(1, 9), 9);
}

TEST_F(AVLFixture, test_simple_borders)
{   
    EXPECT_EQ(avl.check_range(0, 3), 4);
    EXPECT_EQ(avl.check_range(2, 6), 5);
    EXPECT_EQ(avl.check_range(4, 9), 6);
}

TEST_F(AVLFixture, test_going_beyond_borders)
{   
    EXPECT_EQ(avl.check_range(-4, 3), 4);
    EXPECT_EQ(avl.check_range(2, 21), 8);
    EXPECT_EQ(avl.check_range(-26, 37), 10);
}

TEST_F(AVLFixture, test_repeats)
{
    for (int i = 0; i < 4; i++) 
        avl.insert(4);

    for (int i = 0; i < 5; i++) 
        avl.insert(5);
    
    ASSERT_EQ(avl.check_range(4, 5), 2);
}

TEST_F(AVLFixture, test_left_bigger_right)
{   
    EXPECT_EQ(avl.check_range(51, 10), 0);
    EXPECT_EQ(avl.check_range(3, 3),   0);
    EXPECT_EQ(avl.check_range(46, 7),  0);
}