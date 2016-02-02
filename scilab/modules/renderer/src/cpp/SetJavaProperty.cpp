/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Interface functions between between SetProperty functions and the C++/Java part of module
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
void sciJavaUpdateSubwinScale(int iObjUID)
{
    CallRenderer::updateSubwinScale(getScilabJavaVM(), iObjUID);
}

/*---------------------------------------------------------------------------------*/
void sciJavaUpdateTextBoundingBox(int iTextUID)
{
    CallRenderer::updateTextBounds(getScilabJavaVM(), iTextUID);
}

/*---------------------------------------------------------------------------------*/
