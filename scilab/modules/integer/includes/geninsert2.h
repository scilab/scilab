
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

#ifndef __GENINSERT2_H__
#define __GENINSERT2_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param mj
 * @param mi
 * @param indxj
 * @param indxi
 * @param to
 * @param mr
 * @param from
 * @param m3
 * @param inc3
 * @return
 */
INTEGER_IMPEXP int C2F(geninsert2)(int *typ, int *mj, int *mi, int *indxj, int *indxi, int *to, int *mr, int *from, int *m3, int *inc3);
#endif /* __GENINSERT2_H__ */
