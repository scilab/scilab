
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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
