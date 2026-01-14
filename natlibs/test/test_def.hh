/******************************************************************************/
/*! @brief      natlibs test definitions
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef NATLIBS_TEST_DEF_HH
#define NATLIBS_TEST_DEF_HH
#include <thread>

#include "gtest/gtest.h"

class CustomEnvironment :public ::testing::Environment {
public:
    virtual ~CustomEnvironment() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

#endif //NATLIBS_TEST_DEF_HH
