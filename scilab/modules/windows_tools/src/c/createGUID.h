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
#ifndef __CREATEGUID_H__
#define __CREATEGUID_H__

/**
* Creates a GUID, a unique 128-bit int used for CLSIDs and interface identifiers.
* @return a string (GUID)
*/
char *createGUID(void);

#endif /* __CREATEGUID_H__ */
/*--------------------------------------------------------------------------*/ 
