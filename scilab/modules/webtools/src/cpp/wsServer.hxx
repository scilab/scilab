/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SCI_WSSERVER_HXX__
#define __SCI_WSSERVER_HXX__

#include <functional>
#include <memory>
#include <string>

#include <server/ws/ws_server.h>
#include <threads/thread.h>

#include "wsSession.hxx"
#include "internal.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

class WEBTOOLS_IMPEXP ScilabWSServer
{
public:
    typedef std::function<void(const std::shared_ptr<SciWsSession>&)> OnWsConnected;
    typedef std::function<void(const std::shared_ptr<SciWsSession>&)> OnWsDisconnected;
    typedef std::function<void(int, const std::string&, const std::string&)> OnError;

    void setOnWsConnected(const OnWsConnected& func) { m_onWsConnected = func; }
    void setOnWsDisconnected(const OnWsDisconnected& func) { m_onWsDisconnected = func; }
    void setOnError(const OnError& func) { m_onError = func; }

protected:
    OnWsConnected m_onWsConnected;
    OnWsDisconnected m_onWsDisconnected;
    OnError m_onError;
};

class SciWsServer : public CppServer::WS::WSServer, public ScilabWSServer
{
public:
    using CppServer::WS::WSServer::WSServer;

    void setPath(std::string path) { m_expected_path = std::move(path); }
    void setToken(std::string token) { m_token = std::move(token); }

protected:
    std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server) override
    {
        auto ws_server = std::dynamic_pointer_cast<CppServer::WS::WSServer>(server);
        auto session = std::make_shared<SciWsSession>(ws_server, m_expected_path, m_token);

        // NOTE: Unlike the TCP server adapter (tcpServer.hxx) where the server gets
        // a session pointer from the framework and forwards it directly to callbacks,
        // WebSocket "connected" events happen *inside* the WSSession after the HTTP
        // Upgrade handshake is validated (path, token, etc.).
        //
        // We therefore install callbacks on the session to notify the server once
        // the WS handshake succeeded. Those callbacks are stored by the session
        // object itself, so capturing a strong shared_ptr<SciWsSession> here would
        // create a reference cycle:
        //   session -> (stored lambda) -> shared_ptr(session)
        // Prevent this leak by capturing only a weak_ptr and locking it on use.
        std::weak_ptr<SciWsSession> weak_session(session);

        session->setOnConnected([this, weak_session]() {
            if (m_onWsConnected)
            {
                if (auto locked = weak_session.lock())
                {
                    m_onWsConnected(locked);
                }
            }
        });
        session->setOnDisconnected([this, weak_session]() {
            if (m_onWsDisconnected)
            {
                if (auto locked = weak_session.lock())
                {
                    m_onWsDisconnected(locked);
                }
            }
        });
        session->setOnError([this](int error, const std::string& category, const std::string& message) {
            if (m_onError)
            {
                m_onError(error, category, message);
            }
        });

        return session;
    }

private:
    std::string m_expected_path;
    std::string m_token;
};

#endif /* !__SCI_WSSERVER_HXX__ */
