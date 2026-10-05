/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>
#include <sstream>

#include "spreadsheet_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "int.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "cell.hxx"


extern "C"
{
    #include "Scierror.h"
    #include "sciprint.h"
    #include "localization.h"
    #include "arrow_lib.h"
}

/* ==================================================================== */
types::Function::ReturnValue sci_parquetWrite(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 5 && in.size() != 6)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "parquetWrite", 5, 6);
        return types::Function::Error;
    }

    // filename
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "parquetWrite", 1);
        return types::Function::Error;
    }

    // extension - parquet or arrow
    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), "parquetWrite", 2);
        return types::Function::Error;
    }

    // data - list
    if (in[2]->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A list expected.\n"), "parquetWrite", 3);
        return types::Function::Error;
    }

    // column names - string
    if (in[3]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), "parquetWrite", 4);
        return types::Function::Error;
    }

    // column types - string
    if (in[4]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A double expected.\n"), "parquetWrite", 5);
        return types::Function::Error;
    }    

    types::String* pStr = in[0]->getAs<types::String>();
    char* filename = wide_string_to_UTF8(pStr->get(0));

    types::String* file_e = in[1]->getAs<types::String>();
    char* file_extension = wide_string_to_UTF8(file_e->get(0));

    types::String* pCompression = nullptr;
    if (in.size() == 6)
    {
        if (in[5]->isString() == false)
        {
            FREE(filename);
            FREE(file_extension);
            Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), "parquetWrite", 6);
            return types::Function::Error;
        }
        pCompression = in[5]->getAs<types::String>();
    }

    types::List* pList = in[2]->getAs<types::List>();
    int cols = pList->getSize();

    types::String* pMetaColumnNames = in[3]->getAs<types::String>();
    types::String* pMetaColumnTypes = in[4]->getAs<types::String>();

    if (pMetaColumnNames->getSize() != cols)
    {
        FREE(filename);
        FREE(file_extension);
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of %d columns expected.\n"), "parquetWrite", 4, cols);
        return types::Function::Error;
    }

    if (pMetaColumnTypes->getSize() != cols)
    {
        FREE(filename);
        FREE(file_extension);
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of %d columns expected.\n"), "parquetWrite", 5, cols);
        return types::Function::Error;
    }

    if (pCompression != nullptr && pCompression->getSize() != 1 && pCompression->getSize() != cols)
    {
        FREE(filename);
        FREE(file_extension);
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string or a vector of %d strings expected.\n"), "parquetWrite", 6, cols);
        return types::Function::Error;
    }

    ArrowData st = {pMetaColumnNames, pMetaColumnTypes, pList, 0};
    int result = 0;

    if (strcmp(file_extension, ".parquet") == 0)
    {
        result = parquet_write(filename, &st, pCompression);
    }
    else if (strcmp(file_extension, ".arrow") == 0)
    {
        result = arrow_write(filename, &st);
    }
    else
    {
        Scierror(999, _("%s: Wrong extension for input argument #%d: Supported extensions are %s and %s.\n"), "parquetWrite", 1, "parquet", "arrow");
        FREE(filename);
        FREE(file_extension);
        return types::Function::Error;
    }

    if (result == 0)
    {
        FREE(filename);
        FREE(file_extension);
        return types::Function::OK;
    }
    else
    {
        // check error
        switch (result)
        {
            case -1:
            {
                Scierror(999, _("%s: Invalid parameters.\n"), "parquetWrite");
                break;
            }
            case -2:
            {
                Scierror(999, _("%s: Can not open file %s.\n"), "parquetWrite", filename);
                break;
            }
            case -3:
            {
                Scierror(999, _("%s: Impossible to create table.\n"), "parquetWrite");
                break;
            }
            case -4:
            {
                Scierror(999, _("%s: Impossible to create file writer.\n"), "parquetWrite");
                break;
            }
            case -5:
            {
                Scierror(999, _("%s: Impossible to write table.\n"), "parquetWrite");
                break;
            }
            case -6:
            {
                Scierror(999, _("%s: Impossible to close writer.\n"), "parquetWrite");
                break;
            }
            case -7:
            {
                Scierror(999, _("%s: Impossible to close  file.\n"), "parquetWrite");
                break;
            }
        }
        FREE(filename);
        FREE(file_extension);
        return types::Function::Error;
    }
}