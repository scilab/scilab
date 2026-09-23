/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "lspEndpoint.hxx"

extern "C"
{
#include "lsp_interface.h"
}

void lspStartTcp(int port, bool spawned)
{
    LspEndpoint::getInstance().startTcp(port, spawned);
}

void lspStartWs(int port, const char* token, bool spawned)
{
    LspEndpoint::getInstance().startWs(port, token ? std::string(token) : std::string(), spawned);
}

void lspIndexDocument(const char* uri, const char* text, int version)
{
    LspEndpoint::getInstance().indexDocument(uri ? uri : "", text ? text : "", version);
}

void lspRemoveIndexedDocument(const char* uri)
{
    LspEndpoint::getInstance().removeIndexedDocument(uri ? uri : "");
}
