/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __JSON_RPC_HXX__
#define __JSON_RPC_HXX__

#include <string>

size_t json_rpc_decode(const std::string& data, std::string& json_string);
size_t json_rpc_encode(const std::string& json_string, std::string& data);

#endif /* !__JSON_RPC_HXX__ */
