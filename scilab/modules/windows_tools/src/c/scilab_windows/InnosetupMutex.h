/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - 2008 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __INNOSETUPMUTEX_H__
#define __INNOSETUPMUTEX_H__

/*--------------------------------------------------------------------------*/
/**
* Create a named Mutex used by Innosetup
* http://www.vincenzo.net/isxkb/index.php?title=Application_considerations 
*/
void createInnosetupMutex(void);

/**
* Close named Mutex used by Innosetup
*/
void closeInnosetupMutex(void);
/*--------------------------------------------------------------------------*/

#endif /* __INNOSETUPMUTEX_H__ */
