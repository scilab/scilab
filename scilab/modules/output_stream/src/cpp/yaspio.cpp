/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */


#include "yaspio.hxx"

extern "C"
{
#include "charEncoding.h"
#include "MALLOC.h"
}

static YASP_OUTPUT _writer;
static YASP_INPUT _reader;

void setYaspOutputMethod(YASP_OUTPUT writer)
{
    _writer = writer;
}

void setYaspInputMethod(YASP_INPUT reader)
{
    _reader = reader;
}

char *YaspRead()
{
    return (*_reader)();
}

void YaspWrite(const char* text)
{
    (*_writer)(const_cast<char*>(text));
}

void YaspWriteW(const wchar_t* text)
{
    char* pstTemp = wide_string_to_UTF8(text);
    (*_writer)(pstTemp);
    FREE(pstTemp);
}
