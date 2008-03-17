/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETENVC_H__
#define __SETENVC_H__

/**
 * Set in the system environment a variable to a value
 * @param[in] string the name of the variable
 * @param[in] value the value of the variable "string"
 * @return 0 if there is a problem otherwise 1 
 */
int setenvc(char *string,char *value);

/**
 * TODO : comment 
 * @return 
 */
int getUpdateEnvVar(void);

/**
 * TODO : comment 
 * @param val
 */
void setUpdateEnvVar(int val);

#endif /* __SETENVC_H__ */
/*--------------------------------------------------------------------------*/
