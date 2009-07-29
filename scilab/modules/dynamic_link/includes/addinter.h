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

#ifndef __ADDINTER_H__
#define  __ADDINTER_H__

/** 
* the first dynamic interface is at position 500+1 
*/
#define DynInterfStart 500

#define NumberMaxFunctionsByGateway 1000

#define MAXDYNINTERF 50 /* default value compatibility scilab 4.x */
/* !!! WARNING !!! */
/* On Windows , AddInterfaceToScilab based on LoadLibrary C function */
/* you cannot load more than 80 dynamic libraries at the same time. */
/* Scilab will return a error (code Windows 1114) in this case.*/
/* A dynamic link library (DLL) initialization routine failed. */


/**
* Remove interface in scilab
* @param[in] id of interface
*/
void RemoveInterf(int id);

/**
* Add a interface in Scilab
* @param[in] filenamelib dynamic libraries filename
* @param[in] spname Name of interface routine entry point 
* @param[in] fcts Name of new Scilab function implemented in the new interface 
* @param[in] sizefcts size of fcts array
* @return a int id of interface (-1 failed)
*/
int AddInterfaceToScilab(char *filenamelib,char *spname,char **fcts,int sizefcts);

#endif /*  __ADDINTER_H__ */
