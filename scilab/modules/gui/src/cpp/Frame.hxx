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

#ifndef __FRAME_HXX__
#define __FRAME_HXX__

#include "CallScilabBridge.hxx"
extern "C"{
#include "Frame.h"
#include "getScilabJavaVM.h"
#include "CurrentObjectsManagement.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
}

#endif /* __FRAME_HXX__ */
