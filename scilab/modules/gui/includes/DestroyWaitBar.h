/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DESTROY_WAITBAR_H__
#define __DESTROY_WAITBAR_H__

#include "dynlib_gui.h"
#include "ObjectStructure.h"
#include "returnProperty.h"

/**
 * Destroy a WaitBar
 *
 * @param pobj Scilab object corresponding to the WaitBar
 */
GUI_IMPEXP void DestroyWaitBar(sciPointObj * pobj);

#endif
