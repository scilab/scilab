
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GETSCILABOJECT_H__
#define __GETSCILABOJECT_H__

#include <jni.h> /* jobject */

/**
* returns jobject on Scilab main
* @return jobject
*/
jobject getScilabObject(void);

#endif /* __GETSCILABOJECT_H__ */
/*--------------------------------------------------------------------------*/ 
