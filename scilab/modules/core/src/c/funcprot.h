/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __FUNCPROT_H__
#define __FUNCPROT_H__
/*--------------------------------------------------------------------------*/
#include "machine.h" /* C2F */
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
typedef struct
{
    int macprt;
} MPROT_struct;
/*--------------------------------------------------------------------------*/
/**
* initialize funcprot value
* see inisci.f
* @return 0
*/
int C2F(initfuncprot)();

/**
* get funcprot value
* return current value of funcprot
*/
int getfuncprot(void);

/**
* set funcprot value
* @param[in] level 0 1 2
* @return TRUE if value set
*/
BOOL setfuncprot(int level);
/*--------------------------------------------------------------------------*/
#endif /* __FUNCPROT_H__ */
