/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __LOADFUNCTIONSTAB_H__
#define __LOADFUNCTIONSTAB_H__

/**
* load all the functions from modules/xxxx/sci_gateway/xxxx_gateway.xml where xxxx is
* the name of the module
*/
void LoadFunctionsTab(void);

/**
* after a call to LoadFunctionsTab
* it is no more possible to reload table
* this function allows to reload table
* see bug 7602
*/
void unLockLoadFunctionsTab(void);


#endif /* __LOADFUNCTIONSTAB_H__ */
/*--------------------------------------------------------------------------*/

