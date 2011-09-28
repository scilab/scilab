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

#ifndef		HISTORY_H_
# define	HISTORY_H_

#include	"reader.h"

int		previous_cmd(t_list_cmd **cmd, int key);
int		next_cmd(t_list_cmd **cmd, int key);

#endif	/* !HISTORY_H_ */
