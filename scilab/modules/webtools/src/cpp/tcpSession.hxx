/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SCI_TCPSESSION_HXX__
#define __SCI_TCPSESSION_HXX__

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0A00 // Windows 10
#endif

#include <threads/thread.h>
#include <server/asio/tcp_server.h>
#include "internal.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

class WEBTOOLS_IMPEXP ScilabSession
{
public:
    typedef std::function<void()> OnConnected;
    typedef std::function<void()> OnDisconnected;
    typedef std::function<void(const std::vector<uint8_t>&)> OnMessage;
    typedef std::function<void(int, const std::string&, const std::string&)> OnError;

    void setOnConnected(const OnConnected& func) { m_onConnected = func; }
    void setOnDisconnected(const OnDisconnected& func) { m_onDisconnected = func;}
    void setOnError(const OnError& func) { m_onError = func;}
    void setOnMessage(const OnMessage& func) { m_onMessage = func;}

protected:
    OnConnected m_onConnected;
    OnDisconnected m_onDisconnected;
    OnMessage m_onMessage;
    OnError m_onError;
};

class SciTcpSession : public CppServer::Asio::TCPSession, public ScilabSession
{
public:
    // use TCPSession constructor
    using CppServer::Asio::TCPSession::TCPSession;

protected:
    void onReceived(const void* buffer, size_t size) override
    {
        if (m_onMessage)
        {
            std::vector<uint8_t> buf((uint8_t*)buffer, (uint8_t*)buffer + size);
            m_onMessage(buf);
        }
    }

    void onConnected() override
    {
        if(m_onConnected)
        {
            m_onConnected();
        }
    }

    void onDisconnected() override
    {
        if(m_onDisconnected)
        {
            m_onDisconnected();
        }
    }

    void onError(int error, const std::string& category, const std::string& message) override
    {
        if(m_onError)
        {
            m_onError(error, category, message);
        }
    }
};


#endif /* !__SCI_TCPSESSION_HXX__ */
