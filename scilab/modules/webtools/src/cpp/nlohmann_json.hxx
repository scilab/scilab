/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __NLOHMANN_JSON_HXX__
#define __NLOHMANN_JSON_HXX__

/*
 * JSON_DIAGNOSTICS changes nlohmann::json's ABI namespace. Configure it in
 * one place, before json.hpp is included, so every translation unit uses the
 * same json type in public function signatures.
 */
#ifndef JSON_DIAGNOSTICS
    #ifdef NDEBUG
        #define JSON_DIAGNOSTICS 0
    #else
        #define JSON_DIAGNOSTICS 1
    #endif
#endif

#include "nlohmann/json.hpp"

#endif /* !__NLOHMANN_JSON_HXX__ */
