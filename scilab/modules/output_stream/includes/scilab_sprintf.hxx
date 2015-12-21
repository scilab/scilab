/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILAB_SPRINTF_HXX__
#define __SCILAB_SPRINTF_HXX__

#include "dynlib_output_stream_gw.h"
#include "internal.hxx"

struct TokenDef
{
    wchar_t* pwstToken;
    types::InternalType::ScilabType outputType;
    bool length;
    int width;
    bool widthStar;
    int prec;
    bool precStar;
    int typePos;
    int pos;
    int col;

    TokenDef() :
        pwstToken(nullptr),
        outputType(types::InternalType::ScilabNull),
        length(false),
        width(0),
        widthStar(false),
        prec(0),
        precStar(false),
        typePos(0),
        pos(0),
        col(0)
    {}
};

OUTPUT_STREAM_IMPEXP_GW wchar_t** scilab_sprintf(const std::string& funcname, const wchar_t* _pwstInput, types::typed_list &in, int* _piOutputRows, int* _piNewLine);



#endif /* !__SCILAB_SPRINTF_HXX__ */
