/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include "callJoGLView.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicJoGLView.hxx"


using namespace org_scilab_modules_gui_graphicWindow;

extern "C" {

    void createJoGLView(char* id)
    {
        return CallGraphicJoGLView::createJoGLView(getScilabJavaVM(), id);
    }

    void createSwingView(void)
    {
        return CallGraphicJoGLView::createSwingView(getScilabJavaVM());
    }

}
