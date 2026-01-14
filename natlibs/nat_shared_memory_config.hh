/******************************************************************************/
/*! @brief      nat shared memory config class header
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef SHARED_MEMORY_CONFIG_H
#define SHARED_MEMORY_CONFIG_H


#include "nat_libs.hh"
#include "nat_shared_memory_base.hh"

namespace nat{

  /*! @class SharedMemoryConfig
      @brief
        Config shared memory region\n
  */
  class SharedMemoryConfig : public SharedMemoryBase{
  public:
      SharedMemoryConfig(const char*, TYPE);
      virtual ~SharedMemoryConfig();
  public:
      STATUS Update(U128*,SIZE);
      STATUS Read(U128*,SIZE);
      STATUS Modified(void);
  private:
      U128* config_data_;
  private:
      SharedMemoryConfig(){};
  };
}// namespace nat


#endif //SHARED_MEMORY_CONFIG_H
