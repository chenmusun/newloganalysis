#include <iostream>
#include <string>
#include<stdio.h>
//#include<stdlib.h>
#include"log_analysis_manager.h"
INITIALIZE_EASYLOGGINGPP
int main ()
{
	el::Configurations conf("./log.conf");
	el::Loggers::reconfigureLogger("default", conf);
	zmq::context_t context(1);
	LogAnalysisManager lam(context,4);
	if(lam.init("tcp://localhost:5673")){
		lam.Run();
	}

	// FILE * file=fopen("./0857140120150529143207MS8.lte","r");
	// LogAnalysis logana;
	// char line[1024]={0};
	// while(fgets(line,1024,file)!=NULL){
	// 	logana.AnalyseOneLine(line);
	// }
//double d=atof("991.68");
// long ld=d*1000.0;
//long long ll=((double)atof("991.68"))*1000;
//std::cout<<d<<std::endl;
//std::cout<<ll<<std::endl;
//std::cout<<ld<<std::endl;
  return 0;
}
