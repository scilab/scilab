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

typedef struct
{
    wchar_t* pwstToken;
    types::InternalType::RealType outputType;
    bool bLengthFlag;
} TokenDef;


typedef struct
{
    int iArg;
    int iPos;
    types::InternalType::RealType type;
} ArgumentPosition;

OUTPUT_STREAM_IMPEXP_GW wchar_t** scilab_sprintf(wchar_t* _pwstName, wchar_t* _pwstInput, types::typed_list &in, ArgumentPosition* _pArgs, int _iArgsCount, int* _piOutputRows, int* piNewLine);


#endif /* !__SCILAB_SPRINTF_HXX__ */
