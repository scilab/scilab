/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#ifndef AFF_PROMPT_H_
#define AFF_PROMPT_H_

#include    <wchar.h>

/* Token to indicate printPrompt what to do */

/* Display the prompt */
#define WRITE_PROMPT	1
/* Don't display the prompt */
#define NOWRITE_PROMPT -1

/**
 * Save a prompt.
 * @param prompt that will be saved
 * or NULL if you don't want to saved any prompt
 * @return last saved prompt
 */
wchar_t *setPrompt(wchar_t *);

/**
 * Get the current saved prompt.
 * @param token to indicate if prompt must be displayed or not
 * @return size of the prompt.
 */
int printPrompt(int);

#endif /* !AFF_PROMPT_H_ */
