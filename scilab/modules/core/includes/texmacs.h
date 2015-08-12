/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * Copyright (C) ENPC
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __TEXMACS_H__
#define __TEXMACS_H__


void settexmacs(void);
int  C2F(intexmacs)(void);
void next_input (void);
void C2F(texmacsin)(char buffer[], int *buf_size, int *len_line, int *eof, long int dummy1);

#endif /* __TEXMACS_H__ */
