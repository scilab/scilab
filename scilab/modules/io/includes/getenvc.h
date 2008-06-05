/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GETENVC_H__
#define __GETENVC_H__
#include "machine.h"

/**
 * Retrieve a system variable 
 * @param[out] ierr the error code
 * @param[in] var the name of the variable
 * @param[out] buf the value of the variable
 * @param[out] buflen  the length of the variable
 * @param[in] iflag 
 */
void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag);

/**
* Searches for a file using environment paths
* @param[in] filename
* @param[in] environment variable where we search
* @return path found
*/
char *searchEnv(const char *name,const char *env_var);


#endif /* __GETENVC_H__ */
