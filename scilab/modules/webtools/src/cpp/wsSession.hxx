/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SCI_WSSESSION_HXX__
#define __SCI_WSSESSION_HXX__

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0A00 // Windows 10
#endif

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <server/ws/ws_session.h>

#include "internal.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

class WEBTOOLS_IMPEXP ScilabWSSession
{
public:
    typedef std::function<void()> OnConnected;
    typedef std::function<void()> OnDisconnected;
    typedef std::function<void(const std::vector<uint8_t>&)> OnMessage;
    typedef std::function<void(int, const std::string&, const std::string&)> OnError;

    void setOnConnected(const OnConnected& func) { m_onConnected = func; }
    void setOnDisconnected(const OnDisconnected& func) { m_onDisconnected = func; }
    void setOnError(const OnError& func) { m_onError = func; }
    void setOnMessage(const OnMessage& func) { m_onMessage = func; }

protected:
    OnConnected m_onConnected;
    OnDisconnected m_onDisconnected;
    OnMessage m_onMessage;
    OnError m_onError;
};

class SciWsSession : public CppServer::WS::WSSession, public ScilabWSSession
{
public:
    SciWsSession(const std::shared_ptr<CppServer::WS::WSServer>& server, std::string expected_path, std::string token)
        : CppServer::WS::WSSession(server), m_expected_path(std::move(expected_path)), m_token(std::move(token))
    {
    }

    bool SendTextMessage(const std::string& message) { return SendTextAsync(message); }

protected:
    bool onWSConnecting(const CppServer::HTTP::HTTPRequest& request, CppServer::HTTP::HTTPResponse& response) override
    {
        std::string_view url = request.url();
        if (!matchesPath(url, m_expected_path))
        {
            response.MakeErrorResponse(404, "Not found\n");
            return false;
        }

        if (!m_token.empty())
        {
            std::string_view got = queryParam(url, "token");
            if (got != m_token)
            {
                response.MakeErrorResponse(401, "Unauthorized\n");
                return false;
            }
        }

        return true;
    }

    void onWSConnected(const CppServer::HTTP::HTTPRequest& /*request*/) override
    {
        if (m_onConnected)
        {
            m_onConnected();
        }
    }

    void onWSDisconnected() override
    {
        if (m_onDisconnected)
        {
            m_onDisconnected();
        }
    }

    void onWSReceived(const void* buffer, size_t size) override
    {
        if (!m_onMessage)
        {
            return;
        }

        std::vector<uint8_t> buf((const uint8_t*)buffer, (const uint8_t*)buffer + size);
        m_onMessage(buf);
    }

    void onError(int error, const std::string& category, const std::string& message) override
    {
        if (m_onError)
        {
            m_onError(error, category, message);
        }
    }

private:
    static bool matchesPath(std::string_view url, const std::string& expected_path)
    {
        if (expected_path.empty())
        {
            return true;
        }

        if (url.size() < expected_path.size())
        {
            return false;
        }

        if (url.compare(0, expected_path.size(), expected_path) != 0)
        {
            return false;
        }

        if (url.size() == expected_path.size())
        {
            return true;
        }

        char next = url[expected_path.size()];
        return (next == '?') || (next == '#');
    }

    static std::string_view queryParam(std::string_view url, std::string_view key)
    {
        size_t q = url.find('?');
        if (q == std::string_view::npos)
        {
            return {};
        }

        std::string_view query = url.substr(q + 1);
        while (!query.empty())
        {
            size_t amp = query.find('&');
            std::string_view part = (amp == std::string_view::npos) ? query : query.substr(0, amp);

            size_t eq = part.find('=');
            std::string_view k = (eq == std::string_view::npos) ? part : part.substr(0, eq);
            std::string_view v = (eq == std::string_view::npos) ? std::string_view{} : part.substr(eq + 1);
            if (k == key)
            {
                return v;
            }

            if (amp == std::string_view::npos)
            {
                break;
            }
            query.remove_prefix(amp + 1);
        }

        return {};
    }

private:
    std::string m_expected_path;
    std::string m_token;
};

#endif /* !__SCI_WSSESSION_HXX__ */
