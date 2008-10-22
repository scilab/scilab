/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __ZZLEDT_H__
#define __ZZLEDT_H__


/**
* line editor
* @param buffer
* @param buffer size
* @param length line
* @param eof
* @param menu flag
* @param mode
* @param dummy1 (fortran)
*/
void C2F(zzledt)(char *buffer,int *buf_size,int *len_line,int * eof,int *menusflag,int * modex,long int dummy1);

#endif /* __ZZLEDT_H__ */
/*--------------------------------------------------------------------------*/


