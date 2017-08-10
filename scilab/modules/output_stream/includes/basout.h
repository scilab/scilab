
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

#ifndef __BASOUT_H__
#define __BASOUT_H__

#include "machine.h" /* C2F */
#include "dynlib_output_stream.h"

OUTPUT_STREAM_IMPEXP int C2F(basout)(int *io, int *lunit, char *string, long int nbcharacters);

#endif /* __BASOUT_H__ */
