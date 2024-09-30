#include <iostream>
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "avl.hpp"

std::vector<std::string> get_sorted_files(std::filesystem::path path) {
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(path))
        files.push_back(entry.path().string());

    std::sort(files.begin(), files.end());
    return files;
}

TEST(AVL_end_to_end, cmp_with_set) 
{
    std::filesystem::path dir = "../../../tests/end_to_end/";
    std::filesystem::path answers_avl_path = dir / "answers_avl/";
    std::filesystem::path answers_set_path = dir / "answers_set/";

    std::vector<std::string> answers_avl_str = get_sorted_files(answers_avl_path);
    std::vector<std::string> answers_set_str = get_sorted_files(answers_set_path);
    const int count_tests = std::min(answers_avl_str.size(), answers_set_str.size());

    for (int i = 0, count; i < count_tests; ++i) {
        std::ifstream answer_avl_file(answers_avl_str[i]);
        std::vector<int> ans_avl;
        while (answer_avl_file >> count)
            ans_avl.push_back(count);
        answer_avl_file.close();

        std::ifstream answer_set_file(answers_set_str[i]);
        std::vector<int> ans_set;
        while (answer_set_file >> count)
            ans_set.push_back(count);
        answer_set_file.close();

        EXPECT_EQ(ans_avl.size(), ans_set.size());
        for (int j = 0, end = ans_avl.size(); j < end; ++j)
            EXPECT_EQ(ans_avl[j], ans_set[j]);
    }
}

struct AVLFixture : public testing::Test {
    const int size = 10;
    avl_tree::avl_t<int> avl{INT32_MAX};

    void SetUp() override {
        for (int i = 0; i < size; i++)
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