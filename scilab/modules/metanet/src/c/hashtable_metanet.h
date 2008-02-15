
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HASHTABLE_METANET_H__
#define __HASHTABLE_METANET_H__
/*--------------------------------------------------------------------------*/ 
#include "hashtable.h"
/*--------------------------------------------------------------------------*/ 
typedef struct
{
	char *key;
	char *data;
} ENTRY;
/*--------------------------------------------------------------------------*/ 
typedef enum 
{
	SCIFIND, SCIENTER
} SCIACTION;
/*--------------------------------------------------------------------------*/ 
int myhcreate(unsigned nel);
void myhdestroy(void);
ENTRY *myhsearch(ENTRY item,SCIACTION action);
/*--------------------------------------------------------------------------*/ 
#endif /* __HASHTABLE_METANET_H__ */
