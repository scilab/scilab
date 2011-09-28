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

#include		<term.h>
#include		<curses.h>
#include		<termios.h>
#include		<stdio.h>
#include		<stdlib.h>
#include		<unistd.h>
#include		<stdio.h>

void	cap_str(char *str)
{
  char	*cap_str;

  cap_str = tgetstr(str, NULL);
  if (cap_str != NULL)
    tputs(cap_str, 1, putchar);
}

void	cap_goto(int col, int li)
{
  tputs(tgoto(tgetstr("cm", NULL), col, li), 1, putchar);
}
