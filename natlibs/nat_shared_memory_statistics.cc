/******************************************************************************/
/*! @brief      nat shared memory statistics class implementation
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "nat_shared_memory_statistics.hh"

namespace nat{

  /** *************************************************************
   * Constructor\n
   *
   * @param[in]     shmnm      shared memory name (ex./ipc_statistics)
   * @param[in]     type       parent process / child process
   * @throw   throws exception on error
   ************************************************************* */
  SharedMemoryStatistics::SharedMemoryStatistics(const char* shmnm, unsigned type):
      SharedMemoryBase(shmnm, type, STATISTICS_SHARED_MEMORY_SIZE){
  }
  /** *************************************************************
   * Destructor\n
   *
   * @attention  shared memory will leak if instance is not released in parent process
   ************************************************************* */
  SharedMemoryStatistics::~SharedMemoryStatistics(){

  }
  /** *************************************************************
   * Update statistics to shared memory\n
   *
   * @param[in]     statistics    statistics data
   * @param[in]     cnt           statistics item count
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryStatistics::Update(U64* statistics,SIZE cnt){
      if (status_ != OK || !statistics || !cnt){
          return(ERR);
      }
      if (cnt > STATISTICS_ITEM_COUNT){
          return(ERR);
      }
      pthread_mutex_lock(mutex_ptr_);
      memcpy(data_ptr_, statistics, sizeof(U64) * cnt);
      pthread_mutex_unlock(mutex_ptr_);
      return(OK);
  }
  /** *************************************************************
   * Read statistics from shared memory\n
   *
   * @param[in/out] statistics    statistics data
   * @param[in]     cnt           statistics buffer item count (not byte size)
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryStatistics::Read(U64* statistics,SIZE cnt){
      if (status_ != OK || !statistics || !cnt){
          return(ERR);
      }
      if (cnt > STATISTICS_ITEM_COUNT){
          return(ERR);
      }
      pthread_mutex_lock(mutex_ptr_);
      memcpy(statistics, data_ptr_, sizeof(U64) * cnt);
      pthread_mutex_unlock(mutex_ptr_);
      return(OK);
  }
}// namespace nat

