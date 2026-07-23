/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __CALLCOMMAND_H__
#define __CALLCOMMAND_H__

#include "dynlib_core.h"

/**
 * Call a command from a gateway (a Scilab runner should pre-exist)
 * 
 * \param command: input, string containing the command to execute
 * \return the execution status (0 if the command is valid and has been executed to its end)
 *
 */
CORE_IMPEXP int callCommandFromGateway(const char* command);


#endif /* !__CALLCOMMAND_H__ */
