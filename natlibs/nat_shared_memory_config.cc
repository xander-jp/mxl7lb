/******************************************************************************/
/*! @brief      nat shared memory config class implementation
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "nat_shared_memory_config.hh"

namespace nat{

  /** *************************************************************
   * Constructor\n
   *
   * @param[in]     shmnm      shared memory name (ex./ipc_statistics)
   * @param[in]     type       parent process / child process
   * @throw   throws exception on error
   ************************************************************* */
  SharedMemoryConfig::SharedMemoryConfig(const char* shmnm, unsigned type):
      SharedMemoryBase(shmnm, type, CONFIG_SHARED_MEMORY_SIZE){
      config_data_ = (U128*)data_ptr_;
  }
  /** *************************************************************
   * Destructor\n
   *
   * @attention  shared memory will leak if instance is not released in parent process
   ************************************************************* */
  SharedMemoryConfig::~SharedMemoryConfig(){

  }
  /** *************************************************************
   * Update config to shared memory\n
   *
   * @param[in]     config        config data
   * @param[in]     cnt           config item count
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryConfig::Update(U128* config,SIZE cnt){
      if (status_ != OK || !config || !cnt){
          return(ERR);
      }
      if (cnt > CONFIG_ITEM_COUNT){
          return(ERR);
      }
      pthread_mutex_lock(mutex_ptr_);
      memset(config_data_, 0, sizeof(U128) * CONFIG_ITEM_COUNT);
      // set the most significant bit
      (*((U32*)config_data_)) |= (1U<<31);
      memcpy((config_data_+1), config, sizeof(U128) * cnt);
      pthread_mutex_unlock(mutex_ptr_);
      return(OK);
  }
  /** *************************************************************
   * Read config from shared memory\n
   *
   * @param[in/out] config    config data
   * @param[in]     cnt       config buffer item count (not byte size)
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryConfig::Read(U128* config,SIZE cnt){
      if (status_ != OK || !config || !cnt){
          return(ERR);
      }
      if (cnt > CONFIG_ITEM_COUNT){
          return(ERR);
      }
      pthread_mutex_lock(mutex_ptr_);
      // clear the most significant bit
      (*((U32*)config_data_)) &=~(1U<<31);

      memcpy(config, (config_data_+1), sizeof(U128) * cnt);
      pthread_mutex_unlock(mutex_ptr_);
      return(OK);
  }
  /** *************************************************************
   * Check config modification flag\n
   *
   * @result  OK=modified, OK!=not modified
   ************************************************************* */
  STATUS SharedMemoryConfig::Modified(void){
      STATUS ret = OK;
      if (status_ != OK){
          return(ERR);
      }
      pthread_mutex_lock(mutex_ptr_);
      ret = ((*((U32*)config_data_)) & (1U<<31))?OK:ERR;
      pthread_mutex_unlock(mutex_ptr_);
      return(ret);
  }
}// namespace nat

