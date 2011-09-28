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

#ifndef		GOTO_FUNC_H_
# define	GOTO_FUNC_H_

#include	"reader.h"

int		goto_right(t_list_cmd **cmd, int key);
int		goto_left(t_list_cmd **cmd, int key);
int		end_line(t_list_cmd **cmd, int key);
int		beg_line(t_list_cmd **cmd, int key);

#endif /* !GOTO_FUNC_H_ */
