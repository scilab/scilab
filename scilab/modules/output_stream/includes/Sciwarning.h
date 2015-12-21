/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2015 - Pierre-Aime Agnel
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef SCIWARNING_H
#define SCIWARNING_H

#include "dynlib_output_stream.h"

OUTPUT_STREAM_IMPEXP int Sciwarning(const char *msg, ...);

#endif // SCIWARNING_H
