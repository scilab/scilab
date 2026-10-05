/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef ARROW_LIB_HXX
#define ARROW_LIB_HXX

#include <stdint.h>
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct ArrowData_t ArrowData;

    struct ArrowData_t {
        types::String* pColNames;
        types::String* pColTypes;
        types::List* pData;
        int status;
    };

    ArrowData_t arrow_read(const char* filename);

    int arrow_write(const char* filename, ArrowData* raw_data);
    int parquet_write(const char* filename, ArrowData* raw_data, types::String* pCompression);

#ifdef __cplusplus
}
#endif

#endif /* !ARROW_LIB_HXX */