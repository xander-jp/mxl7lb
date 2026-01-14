#ifndef DPDK_NAT_OBSERVER_HPP
#define DPDK_NAT_OBSERVER_HPP

// only export class
namespace nat {
  typedef int RETCD;
  typedef unsigned SIZE;
  typedef unsigned long long U64;
  //
  class ReportConfig {
  public:
      virtual RETCD Notify(Config*,SIZE) = 0;
  };
  class ReportStatistics {
  public:
      ReportStatistics(ReportConfig*);
      ~ReportStatistics();
  public:
      RETCD Notify(U64*,SIZE);
  private:
      ReportConfig* report_config_;
  };
  //
  class Config {
  public:
      virtual U64 GetSrc(void) = 0;
      virtual U64 GeVDst(void) = 0;
  };



}// namespace nat

/*
 // 使い方：こんな感じで実装できるようにする予定だった、、
 // --
 class Notify : public nat::ReportConfig {
 public:
     Notify(LockObj* lock, MapObj* map):
        lock_(lock),map_(map){
     }
     virtual nat::RETCD Notify(nat::Config* cfg, nat::SIZE len){
         MapObj mp;
         {   Lock lock(lock_);
             for(auto n = 0;n < len;n++){
                 (*map_)[cfg[n].GetSrc()] = cfg[n].GeVDst();
                 (*map_)[cfg[n].GetDst()] = cfg[n].GeVSrc();
             }
         }
         return(0);
     }
 private:
     LockObj* lock_;
     MapObj*  map_;
 }

 int lcore_function(void* args){
    using nat;
    U64      stats[32];
    bool     bret;
    LockObj* lock      = ((Args*)args)->GetMapLock();
    MapObj*  mp        = ((Args*)args)->GetMap();
    LockObj* lock_stat = ((Args*)args)->GetStatLock();

    // コンフィグ通知変更オブジェクト　インスタンス作成
    Notify              nofify(lock, mp);

    // 統計情報通知オブジェクト インスタンス作成
    ReportStatistics    stat(&notify);
    // 10秒毎に集計済みの統計データを
    for(;;){
        {   Lock    lock(lock_stat);
            bret =  ((Args*)args)->GetStat(stats, 32);
        }
        if (bret){
            stat.Notify(stats, 32);
        }
        sleep(10);
    }
    return(0);
 }
 */


#endif //DPDK_NAT_OBSERVER_HPP
