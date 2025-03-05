/*******************************************************************************
* EPIPHYLLUM application
* Copyright (c) 2022 CGN
*
* this file is part of EPIPHYLLUM
*
* @file:   LogFileHandler.h
* @brief:  摘要
* @detail: 主要负责log日志文件的控制盒输出API
*
* @author:  p622829@cnt.com.cn
* @version: 1.0.0
* @date:    2022/08/10
* @history:
* @license:
*******************************************************************************/

#ifndef LOGFILEHANDLER_H
#define LOGFILEHANDLER_H 1

#include <fstream>

#include <osg/Notify>
#include <osg/ArgumentParser>
static char _sMsg[256]; //jaslog
namespace osg
{

// 日志处理类
class OSG_EXPORT LogFileHandler :public osg::NotifyHandler
{
public:
	//************************************
	// Method:    LogFileHandler 【日志处理类构造函数】
	// FullName:  LogFileHandler::LogFileHandler
	// Access:    public
	// Returns:   
	// Parameter: const std::string& logFile 【文件名称】
	//************************************
    LogFileHandler(osg::ArgumentParser& argument, std::string logFile = "log.txt",std::string errFile = "logErr.txt");

	//************************************
	// Method:    ~LogFileHandler 【日志处理类析构函数】
	// FullName:  LogFileHandler::~LogFileHandler
	// Access:    public
	// Returns:   
	// Parameter: 
	//************************************
    ~LogFileHandler(void);

	//************************************
	// Method:    getCurTimeStr 【获取当前时间】
	// FullName:  LogFileHandler::getCurTimeStr
	// Access:    public
	// Returns:   std::string 【当前时间字符串】
	// Parameter: 
	//************************************
    static std::string getCurTimeStr();

	//************************************
	// Method:    notify 【获取当前时间】
	// FullName:  LogFileHandler::notify
	// Access:    public
	// Returns:   void
	// Parameter: osg::NotifySeverity severity 【日志通知等级】
	// Parameter: const char* message 【消息】
	//************************************
    virtual void notify(osg::NotifySeverity severity, const char* message);

protected:

    std::ofstream _log; // 日志文件输出流对象
	std::ofstream _logErr; // 错误日志文件输出流对象
};
}


#endif //LOGFILEHANDLER_H
