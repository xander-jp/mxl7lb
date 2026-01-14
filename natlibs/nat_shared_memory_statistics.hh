/******************************************************************************/
/*! @brief      nat shared memory statistics class header
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef SHARED_MEMORY_STATISTICS_H
#define SHARED_MEMORY_STATISTICS_H


#include "nat_libs.hh"
#include "nat_shared_memory_base.hh"

namespace nat{

  /*! @class SharedMemoryStatistics
      @brief
        Statistics shared memory region\n
  */
  class SharedMemoryStatistics : public SharedMemoryBase{
  public:
      SharedMemoryStatistics(const char*, TYPE);
      virtual ~SharedMemoryStatistics();
  public:
      STATUS Update(U64*,SIZE);
      STATUS Read(U64*,SIZE);
  private:
      SharedMemoryStatistics(){};
  };
}// namespace nat


#endif //SHARED_MEMORY_STATISTICS_H
