#include <lib/3DArray.h>
#include <gtest/gtest.h>
#include <iostream>

// Matrix tests
TEST(MatrixTestSuite, DefaultTest) {
    Matrix test(10, 5, 8);
    
    EXPECT_EQ(test[0][0][5], 0);
}

TEST(MatrixTestSuite, IntConstructorTest) {
    Matrix test(10, 5, 8);
    
    test[4][2][3] = 35898;
    
    EXPECT_EQ(test[4][2][3], 35898);
}

TEST(MatrixTestSuite, AdditionTest) {
    Matrix first_array = Matrix::make_array(10, 2, 30);
    first_array[9][1][17] = 85;
    first_array[3][0][28] = 333;

    Matrix second_array = Matrix::make_array(10, 2, 30);
    second_array[9][1][17] = 34;
    second_array[3][0][28] = 195;

    Matrix third_array = first_array + second_array;

    ASSERT_EQ(third_array[9][1][17], 85 + 34);
    ASSERT_EQ(third_array[3][0][28], 333 + 195);
}

TEST(MatrixTestSuite, SubstractionTest) {
    Matrix first_array = Matrix::make_array(10, 2, 30);
    first_array[9][1][17] = 85;
    first_array[3][0][28] = 333;
    
    Matrix second_array = Matrix::make_array(10, 2, 30);
    second_array[9][1][17] = 34;
    second_array[3][0][28] = 195;

    Matrix third_array = first_array - second_array;

    ASSERT_EQ(third_array[9][1][17], 85 - 34);
    ASSERT_EQ(third_array[3][0][28], 333 - 195);
}

TEST(MatrixTestSuite, MultiplicationTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    array[9][1][17] = 85;
    array[3][0][28] = 333;

    Matrix twice_array = array * 2;

    ASSERT_EQ(twice_array[9][1][17], 85 * 2);
    ASSERT_EQ(twice_array[3][0][28], 333 * 2);
}

TEST(MatrixTestSuite, EqualityTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    array[9][1][17] = 100;

    EXPECT_TRUE(array[9][1][17] == 100);
    EXPECT_FALSE(array[9][1][17] == 200);
}

TEST(MatrixTestSuite, InequalityTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    array[9][1][17] = 100;

    EXPECT_TRUE(array[9][1][17] != 200);
    EXPECT_FALSE(array[9][1][17] != 100);
}

TEST(MatrixTestSuite, OutputTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    array[9][1][17] = 42;
    
    std::stringstream ss;
    ss << array[9][1][17];

    EXPECT_EQ(ss.str(), "42");
}

TEST(MatrixTestSuite, InputTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    
    std::stringstream ss("123");
    ss >> array[9][1][17];

    EXPECT_EQ(array[9][1][17], 123);
}

TEST(MatrixTestSuite, AssignmentOverflowTest) {
    Matrix a(2, 2, 2);

    a[0][0][0] = 131071 + 1;

    EXPECT_EQ(a[0][0][0], 0);
}

TEST(MatrixTestSuite, AdditionOverflowTest) {
    Matrix array = Matrix::make_array(10, 2, 30);
    
    array[5][0][28] = 131071;
    array[5][0][25] = 1;
    array[0][0][0] = array[5][0][28] + array[5][0][25];

    EXPECT_EQ(array[0][0][0], 0);
}

TEST(MatrixTestSuite, GivenTest) {
    Matrix array = Matrix::make_array(10, 2, 30);

    array[4][1][2] = 100500;
    
    ASSERT_EQ(array[4][1][2], 100500);
}

TEST(MatrixTestSuite, EmptyTest) {
    Matrix array = Matrix::make_array(2, 1, 2);
    
    ASSERT_EQ(array[0][0][0], 0);
    ASSERT_EQ(array[0][0][1], 0);
    ASSERT_EQ(array[1][0][0], 0);
    ASSERT_EQ(array[1][0][1], 0);
}

TEST(MatrixTestSuite, AssignmentTest) {
    Matrix first_array = Matrix::make_array(10, 2, 30);
    Matrix second_array = Matrix::make_array(10, 2, 30);
    
    first_array[1][1][1] = 50;
    
    second_array = first_array;
    
    EXPECT_EQ(first_array[1][1][1], 50);
    EXPECT_EQ(second_array[1][1][1], 50);
}

TEST(MatrixTestSuite, CopyingTest) {
    Matrix first_array = Matrix::make_array(10, 2, 30);
    Matrix copying_array = Matrix::make_array(10, 2, 30);
    first_array[1][1][28] = 155;
    
    copying_array = first_array;
    copying_array[1][1][28] = 89;
    
    EXPECT_EQ(first_array[1][1][28], 155);
    EXPECT_EQ(copying_array[1][1][28], 89);
}

TEST(MatrixTestSuite, DifferentArraySizesTest) {
    Matrix first_array = Matrix::make_array(10, 2, 30);
    Matrix second_array = Matrix::make_array(6, 19, 1);
    
    EXPECT_THROW({
        try {
            Matrix result = first_array - second_array;
        } catch (const std::invalid_argument& e) {
            EXPECT_STREQ(e.what(), "Different array sizes\n");
            throw;
        }
    }, std::invalid_argument);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
