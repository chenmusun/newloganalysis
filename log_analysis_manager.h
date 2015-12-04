#pragma once
#include "ThreadPool.h"
#include"log_analysis.h"
#include"zhelpers.hpp"
//add open function
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string HAS_NO_WORKER= "HASNOWORKER";
const std::string CLIENT_REQUEST= "CLIENTREQUEST";
const std::string WORKER_REQUEST= "WORKERREQUEST";
const std::string BROKER_REPLY= "BROKERREPLY";
const int MAX_OVERTIME=60;
class LogAnalysisManager{
public:
        LogAnalysisManager(zmq::context_t& cxt,int thread_num);
        bool init(const std::string& addr);
        void Run();
        static  void RecvLogInfo(LogAnalysisManager * lam);
        static  void PickLogInfo(LogAnalysisManager * lam);
        static  void HandleLogInfo(LogAnalysisManager * lam);
        static  void AnalyseLogInfo(LogAnalysis * pla,LogAnalysisManager * lam);
        static  void ScanLogAnalysisFinished(LogAnalysisManager * lam);
        static  void ScanLogAnalysisOvertime(LogAnalysisManager * lam);
        /* void LogDataHandle(); */
        std::queue<std::string> log_info_queue_;//初始日志队列
        std::mutex log_queue_mutex_;
        std::condition_variable log_queue_cv_;
        std::map<std::string,LogAnalysis *> log_analysis_map_;//对象map
        std::mutex log_map_mutex_;
        /* std::condition_variable log_map_cv_; */
        std::queue<LogAnalysis *> log_analysis_queue_;//待处理对象队列
        std::mutex log_analysis_queue_mutex_;
        std::condition_variable log_analysis_queue_cv_;
        zmq::socket_t requester_;//(context, ZMQ_DEALER);
        std::queue<LogAnalysis *> log_handled_queue_;//完成处理对象队列
        std::condition_variable log_handled_queue_cv_;
        std::mutex log_handled_queue_mutex_;
        //overtime object map
        std::map<std::string,LogAnalysis *> log_analysis_overtime_map_;
        std::mutex overtime_map_mutex_;
        std::condition_variable overtime_map_cv_;
        ThreadPool pool_;
        /* std::mutex queue_cv_mutex_; */
};
