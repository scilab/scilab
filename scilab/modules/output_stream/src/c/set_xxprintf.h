/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __SET_XXPRINTF_H__
#define __SET_XXPRINTF_H__
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "stack-def.h" /* bsiz */
/*--------------------------------------------------------------------------*/
typedef int (*XXPRINTF)(FILE *, char *,...);
typedef int (*FLUSH)(FILE *);
/*--------------------------------------------------------------------------*/
#define  MAX_SPRINTF_SIZE  bsiz
/*--------------------------------------------------------------------------*/

/**
* set output function  sprintf, sciprint2, fprintf
* @param[in] fp "Pointer to a FILE object"
* @param[out] xxprintf "Pointer to output function"
* @param[in] flush flush
* @param[out] target output "buffer"
*/
void set_xxprintf(FILE *fp,XXPRINTF *xxprintf,FLUSH *flush,char **target);

#endif /* __SET_XXPRINTF_H__ */
/*--------------------------------------------------------------------------*/

