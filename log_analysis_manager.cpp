#include"log_analysis_manager.h"
LogAnalysisManager::LogAnalysisManager(zmq::context_t& cxt,int thread_num)
        :pool_(thread_num),
         requester_(cxt,ZMQ_DEALER)
{

}


void LogAnalysisManager::Run()
{
        std::thread recv_thread(LogAnalysisManager::RecvLogInfo,this);
        std::thread pick_thread(LogAnalysisManager::PickLogInfo,this);
        std::thread handle_thread(LogAnalysisManager::HandleLogInfo,this);
        std::thread scan_thread(LogAnalysisManager::ScanLogAnalysisFinished,this);
        std::thread overtime_thread(LogAnalysisManager::ScanLogAnalysisOvertime,this);
        recv_thread.join();
        pick_thread.join();
        handle_thread.join();
        scan_thread.join();
        overtime_thread.join();
}

bool LogAnalysisManager::init(const std::string& addr){
        bool ret=true;
        requester_.connect(addr);//"tcp://localhost:5673"
        s_send(requester_,WORKER_REQUEST);
        std::string worker=s_recv(requester_);
        if(worker!=BROKER_REPLY){
                ret=false;
                LOG(ERROR)<<"LogAnalysisManager::init failed";
        }
        return ret;
}

void LogAnalysisManager::RecvLogInfo(LogAnalysisManager * lam){
        LOG(INFO)<<"RecvLogInfo thread start";
        zmq::pollitem_t items[]={
                // Always poll for worker activity on backend
                        { (void *)lam->requester_, 0, ZMQ_POLLIN, 0 },
        };
        while (1) {
                zmq::poll(&items[0], 1, -1);
                //  Handle worker activity on backend
                if (items[0].revents & ZMQ_POLLIN) {
                        std::string log_name=s_recv(lam->requester_);
                        std::string data=s_recv(lam->requester_);
                        {
                                std::lock_guard<std::mutex>  lock(lam->log_queue_mutex_);
                                LOG(INFO)<<"RecvLogInfo put "<<log_name<<" into log_info_queue";
                                lam->log_info_queue_.push(log_name);
                                lam->log_info_queue_.push(data);
                                /* } */

                                /* { */
                                /*         std::lock_guard<std::mutex> lock(queue_cv_mutex_); */
                                lam->log_queue_cv_.notify_all();
                        }

                }
        }

 }
void LogAnalysisManager::PickLogInfo(LogAnalysisManager * lam){
        LOG(INFO)<<"PickLogInfo thread start";
        while(1){
                std::unique_lock<std::mutex> lock(lam->log_queue_mutex_);
                lam->log_queue_cv_.wait(lock,[lam]{return !lam->log_info_queue_.empty();});
                std::string log_name=lam->log_info_queue_.front();
                // LOG(INFO)<<"PickLogInfo Get "<<log_name<<" from log_info_queue";
                lam->log_info_queue_.pop();
                std::string data=lam->log_info_queue_.front();
                lam->log_info_queue_.pop();
                lock.unlock();
                {
                        std::lock_guard<std::mutex>  lock(lam->log_map_mutex_);
                        // LOG(INFO)<<"PickLogInfo access the log_analysis_map";
                        auto pos=lam->log_analysis_map_.find(log_name);
                        if(pos!=lam->log_analysis_map_.end()){
                                // LOG(INFO)<<"PickLogInfo push data into log_analysis object";
                                (pos->second)->PushLogInfo(data);
                                //judge if the object now is in the overtime map
                                if((pos->second)->GetLogStatus()==OVERTIME){
                                        LOG(INFO)<<"wakeup the overtime object from overtime map";
                                        (pos->second)->SetLogStatus(READY);
                                        {
                                                std::lock_guard<std::mutex>  lock(lam->log_analysis_queue_mutex_);
                                                LOG(INFO)<<"PickLogInfo add non-overtime log_analysis object into log_analysis_queue";
                                                lam->log_analysis_queue_.push(pos->second);
                                                lam->log_analysis_queue_cv_.notify_all();
                                        }
                                        std::lock_guard<std::mutex>  lock(lam->overtime_map_mutex_);
                                        auto pos_del=lam->log_analysis_overtime_map_.find(log_name);
                                        if(pos_del!=lam->log_analysis_overtime_map_.end())
                                                lam->log_analysis_overtime_map_.erase(pos_del);
                                        else{
                                                LOG(ERROR)<<"can't find object in overtime map";
                                        }

                                }

                        }
                        else {
                                // LOG(INFO)<<"PickLogInfo create new log_analysis object";
                                LogAnalysis * pla=new LogAnalysis();
                                pla->log_name_=log_name;
                                pla->PushLogInfo(data);
                                //TODO
                                char path[256]={0};
                                sprintf(path,"./save/%s",log_name.c_str());
                                pla->fd_=open(path,O_WRONLY|O_APPEND|O_CREAT,S_IWUSR|S_IRUSR);
                                //
                                // log_map.insert(std::make_pair(log_name,fd));
                                // if( write(pla->fd_,data.c_str(),data.length())==-1)
                                //         std::cout << "write data error" << "\n";
                                lam->log_analysis_map_.insert(std::make_pair(log_name,pla));
                                //将新增对象放到就绪队列中并通知
                                std::lock_guard<std::mutex>  lock(lam->log_analysis_queue_mutex_);
                                LOG(INFO)<<"PickLogInfo add new log_analysis object into log_analysis_queue";
                                lam->log_analysis_queue_.push(pla);
                                lam->log_analysis_queue_cv_.notify_all();
                        }
                }


        }
}
void LogAnalysisManager::HandleLogInfo(LogAnalysisManager * lam){
        LOG(INFO)<<"HandleLogInfo thread start";
        while (1) {
                std::unique_lock<std::mutex> lock(lam->log_analysis_queue_mutex_);
                lam->log_analysis_queue_cv_.wait(lock,[lam]{return !lam->log_analysis_queue_.empty();});
                // LOG(INFO)<<"HandleLogInfo get log_analysis object from log_analysis_queue";
                LogAnalysis * pla=lam->log_analysis_queue_.front();
                lam->log_analysis_queue_.pop();
                lock.unlock();
                //TODO此处用线程池开始调用
                // LOG(INFO)<<"HandleLogInfo uses thread_pool to handle data";
                lam->pool_.enqueue(LogAnalysisManager::AnalyseLogInfo,pla,lam);
        }

}

void LogAnalysisManager::AnalyseLogInfo(LogAnalysis * pla,LogAnalysisManager * lam){
        LOG(INFO)<<"AnalyseLogInfo thread start";
        LogStatus ls=pla->GetLogStatus();
        if(ls==HANDLED){
                LOG(INFO)<<"AnalyseLogInfo put handled object into log_handled_queue";
                std::lock_guard<std::mutex>  lock(lam->log_handled_queue_mutex_);
                lam->log_handled_queue_.push(pla);
                lam->log_handled_queue_cv_.notify_all();
        }
        else if(ls==OVERTIME) {
                LOG(INFO)<<"AnalyseLogInfo put handled object into overtime_map";
                std::lock_guard<std::mutex>  lock(lam->overtime_map_mutex_);
                lam->log_analysis_overtime_map_.insert(std::make_pair(pla->log_name_,pla));
                lam->overtime_map_cv_.notify_all();
        }
        else {
                LOG(INFO)<<"AnalyseLogInfo handle data";
                std::string data=pla->PopLogInfo();
                if(!data.empty()){
                        if( write(pla->fd_,data.c_str(),data.length())==-1)
                                std::cout << "write data error" << "\n";
                }

                //put object back to ready queue
                {
                        std::lock_guard<std::mutex>  lock(lam->log_analysis_queue_mutex_);
                        LOG(INFO)<<"AnalyseLogInfo thread put log_analysis object back into log_analysis_queue";
                        lam->log_analysis_queue_.push(pla);
                        lam->log_analysis_queue_cv_.notify_all();
                }
        }

 }

void LogAnalysisManager::ScanLogAnalysisFinished(LogAnalysisManager * lam){
        LOG(INFO)<<"ScanLogAnalysisFinished thread start";
        std::unique_lock<std::mutex> lock(lam->log_handled_queue_mutex_);
        lam->log_handled_queue_cv_.wait(lock,[lam]{return !lam->log_handled_queue_.empty();});
        // LOG(INFO)<<"ScanLogAnalysisFinished thread get log_analysis from log_handled_queue";
        LogAnalysis * pla=lam->log_handled_queue_.front();
        lam->log_handled_queue_.pop();
        lock.unlock();
        {
                std::lock_guard<std::mutex>  lock(lam->log_map_mutex_);
                LOG(INFO)<<"ScanLogAnalysisFinished thread erase handled log_analysis object from log_analysis_map "<<pla->log_name_;
                auto pos=lam->log_analysis_map_.find(pla->log_name_);
                if(pos!=lam->log_analysis_map_.end()){
                        delete pos->second;
                        lam->log_analysis_map_.erase(pos);
                }

        }
}


void LogAnalysisManager::ScanLogAnalysisOvertime(LogAnalysisManager * lam){
        LOG(INFO)<<"ScanLogAnalysisOvertime thread start";
        while(1){
                std::unique_lock<std::mutex> lock(lam->overtime_map_mutex_);
                lam->overtime_map_cv_.wait(lock,[lam]{return !lam->log_analysis_overtime_map_.empty();});
                time_t nowtime=time(0);
                LOG(INFO)<<"ScanLogAnalysisOvertime thread start scaning";
                for(auto pos=lam->log_analysis_overtime_map_.begin();pos!=lam->log_analysis_overtime_map_.end();++pos){
                        time_t timespan=nowtime-(pos->second)->time_no_data_;
                        if(timespan>=MAX_OVERTIME){
                                std::lock_guard<std::mutex>  lock(lam->log_map_mutex_);
                                LOG(INFO)<<"Overtime thread erase  object from log_analysis_map "<<pos->first<<" add timespan is "<<timespan;
                                auto pos_del=lam->log_analysis_map_.find(pos->first);
                                if(pos_del!=lam->log_analysis_map_.end()){
                                        delete pos_del->second;
                                        lam->log_analysis_map_.erase(pos_del);
                                }
                                lam->log_analysis_overtime_map_.erase(pos);

                        }

                }
                lock.unlock();
                sleep(MAX_OVERTIME);
        }
}
