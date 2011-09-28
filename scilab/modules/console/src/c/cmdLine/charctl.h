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

#ifndef		CHAR_CTL_H_
# define	CHAR_CTL_H_

#include	"reader.h"

int		add_letter(t_list_cmd **cmd, int key);
int		rm_letter(t_list_cmd **cmd, int key);
int		delete_line_from_curs(t_list_cmd **cmd, int key);

#endif	/* !CHAR_CTL_H_*/
