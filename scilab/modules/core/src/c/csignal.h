/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
