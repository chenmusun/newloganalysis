#include "log_analysis.h"
#include"lteieeventcommon.h"
KeywordProc  keyword_proc[]={
		{"DAY",LogAnalysis::GenerateTimeStamp},
		{"HOUR",LogAnalysis::GenerateTimeStamp},
		{"MIN",LogAnalysis::GenerateTimeStamp},
		{"SEC",LogAnalysis::GenerateTimeStamp},
		{"GPS",LogAnalysis::GenerateGPS},
		{"LAB",LogAnalysis::GenerateStartFlag},
		{"LBQ",LogAnalysis::GenerateIEEvent},
		{"APPLAYER",LogAnalysis::GenerateIE},
		{"APPEVENT",LogAnalysis::GenerateEvent},
};

EventNameID event_name_id[]={
		{"FTP_S_APPServiceRequest_Download",FTP_S_APPServiceRequest_Download},
		{"FTP_S_APPServiceSuccess_Download",FTP_S_APPServiceSuccess_Download},
		{"FTP_S_APPServiceDrop_Download",FTP_S_APPServiceDrop_Download},
		{"FTP_S_APPServiceFailure_Download",FTP_S_APPServiceFailure_Download},
		{"FTP_S_APPServiceRequest_Upload",FTP_S_APPServiceRequest_Upload},
		{"FTP_S_APPServiceSuccess_Upload",FTP_S_APPServiceSuccess_Upload},
		{"FTP_S_APPServiceDrop_Upload",FTP_S_APPServiceDrop_Upload},
		{"FTP_S_APPServiceFailure_Upload",FTP_S_APPServiceFailure_Upload},
		{"HTTP_S_APPServiceRequest_Browse",HTTP_S_APPServiceRequest_Browse},
		{"HTTP_S_APPServiceConfirm_Browse",HTTP_S_APPServiceConfirm_Browse},
		{"HTTP_S_APPServiceSuccess_Browse",HTTP_S_APPServiceSuccess_Browse},
		{"HTTP_S_APPServiceRequest_Download",HTTP_S_APPServiceRequest_Download},
		{"HTTP_S_APPServiceSuccess_Download",HTTP_S_APPServiceSuccess_Download},
		{"HTTP_S_APPServiceDrop_Download",HTTP_S_APPServiceDrop_Download},
		{"STREAM_S_APPServiceRequest_Stream",STREAM_S_APPServiceRequest_Stream},
		{"STREAM_S_APPServiceSuccess_Stream",STREAM_S_APPServiceSuccess_Stream},
		{"STREAM_S_APPServiceConnect_Stream",STREAM_S_APPServiceConnect_Stream},
		{"STREAM_C_StreamRealPlay_Stream",STREAM_C_StreamRealPlay_Stream},
		{"STREAM_C_StreamBufferBegin_Stream",STREAM_C_StreamBufferBegin_Stream},
		{"STREAM_C_StreamBufferEnd_Stream",STREAM_C_StreamBufferEnd_Stream},
		{"STREAM_C_StreamFileBegin_Stream",STREAM_C_StreamFileBegin_Stream},
		{"STREAM_C_StreamFileEnd_Stream",STREAM_C_StreamFileEnd_Stream},
		{"STREAM_S_APPServiceReject_Stream",STREAM_S_APPServiceReject_Stream},
		{"STREAM_S_APPServiceTimeout_Stream",STREAM_S_APPServiceTimeout_Stream},
		{"STREAM_S_APPServiceFailure_Stream",STREAM_S_APPServiceFailure_Stream},
};


set<KeywordProc> LogAnalysis::keyword_proc_set_(keyword_proc,keyword_proc+sizeof(keyword_proc)/sizeof(KeywordProc));
set<EventNameID> LogAnalysis::event_name_id_set_(event_name_id,event_name_id+sizeof(event_name_id)/sizeof(EventNameID));
LogAnalysis::LogAnalysis():start_analysis_(false),time_no_data_(0),log_status_(READY)
{

}

LogAnalysis::~LogAnalysis()
{

}

void LogAnalysis::AnalyseOneLine(const string& line)
{
	string head=SplitFirstString(line,'\t');
	KeywordProc kp;
        kp.keyword=head;

	auto pos=keyword_proc_set_.find(kp);
	//if((set<KeywordProc>::iterator pos=))
	if(pos!=keyword_proc_set_.end())
	{
//		if(start_analysis_||(pos->keyword=="LAB"))
			pos->proc(line,this);
        }

}

string LogAnalysis::SplitFirstString(const string& str_source,char sep)
{
  size_t pos=str_source.find(sep);
  return (pos==string::npos)?"":str_source.substr(0,pos);
}

void LogAnalysis::SplitStrings(const string& str_source,char sep,vector<string>& vec,bool erase)
{
  //str_source.pop_back();//删除最后一个字符\n
  size_t begin=0;
  size_t end=str_source.find(sep,begin);
  while(end!=string::npos){
    vec.push_back(str_source.substr(begin,end-begin));
    begin=end+1;
    end=str_source.find(sep,begin);
  }
  if(erase)
	  vec.push_back(str_source.substr(begin,str_source.length()-begin-1));//去除末尾的\n
  else
	  vec.push_back(str_source.substr(begin));
}

void LogAnalysis::GenerateTimeStamp(const string& str_time,void * arg/*TimeEnum te*/)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
//	LogTime * time=reinterpret_cast<LogTime *>(output);
  vector<string> time_vec;
  SplitStrings(str_time,'\t',time_vec);
  int vec_size=time_vec.size();
  if(!vec_size)
	  return;

  if((time_vec[0]=="DAY")&&(vec_size>=4))
	  loganalysis->time_.year_month_day=time_vec[1]+"-"+time_vec[2]+"-"+time_vec[3];
  else if((time_vec[0]=="HOUR")&&(vec_size>=2))
	  loganalysis->time_.hour=time_vec[1];
  else if((time_vec[0]=="MIN")&&(vec_size>=2))
	  loganalysis->time_.minute=time_vec[1];
  else if((time_vec[0]=="SEC")&&(vec_size>=2)){
	  loganalysis->time_.second=time_vec[1];
	  loganalysis->time_.millisecond=atoi(time_vec[2].c_str());
	  //生成时间
	  char fmt[] = "%Y-%m-%d-%H:%M:%S";
	  string timestr=loganalysis->time_.year_month_day+"-"+loganalysis->time_.hour+":"+loganalysis->time_.minute+":"+loganalysis->time_.second;
	  // char buf[] = "2000-01-01-00:00:00";
	  LOG(INFO)<<"Time now : "<<timestr<<" "<<time_vec[2]<<std::endl;
	  struct tm t;
	  strptime(timestr.c_str(),fmt,&t);
	  time_t sec;
	  if((sec=mktime(&t))!=-1){
		  loganalysis->time_.timestamp=sec*1000+loganalysis->time_.millisecond;
	  }

  }
  else{

  }

}

void LogAnalysis::GenerateGPS(const string& str_GPS,void *arg)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
//	LogGPS * gps=reinterpret_cast<LogGPS *>(output);
  vector<string> gps_vec;
  SplitStrings(str_GPS,'\t',gps_vec);
  int vec_size=gps_vec.size();
  if(vec_size>=3){
	  loganalysis->gps_.longitude=atof(gps_vec[1].c_str());
	  loganalysis->gps_.latitude=atof(gps_vec[2].c_str());
	  LOG(INFO)<<"GPS now : "<<loganalysis->gps_.longitude<<"	"<<loganalysis->gps_.latitude<<std::endl;
  }
}

void LogAnalysis::GenerateStartFlag(const string& str_flag,void * arg)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
	//bool *start_analysis=reinterpret_cast<bool *>(output);
  vector<string> flag_vec;
  SplitStrings(str_flag,'\t',flag_vec);
  int vec_size=flag_vec.size();
  if(vec_size>=3){
    if(flag_vec[2]=="Application_Test_Begin"){
    	loganalysis->start_analysis_=true;
    }
    else if(flag_vec[2]=="Application_Test_End"){
    	loganalysis->start_analysis_=false;
    }
    else{
    }
  }
  LOG(INFO)<<"Start Flag now : "<<loganalysis->start_analysis_<<std::endl;
}


void LogAnalysis::GenerateIE(const string& ie,void * arg)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
	if(!loganalysis->start_analysis_)
		return;
	  vector<string> ie_vec;
	  SplitStrings(ie,'\t',ie_vec);
	  int vec_size=ie_vec.size();
	  if(vec_size>=4){
		  vector<string> ie_value_vec;
		  SplitStrings(ie_vec[3],'#',ie_value_vec,false);
		  vec_size=ie_value_vec.size();
		  map<int,long long> ie_map;
		  for(int i=0;i<vec_size;++i)
		  {
			  if(ie_value_vec[i].empty())
				  continue;
			   long value=atof(ie_value_vec[i].c_str())*1000+0.5;
			  if(ie_vec[1]=="FTP"){
			  		  ie_map.insert(std::make_pair(IE_FTP_INFO[i],value));
			  		 LOG(INFO)<<"FTP "<<i<<" "<<value<<std::endl;
				}
			  else if(ie_vec[1]=="HTTP"){
				  ie_map.insert(std::make_pair(IE_HTTP_INFO[i],value));
				  LOG(INFO)<<"HTTP "<<i<<" "<<value<<std::endl;
			  }
			  else if(ie_vec[1]=="STREAM"){
				  ie_map.insert(std::make_pair(IE_STREAM_INFO[i],value));
				  LOG(INFO)<<"STREAM "<<i<<" "<<value<<std::endl;
			  }
			  else{

			  }

		  }

		  	//TODO 对IE进行分析
	  }

}

void LogAnalysis::GenerateEvent(const string& event,void * arg)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
	if(!loganalysis->start_analysis_)
		return;
	  vector<string> event_vec;
	  SplitStrings(event,'\t',event_vec);
	  int vec_size=event_vec.size();
	  if(vec_size>=5){
		  EventNameID en;
		  en.event_name=event_vec[1]+"_"+event_vec[3]+"_"+event_vec[4];
		//  LOG(INFO)<<en.event_name<<std::endl;
		  auto pos=event_name_id_set_.find(en);
		  if(pos!=event_name_id_set_.end()){
			  int event_id=pos->event_id;
			  LOG(INFO)<<"Event name: "<<en.event_name<<" Event ID : "<<event_id<<std::endl;
		  }
	  }
}

void LogAnalysis::GenerateIEEvent(const string& ie_event,void *arg)
{
	LogAnalysis * loganalysis=reinterpret_cast<LogAnalysis *>(arg);
	if(!loganalysis->start_analysis_)
		return;
}


void LogAnalysis::PushLogInfo(std::string& str){
  std::lock_guard<std::mutex>  lock(queue_mutex_);
  log_info_queue_.push(str);
}
std::string LogAnalysis::PopLogInfo(){
  std::string ret;
  do{
    {
      std::lock_guard<std::mutex>  lock(queue_mutex_);
      if(!log_info_queue_.empty()){
        ret=log_info_queue_.front();
        log_info_queue_.pop();
        time_no_data_=0;
        break;
      }
    }

    time_no_data_=time(0);
    LOG(INFO)<<"SetLogStatus";
    SetLogStatus(OVERTIME);
      // time_t nowtime=time(0);
      // if(time_no_data_){
      //   if(nowtime-time_no_data_>60)
      //     SetLogStatus(HANDLED);
      // }
      // else {
      //   time_no_data_=nowtime;
      // }

    }while(0);

  return ret;
}

LogStatus LogAnalysis::GetLogStatus()
{
  std::lock_guard<std::mutex>  lock(log_status_mutex_);
  return log_status_;
}
void LogAnalysis::SetLogStatus(LogStatus ls)
{
    std::lock_guard<std::mutex>  lock(log_status_mutex_);
    log_status_=ls;
}
