/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
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

#ifndef _PROMPT_H_
#define _PROMPT_H_
/* Define prompts of Scilab */

#include "machine.h"            /* C2F */
#include "dynlib_console.h"

#ifdef SCIPROMPT
#undef SCIPROMPT
#endif
#define SCIPROMPT "--> "
#define SCIPROMPTDEBUG "debug> "
#define SCIPROMPTBREAK "break> "

#ifdef SCIPROMPT_PAUSE
#undef SCIPROMPT_PAUSE
#endif
#define SCIPROMPT_PAUSE ">> "

#ifdef SCIPROMPT_INTERRUPT
#undef SCIPROMPT_INTERRUPT
#endif
#define SCIPROMPT_INTERRUPT "-%d-> "

#define PROMPT_SIZE_MAX 64

/*--------------------------------------------------------------------------*/
CONSOLE_IMPEXP void C2F(setprlev) (int *pause);

CONSOLE_IMPEXP void GetCurrentPrompt(char *CurrentPrompt);

CONSOLE_IMPEXP void SetTemporaryPrompt(const char *tempPrompt);

CONSOLE_IMPEXP char *GetTemporaryPrompt(void);

CONSOLE_IMPEXP void ClearTemporaryPrompt(void);

/*--------------------------------------------------------------------------*/
#endif /* _PROMPT_H_ */
