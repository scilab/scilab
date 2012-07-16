/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Interface functions between between SetProperty functions and the C++/Java part of module
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetJavaProperty.h"
#include "GiwsException.hxx"


extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

using namespace org_scilab_modules_renderer;

/*---------------------------------------------------------------------------------*/
void sciJavaUpdateSubwinScale(char * pSubwinUID)
{
    CallRenderer::updateSubwinScale(getScilabJavaVM(), pSubwinUID);
}

/*---------------------------------------------------------------------------------*/
void sciJavaUpdateTextBoundingBox(char * pTextUID)
{
    CallRenderer::updateTextBounds(getScilabJavaVM(), pTextUID);
}

/*---------------------------------------------------------------------------------*/
