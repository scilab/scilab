/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "inspector.hxx"
#include "sciprint.h"

types::Function::ReturnValue sci_inspectorGetItemCount(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    sciprint("\n%d\tItems\n", types::Inspector::getItemCount());
    sciprint("%d\tUnreferenced Items\n", types::Inspector::getUnreferencedItemCount());
    return types::Function::OK;
}
