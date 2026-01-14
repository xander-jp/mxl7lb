/******************************************************************************/
/*! @brief      nat shared memory library common header
    @date       created(Dec 11, 2018)
    @par        Copyright 2018 dsugisawa. Released Under the MIT license.
******************************************************************************/
#ifndef NAT_OBSERVER_DEF_H
#define NAT_OBSERVER_DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <pthread.h>


/*! @name basic defineded... */
/* @{ */
#ifndef MIN
#define	MIN(a,b)            (a<b?a:b)    /*!< math:min */
#endif/*MIN*/

#ifndef MAX
#define	MAX(a,b)            (a>b?a:b)    /*!< math::max */
#endif/*MAX*/

#ifndef INVALID_HANDLE
#define INVALID_HANDLE      (0xffffffff)/*!< invalid value */
#endif/*INVALID_HANDLE*/

#if defined(__linux) || defined(__linux__) || defined(linux)
#define SHM_OPEN_OFLG   (O_CREAT|O_EXCL|O_RDWR|O_TRUNC)
#define SHM_OPEN_UFLG   (S_IRUSR|S_IWUSR)
#else
#define SHM_OPEN_OFLG   (O_CREAT|O_RDWR)
#define SHM_OPEN_UFLG   (S_IRUSR|S_IWUSR)
#endif

/* @} */



namespace nat{
  /*! @name typedefs  */
  /* @{ */
  typedef int STATUS;                   /*!< status code */
  typedef unsigned long long U64;       /*!< 64bit unsigned */
  typedef unsigned int U32;             /*!< 32bit unsigned */
  typedef unsigned TYPE;                /*!< enum types */
  typedef unsigned SIZE;                /*!< size  */
  typedef struct u128 {
      U64 hi;
      U64 low;
  }U128;                                /*!< 128 bit */
  /* @} */

  /*! @name status code  */
  /* @{ */
  static const int ERR = -1;            /*!< error */
  static const int OK = 0;              /*!< okay */
  /* @} */

  /*! @name statistics shared memory  */
  /* @{ */
  static const unsigned STATISTICS_SHARED_MEMORY_SIZE = (0x0800);    /*!< memory size */
  static const unsigned STATISTICS_DATA_SIZE = (STATISTICS_SHARED_MEMORY_SIZE - sizeof(pthread_mutex_t));
                                                                     /*!< data area size */
  static const unsigned STATISTICS_ITEM_SIZE = sizeof(U64);          /*!< item size */
  static const unsigned STATISTICS_ITEM_COUNT = (STATISTICS_DATA_SIZE/STATISTICS_ITEM_SIZE);
                                                                     /*!< item max count */
  /* @} */

  /*! @name config shared memory  */
  /* @{ */
  static const unsigned CONFIG_SHARED_MEMORY_SIZE = (0x10000);       /*!< memory size */
  static const unsigned CONFIG_DATA_SIZE = (CONFIG_SHARED_MEMORY_SIZE - sizeof(pthread_mutex_t));
                                                                     /*!< data area size */
  static const unsigned CONFIG_ITEM_SIZE = sizeof(U128);             /*!< item size */
  static const unsigned CONFIG_ITEM_COUNT = (CONFIG_DATA_SIZE/CONFIG_ITEM_SIZE-1);
                                                                     /*!< item max count */
  /* @} */


  /*! @name typedefs  */
  /* @{ */
  typedef int STATUS;                   /*!< status code */
  typedef unsigned long long U64;       /*!< 64bit unsigned */
  typedef unsigned int U32;             /*!< 32bit unsigned */
  typedef unsigned TYPE;                /*!< enum types */
  typedef unsigned SIZE;                /*!< size  */
  /* @} */

  /*! @name statistics enum */
  /* @{ */
  enum StatisticsType {
      StatisticsType_DC = 0,
      StatisticsType_VALID = 1,
      StatisticsType_TRANSFER_PACKET_COUNT = 2,
      StatisticsType_TRANSFER_PACKET_SIZE = 3,
      StatisticsType_DROP_PACKET_COUNT = 4,
      StatisticsType_DROP_PACKET_SIZE = 5,
      StatisticsType_CURRENT_MAP_SIZE = 6,
      StatisticsType_IP_CHECKSUM_INVALID = 7,
      StatisticsType_MAX = 255
  };
  /* @} */


}// namespace nat


#endif //NAT_OBSERVER_DEF_H
