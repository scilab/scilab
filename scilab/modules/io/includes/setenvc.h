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
#include "dynlib_io.h"
#include "BOOL.h"

/**
 * Set in the system environment a variable to a value
 * @param[in] string the name of the variable
 * @param[in] value the value of the variable "string"
 * @return FALSE if there is a problem otherwise TRUE
 */
IO_IMPEXP BOOL setenvc(char *string,char *value);

#endif /* __SETENVC_H__ */
/*--------------------------------------------------------------------------*/
