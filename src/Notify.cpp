/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/
#include <osg/Notify>
#include <osg/ApplicationUsage>
#include <osg/os_utils>
#include <osg/ref_ptr>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <ctype.h>
#ifdef  WIN32
#include <windows.h>
#endif //  WIN32

#define OSG_INIT_SINGLETON_PROXY(ProxyName, Func) static struct ProxyName{ ProxyName() { Func; } } s_##ProxyName;

namespace osg
{

class NullStreamBuffer : public std::streambuf
{
private:
    std::streamsize xsputn(const std::streambuf::char_type *str, std::streamsize n)
    {
		OSG_UNUSED(str);
        return n;
    }
};

struct NullStream : public std::ostream
{
public:
    NullStream():
        std::ostream(new NullStreamBuffer)
    {
        _buffer = static_cast<NullStreamBuffer *>(rdbuf());
    }

    ~NullStream()
    {
        rdbuf(0);
        delete _buffer;
    }

protected:
    NullStreamBuffer* _buffer;
};

/** Stream buffer calling notify handler when buffer is synchronized (usually on std::endl).
 * Stream stores last notification severity to pass it to handler call.
 */
struct NotifyStreamBuffer : public std::stringbuf
{
    NotifyStreamBuffer() : _severity(osg::DEBUG)
    {
        /* reduce the need to reallocate the std::ostream buffer behind osg::Notify (causing multitreading issues) by pre-allocating 4095 bytes */
        str(std::string(4095, 0));
        pubseekpos(0, std::ios_base::out);
    }

    void setNotifyHandler(osg::NotifyHandler *handler) { _handler = handler; }
    osg::NotifyHandler *getNotifyHandler() const { return _handler.get(); }

    /** Sets severity for next call of notify handler */
    void setCurrentSeverity(osg::NotifySeverity severity)
    {
        if (_severity != severity)
        {
            sync();
            _severity = severity;
        }
    }

    osg::NotifySeverity getCurrentSeverity() const { return _severity; }

private:

    int sync()
    {
        sputc(0); // string termination
        if (_handler.valid())
            _handler->notify(_severity, pbase());
        pubseekpos(0, std::ios_base::out); // or str(std::string())
        return 0;
    }

    osg::ref_ptr<osg::NotifyHandler> _handler;
    osg::NotifySeverity _severity;
};

struct NotifyStream : public std::ostream
{
public:
    NotifyStream():
        std::ostream(new NotifyStreamBuffer)
    {
        _buffer = static_cast<NotifyStreamBuffer *>(rdbuf());
    }

    void setCurrentSeverity(osg::NotifySeverity severity)
    {
        _buffer->setCurrentSeverity(severity);
    }

    osg::NotifySeverity getCurrentSeverity() const
    {
        return _buffer->getCurrentSeverity();
    }

    ~NotifyStream()
    {
        rdbuf(0);
        delete _buffer;
    }

protected:
    NotifyStreamBuffer* _buffer;
};

}

using namespace osg;

static osg::ApplicationUsageProxy Notify_e0(osg::ApplicationUsage::ENVIRONMENTAL_VARIABLE, "OSG_NOTIFY_LEVEL <mode>", "FATAL | WARN | NOTICE | DEBUG_INFO | DEBUG_FP | DEBUG | INFO | ALWAYS");

struct NotifySingleton
{
    NotifySingleton()
    {
        // _notifyLevel
        // =============

        _notifyLevel = osg::DEBUG; // Default value

        std::string OSGNOTIFYLEVEL;
        if(getEnvVar("OSG_NOTIFY_LEVEL", OSGNOTIFYLEVEL) || getEnvVar("OSGNOTIFYLEVEL", OSGNOTIFYLEVEL))
        {

            std::string stringOSGNOTIFYLEVEL(OSGNOTIFYLEVEL);

            // Convert to upper case
            for(std::string::iterator i=stringOSGNOTIFYLEVEL.begin();
                i!=stringOSGNOTIFYLEVEL.end();
                ++i)
            {
                *i=toupper(*i);
            }
			if (stringOSGNOTIFYLEVEL.find("SYSERR") != std::string::npos)          _notifyLevel = osg::SYSERR;
            else if(stringOSGNOTIFYLEVEL.find("FATAL")!=std::string::npos)          _notifyLevel=osg::FATAL;
            else if(stringOSGNOTIFYLEVEL.find("ERROR")!=std::string::npos)      _notifyLevel=osg::_ERROR;
            else if(stringOSGNOTIFYLEVEL.find("WARN")!=std::string::npos)       _notifyLevel=osg::WARN; 
            else if(stringOSGNOTIFYLEVEL.find("INFO")!=std::string::npos)	    _notifyLevel=osg::INFO;
            else if(stringOSGNOTIFYLEVEL.find("DEBUG")!=std::string::npos)       _notifyLevel=osg::DEBUG;
            else std::cout << "Warning: invalid OSG_NOTIFY_LEVEL set ("<<stringOSGNOTIFYLEVEL<<")"<<std::endl;

        }

        // Setup standard notify handler
        osg::NotifyStreamBuffer *buffer = dynamic_cast<osg::NotifyStreamBuffer *>(_notifyStream.rdbuf());
        if (buffer && !buffer->getNotifyHandler())
            buffer->setNotifyHandler(new StandardNotifyHandler);
    }
	
public:
	///using Ptr = std::shared_ptr<NotifySingleton> ;
    osg::NotifySeverity _notifyLevel;
    osg::NullStream     _nullStream;
    osg::NotifyStream   _notifyStream;
private:

};

static NotifySingleton& getNotifySingleton()
{
    static NotifySingleton s_NotifySingleton;
    return s_NotifySingleton;
}


bool osg::initNotifyLevel()
{
    getNotifySingleton();
    return true;
}

// Use a proxy to force the initialization of the NotifySingleton during static initialization
//OSG_INIT_SINGLETON_PROXY(NotifySingletonProxy, osg::initNotifyLevel())

void osg::setNotifyLevel(osg::NotifySeverity severity)
{
	//std::shared_ptr<NotifySingleton> notify = getNotifySingleton();
	getNotifySingleton()._notifyLevel = severity;
}

osg::NotifySeverity osg::getNotifyLevel()
{
    return getNotifySingleton()._notifyLevel;
}

void osg::setNotifyHandler(osg::NotifyHandler *handler)
{
    osg::NotifyStreamBuffer *buffer = static_cast<osg::NotifyStreamBuffer*>(getNotifySingleton()._notifyStream.rdbuf());
    if (buffer) buffer->setNotifyHandler(handler);
}

osg::NotifyHandler* osg::getNotifyHandler()
{
    osg::NotifyStreamBuffer *buffer = static_cast<osg::NotifyStreamBuffer *>(getNotifySingleton()._notifyStream.rdbuf());
    return buffer ? buffer->getNotifyHandler() : 0;
}


#ifndef OSG_NOTIFY_DISABLED
bool osg::isNotifyEnabled( osg::NotifySeverity severity )
{
    return severity<=getNotifySingleton()._notifyLevel;
}
#endif

std::ostream& osg::notify(const osg::NotifySeverity severity)
{
    if (osg::isNotifyEnabled(severity))
    {
        getNotifySingleton()._notifyStream.setCurrentSeverity(severity);
        return getNotifySingleton()._notifyStream;
    }
    return getNotifySingleton()._nullStream;
}

void osg::StandardNotifyHandler::notify(osg::NotifySeverity severity, const char *message)
{
	if (severity <= osg::WARN)
	{
		fputs(message, stderr);
	}
	else
	{
		fputs(message, stdout);
	}
}

#if defined(WIN32) && !defined(__CYGWIN__)

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

void osg::WinDebugNotifyHandler::notify(osg::NotifySeverity severity, const char *message)
{
    OSG_UNUSED(severity);

    OutputDebugStringA(message);
}

#endif
