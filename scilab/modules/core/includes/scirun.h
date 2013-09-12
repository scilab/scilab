/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __SCIRUN_H__
#define __SCIRUN_H__

#include "machine.h"

/**
 * Run scilab parser with initial instruction
 * @param startupCode : character string which contains the initial instruction
 * @param startupCode_len : length of startupCode
 */
int C2F(scirun)(char *startupCode, long int startupCode_len);

#endif /* __SCIRUN_H__ */
