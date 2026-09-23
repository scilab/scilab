/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SCI_TCPCLIENT_HXX__
#define __SCI_TCPCLIENT_HXX__

#include <server/asio/tcp_client.h>
#include <threads/thread.h>
#include "internal.hxx"
#include "tcpSession.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

class ScilabClient
{
public:
    typedef std::function<void()> OnConnected;
    typedef std::function<void()> OnDisconnected;
    typedef std::function<void(const std::vector<uint8_t>&)> OnMessage;
    typedef std::function<void(int, const std::string&, const std::string&)> OnError;

    void setOnConnected(const OnConnected& func) { m_onConnected = func; }
    void setOnDisconnected(const OnDisconnected& func) { m_onDisconnected = func; }
    void setOnMessage(const OnMessage& func) { m_onMessage = func; }
    void setOnError(const OnError& func) { m_onError = func; }

protected:
    OnConnected m_onConnected;
    OnDisconnected m_onDisconnected;
    OnError m_onError;
    OnMessage m_onMessage;
};

class WEBTOOLS_IMPEXP SciTcpClient : public CppServer::Asio::TCPClient, public ScilabClient
{
public:
    // use TCPClient constructor
    using CppServer::Asio::TCPClient::TCPClient;

protected:
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
        if (m_onError)
        {
            m_onError(error, category, message);
        }
    }

    void onReceived(const void* buffer, size_t size) override
    {
        if (m_onMessage)
        {
            std::vector<uint8_t> buf((uint8_t*)buffer, (uint8_t*)buffer + size);
            m_onMessage(buf);
        }
    }
};


#endif /* !__SCI_TCPCLIENT_HXX__ */
