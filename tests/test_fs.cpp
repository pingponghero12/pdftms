#include <gtest/gtest.h>
#include "fs.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FsTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir = fs::temp_directory_path() / "pdftms_test";
        fs::create_directories(test_dir);
        fs::current_path(test_dir);

        test_file = test_dir / "test.pdf";
        std::ofstream(test_file).put('x');

        test_subdir = test_dir / "subdir";
        fs::create_directories(test_subdir);
    }

    void TearDown() override {
        fs::remove_all(test_dir);
    }

    fs::path test_dir;
    fs::path test_file;
    fs::path test_subdir;
};

// Tests for get_src_path
TEST_F(FsTest, GetSrcPathExistingFile) {
    auto result = get_src_path("test.pdf");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->filename(), "test.pdf");
}

TEST_F(FsTest, GetSrcPathNonExistentFile) {
    auto result = get_src_path("nonexistent.pdf");
    EXPECT_FALSE(result.has_value());
}

// Tests for get_dest
TEST_F(FsTest, GetDestExistingDirectory) {
    auto result = get_dest(test_subdir.string(), true);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, fs::canonical(test_subdir));
}

TEST_F(FsTest, GetDestNonExistentDirectory) {
    auto result = get_dest((test_dir / "nonexistent").string(), true);
    EXPECT_FALSE(result.has_value());
}

TEST_F(FsTest, GetDestFileAsDirectory) {
    auto result = get_dest(test_file.string(), true);
    EXPECT_FALSE(result.has_value());
}

TEST_F(FsTest, GetDestRelativePath) {
    fs::current_path(test_dir);
    auto result = get_dest("subdir", true);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, fs::canonical(test_subdir));
}

// Tests for get_dest_path
TEST_F(FsTest, GetDestPathValidDirectory) {
    auto result = get_dest_path(test_subdir.string(), "new.pdf");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->filename(), "new.pdf");
    EXPECT_EQ(result->parent_path(), fs::canonical(test_subdir));
}

TEST_F(FsTest, GetDestPathInvalidDirectory) {
    auto result = get_dest_path((test_dir / "nonexistent").string(), "new.pdf");
    EXPECT_FALSE(result.has_value());
}
