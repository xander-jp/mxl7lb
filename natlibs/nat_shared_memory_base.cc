/******************************************************************************/
/*! @brief      nat shared memory base class implementation
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#include "nat_shared_memory_base.hh"

namespace nat{

  /** *************************************************************
   * Constructor\n
   *
   * @param[in]     shmnm      shared memory name (ex./ipc_statistics)
   * @param[in]     type       parent process / child process
   * @param[in]     size       shared memory size
   * @throw   throws std::runtime_error on error
   ************************************************************* */
  SharedMemoryBase::SharedMemoryBase(const char* shmnm, TYPE type, SIZE size):
      shmid_(INVALID_HANDLE),status_(ERR), type_(type),
      base_ptr_(NULL),mutex_ptr_(NULL),data_ptr_(NULL),size_(size){
      //
      if (type_ == PARENT){
          if ((status_ = _Create(shmnm)) != OK){
              throw std::runtime_error("Exception : _Create");
          }
      }else if (type_ == CHILD) {
          if ((status_ = _Reference(shmnm)) != OK){
              throw std::runtime_error("Exception : _Reference");
          }
      }else {
          throw std::runtime_error("not implemented type(UNKNOWN)");
      }
  }
  /** *************************************************************
   * Destructor\n
   *
   * @attention  shared memory will leak if instance is not released in parent process
   ************************************************************* */
  SharedMemoryBase::~SharedMemoryBase(){
      if (type_ != UNKNOWN){
          _Remove();
      }
  }
  /** *************************************************************
   * For child process: get reference to shared memory\n
   *
   * @param[in]     shmnm      shared memory name (ex./ipc_statistics)
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryBase::_Reference(const char* shmnm){
      int	shmfd;
      shmnm_ = shmnm?shmnm:"";
      if (shmnm_.length() == 0){
          return(ERR);
      }
      // skip if already initialized
      if (status_ == OK){
          return(ERR);
      }
      // shared memory region
      shmfd = shm_open(shmnm_.c_str(),O_RDWR,0);
      if (shmfd < 0){
          return(ERR);
      }
      base_ptr_ = (char*)mmap(0, size_, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
      if ((char*)base_ptr_ == (char*)INVALID_HANDLE){
          close(shmfd);
          return(ERR);
      }
      mutex_ptr_ = (pthread_mutex_t*)(base_ptr_);
      data_ptr_ = (U64*)(mutex_ptr_ + 1);
      shmid_ = shmfd;
      close(shmfd);

      return(OK);
  }
  /** *************************************************************
   * For parent process: create shared memory\n
   *
   * @param[in]     shmnm      shared memory name (ex./ipc_statistics)
   * @result  OK=success, OK!=error
   * @throw   throws std::runtime_error on error
   ************************************************************* */
  STATUS SharedMemoryBase::_Create(const char* shmnm){
      int	shmfd;
      shmnm_ = shmnm?shmnm:"";
      pthread_mutexattr_t ma;

      if (shmnm_.length() == 0){
          return(ERR);
      }
      // skip if already initialized
      if (status_ == OK){
          return(ERR);
      }
      // remove first
      shm_unlink(shmnm_.c_str());
      // create shared memory region
      shmfd = shm_open(shmnm_.c_str(), SHM_OPEN_OFLG, SHM_OPEN_UFLG);
      if (shmfd < 0){
          return(ERR);
      }
      if (ftruncate(shmfd, size_) == -1){
          close(shmfd);
          return(ERR);
      }
      base_ptr_ = (char*)mmap(0, size_, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
      if ((char*)base_ptr_ == (char*)INVALID_HANDLE){
          close(shmfd);
          return(ERR);
      }
      mutex_ptr_ = (pthread_mutex_t*)(base_ptr_);
      data_ptr_ = (U64*)(mutex_ptr_ + 1);

      // initialize parameters
      memset(base_ptr_,0,size_);
      shmid_ = shmfd;
      close(shmfd);

      // set mutex attributes
      if (pthread_mutexattr_init(&ma)){ throw std::runtime_error("failed.pthread_mutexattr_init\n"); }
#ifdef PTHREAD_MUTEX_ROBUST_NP
      if (pthread_mutexattr_setrobust_np(&ma,PTHREAD_MUTEX_ROBUST_NP)){ throw std::runtime_error("failed.pthread_mutexattr_setrobust_np\n"); }
#endif
#ifdef PTHREAD_MUTEX_ROBUST
      if (pthread_mutexattr_setrobust(&ma,PTHREAD_MUTEX_ROBUST)){ throw std::runtime_error("failed.pthread_mutexattr_setrobust\n"); }
#endif
      if (pthread_mutexattr_setpshared(&ma,PTHREAD_PROCESS_SHARED)) { throw std::runtime_error("failed.pthread_mutexattr_setpshared\n"); }
      if (pthread_mutexattr_setprotocol(&ma,PTHREAD_PRIO_INHERIT)) { throw std::runtime_error("failed.pthread_mutexattr_setprotocol\n"); }

      // set flags for inter-process use
      pthread_mutex_init(mutex_ptr_, &ma);
      pthread_mutexattr_destroy(&ma);
      //
      return(OK);
  }
  /** *************************************************************
   * Release shared memory resource\n
   * For parent and child processes\n
   *
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryBase::_Remove(void){
      close(shmid_);
      munmap(base_ptr_ ,size_);
      base_ptr_ = (char*)INVALID_HANDLE;
      status_ = ERR;
      return(OK);
  }

  /** *************************************************************
   * Sync shared memory resource\n
   * For parent and child processes\n
   *
   * @result  OK=success, OK!=error
   ************************************************************* */
  STATUS SharedMemoryBase::Msync(){
      return(msync(base_ptr_ ,size_, MS_ASYNC)==0?OK:ERR);
  }

}// namespace nat

