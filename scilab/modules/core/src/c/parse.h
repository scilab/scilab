/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __PARSE_H__
#define __PARSE_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/

/**
 * Parse a the global variable C2F(cha1).buf (ugly ...)
 *
 * @return <ReturnValue>
 */
int C2F(parse)(void);

/**
 * @TODO : add comment
 *
 * @param n 
 * @param ierr  
 */
void Msgs(int n,int ierr);

/**
 * @TODO : add comment
 *
 * @param n 
 * @return <ReturnValue>
 */
int Eptover(int n);

/**
 * @TODO : add comment
 *
 * @param n 
 * @return <ReturnValue>
 */
int Ptover(int n);

/**
 * @TODO : add comment
 *
 * @param void  
 */
void C2F(parsecomment)(void);
/*--------------------------------------------------------------------------*/
#endif /* __PARSE_H__ */
/*--------------------------------------------------------------------------*/
