/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * Copyright (C) ENPC
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#ifndef __TEXMACS_H__
#define __TEXMACS_H__

#include "machine.h" /* C2F */

void settexmacs(void);
int  C2F(intexmacs)(void);
void next_input (void);
void C2F(texmacsin)(char buffer[], int *buf_size, int *len_line, int *eof, long int dummy1);

#endif /* __TEXMACS_H__ */
