/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
