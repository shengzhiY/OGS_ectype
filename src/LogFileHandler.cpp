/*******************************************************************************
* EPIPHYLLUM application
* Copyright (c) 2022 CGN
*
* this file is part of EPIPHYLLUM
*
* @file:   LogFileHandler.h
* @brief:  摘要
* @detail: 主要负责log日志文件的控制盒输出
*
* @author:  p622829@cnt.com.cn
* @version: 1.0.0
* @date:    2022/08/10
* @history:
* @license:
*******************************************************************************/

#include <time.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <osg/LogFileHandler.h>

using namespace osg;

//static std::string LEVELS[] = { "[ALWAYS] ","[ FATAL] ","[ WARN ] ","[NOTICE] ","[ INFO ] ","[ DEBUG] ","[DEBUG_FP] " };
static std::string LEVELS[6] = {"[SYSERR] ", "[ FATAL] ", "[ ERROR] ", "[ WARN ] ", "[ INFO ] ", "[ DEBUG] " };

// 日志对象的创建和路径设置及打开日志
LogFileHandler::LogFileHandler(osg::ArgumentParser& argument,std::string file,std::string errFile)
{
	if (0 == file.compare("log.txt"))
	{
		file = argument.getApplicationName();
		const std::string str = "/";

		// 获取input.json5文件路径
		std::string::iterator iter = std::find_end(file.begin(), file.end(), str.begin(), str.end());
		file.replace(iter + 1, file.end(), "log.txt");
		_log.open(file.c_str());
	}
	else
	{
		_log.open(file);
		_logErr.open(errFile);
	}	
}

// 日志文件关闭
LogFileHandler::~LogFileHandler(void)
{
	//m_log.flush();
	_log.close();
	_logErr.close();
}

// 日志的等级、时间、日志消息的记录
void LogFileHandler::notify(osg::NotifySeverity severity, const char *message)
{
	if((*message == '\0') || (message[0] == '\0'))
		return ;
	const char* msg = strstr(message, "::");

	if (nullptr == msg)
	{
		msg = message;
	}
	else
	{
		msg = msg + 2;
	}
	
	if (severity <= _ERROR)
	{
		_logErr << LEVELS[severity] << getCurTimeStr() << "  " << msg;
		_logErr.flush();
	}
	else 
	{
		_log << LEVELS[severity] << getCurTimeStr() << "  " << msg;
		_log.flush();
	}  
}

// 每条日志的记录是的时刻
std::string LogFileHandler::getCurTimeStr()
{
	time_t t = time(0); 
	char tmp[20] = {0};
	if (localtime(&t) != nullptr)
	{
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));
	}
	std::string tStr = tmp;
	return tStr;
	//time_t rawtime;
	//struct tm* timeinfo;
	//time(&rawtime);
	//timeinfo = localtime(&rawtime);

	//char imgeName[255] = { 0 };
	//sprintf(imgeName, "%4d:%02d:%02d:%02d:%02d:%02d", 1900 + timeinfo->tm_year,
	//	1 + timeinfo->tm_mon,
	//	timeinfo->tm_mday,
	//	timeinfo->tm_hour,
	//	timeinfo->tm_min,
	//	timeinfo->tm_sec);

	//return imgeName;
}

