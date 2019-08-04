#include "log2hist.cc"
#include <gtest/gtest.h>
 

TEST(BoundaryValueTest, 2Bin_Empty) { 
    Log2Hist log2hist(2);
    log2hist.Add(10);
    log2hist.Add(11);
    log2hist.Add(11.5);
    
    uint32_t *bins = log2hist.GetBins();
    uint32_t res[3] = {3, 0, 0};
    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(res[i], bins[i]);
    }   
}

TEST(BoundaryValueTest, 2Bins) { 
    Log2Hist log2hist(2);
    log2hist.Add(0);
    log2hist.Add(1);
    log2hist.Add(1.5);
    log2hist.Add(2);
    log2hist.Add(3);
    log2hist.Add(4);
    
    uint32_t *bins = log2hist.GetBins();
    uint32_t res[3] = {1, 3, 2};
    for (int i = 0; i < 3; i++)
    {
        EXPECT_EQ(res[i], bins[i]);
    }   
}
 
TEST(BoundaryValueTest, 3Bins) { 
    Log2Hist log2hist(3);
    log2hist.Add(0);
    log2hist.Add(0);
    log2hist.Add(1);
    log2hist.Add(1.5);
    log2hist.Add(1.99);
    log2hist.Add(2);
    log2hist.Add(3);
    log2hist.Add(4);
    log2hist.Add(5);
    log2hist.Add(5);
    log2hist.Add(7);
    log2hist.Add(8);
    
    uint32_t *bins = log2hist.GetBins();
    uint32_t res[4] = {1, 5, 2, 4};
    for (int i = 0; i < 4; i++)
    {
        EXPECT_EQ(res[i], bins[i]);
    }   
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}