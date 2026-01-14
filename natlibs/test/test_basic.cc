/******************************************************************************/
/*! @brief      natlibs basic tests
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "test_def.hh"


#include "nat_shared_memory_statistics.hh"
#include "nat_shared_memory_config.hh"
#include <fstream>
#include <iostream>


int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new CustomEnvironment());
    return(RUN_ALL_TESTS());
}

TEST(BasicTest, AllocFree) {
    nat::SharedMemoryConfig  sc("/ipc_test_key", nat::SharedMemoryBase::PARENT);
}

TEST(BasicTest, ParentChildCommunicate) {
    using namespace nat;
    U128 data[4] = {{0,1},{2,3},{4,5},{6,7},};
    pid_t child_pid;
    if ((child_pid = fork()) == -1){
        throw std::runtime_error("failed. fork()\n");
    }
    // child process handling
    if (child_pid == 0){
        memset(data, 0, sizeof(data));
        usleep(100000);
        {   nat::SharedMemoryConfig  sc("/ipc_test_key", nat::SharedMemoryBase::CHILD);
            sc.Read(data, 4);
            printf("child[%u/%u/%u/%u/%u/%u/%u/%u]\n",
                unsigned(data[0].hi),unsigned(data[0].low),
                unsigned(data[1].hi),unsigned(data[1].low),
                unsigned(data[2].hi),unsigned(data[2].low),
                unsigned(data[3].hi),unsigned(data[3].low));
        }
        EXPECT_EQ(data[0].hi, 0UL);
        usleep(100000);
        {   nat::SharedMemoryConfig  sc("/ipc_test_key", nat::SharedMemoryBase::CHILD);
            data[0].hi = 10; data[0].low = 11;
            data[1].hi = 20; data[1].low = 21;
            data[2].hi = 30; data[2].low = 31;
            data[3].hi = 40; data[3].low = 41;
            //
            EXPECT_EQ(sc.Update(data, 4), nat::OK);
        }
        exit(0);
    }
    // parent process handling
    nat::SharedMemoryConfig  sc("/ipc_test_key", nat::SharedMemoryBase::PARENT);
    EXPECT_EQ(sc.Update(data,4), nat::OK);
    usleep(1000000);
    EXPECT_EQ(sc.Read(data, 4), nat::OK);
    printf("parent[%u/%u/%u/%u/%u/%u/%u/%u]\n",
    unsigned(data[0].hi),unsigned(data[0].low),
    unsigned(data[1].hi),unsigned(data[1].low),
    unsigned(data[2].hi),unsigned(data[2].low),
    unsigned(data[3].hi),unsigned(data[3].low));

    EXPECT_EQ(data[0].hi, 10);
    EXPECT_EQ(data[0].low, 11);
    EXPECT_EQ(data[1].hi, 20);
    EXPECT_EQ(data[1].low, 21);
}
