/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallDatatipCreateField.hxx"

using namespace org_scilab_modules_gui_datatip;

double* CallDatatipCreateField(char const* polylineUid, double const* coordDoubleXY, int coordDoubleXYSize)
{
    return DatatipCreate::createDatatipField(getScilabJavaVM(), polylineUid, coordDoubleXY, coordDoubleXYSize);
}
