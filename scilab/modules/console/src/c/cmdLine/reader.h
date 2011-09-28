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

#ifndef		READER_H_
# define	READER_H_

#include	<wchar.h>

typedef struct s_list_cmd t_list_cmd;

struct		s_list_cmd
{
  int			index;
  int			line;
  int			nbr_line;
  int			bin;
  wchar_t		*cmd;
  struct s_list_cmd	*next;
  struct s_list_cmd	*previous;
};

typedef struct	s_func_key
{
  int		key1;
  int		key2;
  int		(*ptr_func)(t_list_cmd **cmd, int key);
}		t_func_key;

int	putstr(wchar_t *);
char	*reader(t_list_cmd **);

t_list_cmd	*core_reader(t_list_cmd *list_cmd);

#endif	/* !READER_H_ */
