
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __MSGOUT_H__
#define __MSGOUT_H__

#include "machine.h"
/**
 * TODO : comment
 * @param io
 * @param lunit
 * @param str
 * @param str_len
 */
int C2F(msgout)(int *io, int *lunit, char *str, long int str_len);

#endif /* __MSGOUT_H__ */
