/******************************************************************************/
/*! @brief      nat shared memory base class header
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef SHARED_MEMORY_BASE_H
#define SHARED_MEMORY_BASE_H


#include "nat_libs.hh"

namespace nat{

  /*! @class SharedMemoryBase
      @brief
        Shared memory basic functionality\n
  */
  class SharedMemoryBase {
  public:
      enum { UNKNOWN, PARENT, CHILD };
  public:
      STATUS Msync(void);
  protected:
      SharedMemoryBase(const char*, TYPE, SIZE);
      virtual ~SharedMemoryBase();
  private:
      STATUS _Create(const char*);
      STATUS _Reference(const char*);
      STATUS _Remove(void);
  protected:
      std::string       shmnm_;
      int               shmid_;
      STATUS            status_;
      TYPE              type_;
      char*             base_ptr_;
      pthread_mutex_t*  mutex_ptr_;
      U64*              data_ptr_;
      SIZE              size_;
  protected:
      SharedMemoryBase(){};
  };


}// namespace nat


#endif //SHARED_MEMORY_BASE_H
