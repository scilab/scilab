/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
