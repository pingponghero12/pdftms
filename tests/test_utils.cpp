#include <gtest/gtest.h>
#include "utils.hpp"

#include <cstdlib>

// Save Home and then change it temporarly
class UtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        original_home = std::getenv("HOME");
        setenv("HOME", "/test/home", 1);
    }

    void TearDown() override {
        if (original_home) {
            setenv("HOME", original_home, 1);
        } else {
            unsetenv("HOME");
        }
    }

    const char* original_home;
};

// Tests for expand_tilde
TEST_F(UtilsTest, ExpandTildeWithHome) {
    EXPECT_EQ(expand_tilde("~/documents"), "/test/home/documents");
}

TEST_F(UtilsTest, ExpandTildeWithoutTilde) {
    EXPECT_EQ(expand_tilde("/usr/local"), "/usr/local");
}

TEST_F(UtilsTest, ExpandTildeEmpty) {
    EXPECT_EQ(expand_tilde(""), "");
}

TEST_F(UtilsTest, ExpandTildeMiddleTilde) {
    EXPECT_EQ(expand_tilde("/usr/~/local"), "/usr/~/local");
}

// Tests for rtrim
TEST(RtrimTest, TrimWhitespace) {
    EXPECT_EQ(rtrim("hello   "), "hello");
}

TEST(RtrimTest, TrimTabs) {
    EXPECT_EQ(rtrim("hello\t\t"), "hello");
}

TEST(RtrimTest, TrimNewlines) {
    EXPECT_EQ(rtrim("hello\n\n"), "hello");
}

TEST(RtrimTest, NoTrim) {
    EXPECT_EQ(rtrim("hello"), "hello");
}

TEST(RtrimTest, EmptyString) {
    EXPECT_EQ(rtrim(""), "");
}

TEST(RtrimTest, OnlyWhitespace) {
    EXPECT_EQ(rtrim("   "), "");
}
