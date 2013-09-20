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
#ifndef __CSIGNAL_H__
#define __CSIGNAL_H__
#include <signal.h>
#include "machine.h"

/**
 * TODO : comment
 * @param sig
 */
void controlC_handler (int sig);


/**
 * Set the CTRL+C behaviour (stop the current execution
 * @return 0 if the signal has been set -1 otherwise
 */
int csignal(void);

#endif /* __CSIGNAL_H__ */
