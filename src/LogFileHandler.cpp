/*******************************************************************************
* EPIPHYLLUM application
* Copyright (c) 2022 CGN
*
* this file is part of EPIPHYLLUM
*
* @file:   LogFileHandler.h
* @brief:  ժҪ
* @detail: ��Ҫ����log��־�ļ��Ŀ��ƺ����
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

// ��־����Ĵ�����·�����ü�����־
LogFileHandler::LogFileHandler(osg::ArgumentParser& argument,std::string file,std::string errFile)
{
	if (0 == file.compare("log.txt"))
	{
		file = argument.getApplicationName();
		const std::string str = "/";

		// ��ȡinput.json5�ļ�·��
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

// ��־�ļ��ر�
LogFileHandler::~LogFileHandler(void)
{
	//m_log.flush();
	_log.close();
	_logErr.close();
}

// ��־�ĵȼ���ʱ�䡢��־��Ϣ�ļ�¼
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

// ÿ����־�ļ�¼�ǵ�ʱ��
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

