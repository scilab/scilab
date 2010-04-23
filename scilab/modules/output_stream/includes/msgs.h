/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __MSGS_H__
#define __MSGS_H__

#include "dynlib_output_stream.h"

/**
 * @TODO : comment
 *
 * @param n 
 * @param ierr  
 * @return <ReturnValue>
 */
OUTPUT_STREAM_IMPEXP int C2F(msgs)(int *n, int *ierr);


/**
* @TODO : add comment
*
* @param n 
* @param ierr  
*/
OUTPUT_STREAM_IMPEXP void Msgs(int n,int ierr);


#endif /* __MSGS_H__ */
