/*
 * ie_event_name_id.h
 *
 *  Created on: 2015年10月14日
 *      Author: chenms
 */
#pragma once
#include"LTEIEDef.h"
#include"EventSettingCollection.h"
using namespace AUTOTEST;

struct IEEventNameID{
	std::string ie_event_name;
	int ie_evnet_id;
	bool operator<(const IEEventNameID &iee) const
	{
		return ie_event_name<iee.ie_event_name;
	}
};
IEEventNameID IE_EVENT_NAME_ID[]={
		{"IE_AppBytesReceived_Total",IE_AppBytesReceived_Total},
		{"IE_AppBytesReceived_LTE",IE_AppBytesReceived_LTE},
		{"IE_AppBytesReceived_HSPA_PLUS",IE_AppBytesReceived_HSPA_PLUS},
		{"IE_AppBytesReceived_HSDPA",IE_AppBytesReceived_HSDPA},
		{"IE_AppBytesReceived_R4",IE_AppBytesReceived_R4},
		{"IE_AppBytesReceived_EVDO",IE_AppBytesReceived_EVDO},
		{"IE_AppBytesReceived_1x",IE_AppBytesReceived_1x},
		{"IE_AppBytesReceived_GPRS_EDGE",IE_AppBytesReceived_GPRS_EDGE},
		{"IE_AppBytesend_Total",IE_AppBytesend_Total},
		{"IE_AppBytesSend_LTE",IE_AppBytesSend_LTE},
		{"IE_AppBytesSend_HSPA_PLUS",IE_AppBytesSend_HSPA_PLUS},
		{"IE_AppBytesSend_HSUPA"	,IE_AppBytesSend_HSUPA},
		{"IE_AppBytesSend_R4"	,IE_AppBytesSend_R4},
		{"IE_AppBytesSend_EVDO",IE_AppBytesSend_EVDO},
		{"IE_AppBytesSend_1x"	,IE_AppBytesSend_1x},
		{"IE_AppBytesSend_GPRS_EDGE",IE_AppBytesSend_GPRS_EDGE},
		{"IE_App_currentThroughput_dl",IE_App_currentThroughput_dl},
		{"IE_App_currentThroughput_ul",IE_App_currentThroughput_ul},
		{"IE_AppBytesReceived_Total_HTTP",IE_AppBytesReceived_Total_HTTP},
		{"IE_AppBytesReceived_LTE_HTTP",IE_AppBytesReceived_LTE_HTTP},
		{"IE_AppBytesReceived_HSPA_PLUS_HTTP",IE_AppBytesReceived_HSPA_PLUS_HTTP},
		{"IE_AppBytesReceived_HSDPA_HTTP",IE_AppBytesReceived_HSDPA_HTTP},
		{"IE_AppBytesReceived_R4_HTTP",IE_AppBytesReceived_R4_HTTP},
		{"IE_AppBytesReceived_EVDO_HTTP",IE_AppBytesReceived_EVDO_HTTP},
		{"IE_AppBytesReceived_1x_HTTP",IE_AppBytesReceived_1x_HTTP},
		{"IE_AppBytesReceived_GPRS_EDGE_HTTP"	,IE_AppBytesReceived_GPRS_EDGE_HTTP},
		{"IE_AppBytesend_Total_HTTP",IE_AppBytesend_Total_HTTP},
		{"IE_AppBytesSend_LTE_HTTP",IE_AppBytesSend_LTE_HTTP},
		{"IE_AppBytesSend_HSPA_PLUS_HTTP",IE_AppBytesSend_HSPA_PLUS_HTTP},
		{"IE_AppBytesSend_HSUPA_HTTP",IE_AppBytesSend_HSUPA_HTTP},
		{"IE_AppBytesSend_R4_HTTP",IE_AppBytesSend_R4_HTTP},
		{"IE_AppBytesSend_EVDO_HTTP",IE_AppBytesSend_EVDO_HTTP},
		{"IE_AppBytesSend_1x_HTTP",IE_AppBytesSend_1x_HTTP},
		{"IE_AppBytesSend_GPRS_EDGE_HTTP",IE_AppBytesSend_GPRS_EDGE_HTTP},
		{"IE_App_currentThroughput_dl_HTTP",IE_App_currentThroughput_dl_HTTP},
		{"IE_App_currentThroughput_ul_HTTP",IE_App_currentThroughput_ul_HTTP},
		{"IE_AppBytesReceived_Total_STREAM",IE_AppBytesReceived_Total_STREAM},
		{"IE_AppBytesReceived_LTE_STREAM",IE_AppBytesReceived_LTE_STREAM},
		{"IE_AppBytesReceived_HSPA_PLUS_STREAM",IE_AppBytesReceived_HSPA_PLUS_STREAM},
		{"IE_AppBytesReceived_HSDPA_STREAM",IE_AppBytesReceived_HSDPA_STREAM},
		{"IE_AppBytesReceived_R4_STREAM"	,IE_AppBytesReceived_R4_STREAM},
		{"IE_AppBytesReceived_EVDO_STREAM",IE_AppBytesReceived_EVDO_STREAM},
		{"IE_AppBytesReceived_1x_STREAM",IE_AppBytesReceived_1x_STREAM},
		{"IE_AppBytesReceived_GPRS_EDGE_STREAM",IE_AppBytesReceived_GPRS_EDGE_STREAM},
		{"IE_AppBytesend_Total_STREAM",IE_AppBytesend_Total_STREAM},
		{"IE_AppBytesSend_LTE_STREAM",IE_AppBytesSend_LTE_STREAM},
		{"IE_AppBytesSend_HSPA_PLUS_STREAM",IE_AppBytesSend_HSPA_PLUS_STREAM},
		{"IE_AppBytesSend_HSUPA_STREAM",IE_AppBytesSend_HSUPA_STREAM},
		{"IE_AppBytesSend_R4_STREAM",IE_AppBytesSend_R4_STREAM},
		{"IE_AppBytesSend_EVDO_STREAM",IE_AppBytesSend_EVDO_STREAM},
		{"IE_AppBytesSend_1x_STREAM",IE_AppBytesSend_1x_STREAM},
		{"IE_AppBytesSend_GPRS_EDGE_STREAM",IE_AppBytesSend_GPRS_EDGE_STREAM},
		{"IE_App_currentThroughput_dl_STREAM",IE_App_currentThroughput_dl_STREAM},
		{"IE_App_currentThroughput_ul_STREAM",IE_App_currentThroughput_ul_STREAM},
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
