/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "dapEndpoint.hxx"

extern "C"
{
#include "dap_interface.h"
}

void dapStartTcp(int port, bool spawned)
{
    DapEndpoint::getInstance().startTcp(port, spawned);
}

void dapStartWs(int port, const char* token, bool spawned)
{
    DapEndpoint::getInstance().startWs(port, token ? std::string(token) : std::string(), spawned);
}

void dapShutdown(void)
{
    DapEndpoint::getInstance().shutdown();
}
