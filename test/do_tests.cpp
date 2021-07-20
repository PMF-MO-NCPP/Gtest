#include "../src/mylib.h"
#include "gtest/gtest.h"
#include <cmath>

// testiranje gcd funkcije na negativne brojeve
TEST(GCDTest, Negative) {
  EXPECT_EQ(1, gcd(-5,-3));
  EXPECT_EQ(2, gcd(-4, -2));
  EXPECT_GT(gcd(-10,-5), 1);  // gcd(-10,-5) > 1
}

// testiranje gcd funkcije na nulu
TEST(GCDTest, Zero) {
  EXPECT_EQ(10, gcd(0,10));
  EXPECT_EQ(10, gcd(10,0));
  EXPECT_THROW(gcd(0,0), std::runtime_error); // Očekujemo da izbaci runtime_error.
}

// testiranje gcd funkcije na pozitivne brojeve
TEST(GCDTest, Positive) {
  EXPECT_EQ(1, gcd(1,3));
  EXPECT_EQ(8, gcd(16,8));
  EXPECT_EQ(9, gcd(27,9));
}

// testiranje C-stringova
TEST(stripTest, basic){
    char * rez = strip(" xyz ");
    EXPECT_STREQ(rez, "xyz");
    delete [] rez;

    char * rez1 = strip("xyzw ");
    EXPECT_STREQ(rez1, "xyzw");
    delete [] rez1;

    char * rez2 = strip("xyzw");
    EXPECT_STREQ(rez2, "xyzw");
    delete [] rez2;

}

// testiranje realnih brojeva
TEST(SqrtTest, basic){
    double x = Sqrt(2.0);
    double y = std::sqrt(2.0);
    ASSERT_DOUBLE_EQ(x,y);
    x = Sqrt(375.0);
    y = std::sqrt(375.0);
    ASSERT_DOUBLE_EQ(x,y);
    ASSERT_NEAR(x,y, 1E-14);
    EXPECT_ANY_THROW(Sqrt(-1.0));
    EXPECT_NO_THROW(Sqrt(-1.0));
}

// testiranje predikata
bool isCoprime(long a, long b) { return gcd(a,b) == 1; }

TEST(predTest, basic){
    EXPECT_PRED2(isCoprime, 3, 5); // o.k.
    long a = 6, b = 12;
    EXPECT_PRED2(isCoprime, a, b); // greška
}

testing::AssertionResult isCoprime1(long a, long b) {
        if(gcd(a,b) == 1)
            return testing::AssertionSuccess();
        else
            return testing::AssertionFailure();
}

TEST(predTest, advanced){
    EXPECT_PRED2(isCoprime1, 3, 5) << " o.k." << 3 << " i " << 5 << " su relativno prosti";
    long a = 6, b = 12;
    EXPECT_PRED2(isCoprime1, a, b) << a << " i " << b << " nisu relativno prosti"; 
}

