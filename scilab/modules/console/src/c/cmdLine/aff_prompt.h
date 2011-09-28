/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef		AFF_PROMPT_H_
# define	AFF_PROMPT_H_

# define WRT_PRT	1
# define NOWRT_PRT	-1

#include    <wchar.h>

wchar_t	*setPrompt(wchar_t *);
int	getPrompt(int);

#endif		/* !AFF_PROMPT_H_ */
