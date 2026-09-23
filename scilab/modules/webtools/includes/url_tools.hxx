/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#ifndef __URL_DECODE_H__
#define __URL_DECODE_H__

#include <string>

#include "dynlib_webtools.h"

WEBTOOLS_IMPEXP int url_decode(const std::string& str, std::string& out);
WEBTOOLS_IMPEXP int url_encode(const std::string& str, std::string& out);
WEBTOOLS_IMPEXP int url_split(const std::string& str, 
    std::string& scheme, std::string& server, std::string& path, std::string& query, 
    std::string& user, std::string& password, std::string& port, std::string& fragment);

#endif /* __URL_DECODE_H__ */

