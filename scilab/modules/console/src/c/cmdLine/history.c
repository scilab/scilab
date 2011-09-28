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

#include	<stdio.h>
#include	<unistd.h>
#include	<term.h>
#include	<termios.h>
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"
#include	"reader.h"

int		previous_cmd(t_list_cmd **cmd, int key)
{
  int		prompt_size;

  key = 0;
  if ((*cmd)->previous)
    {
      while ((*cmd)->index)
	goto_left(cmd, key);
      cap_str("up");
      cap_str("do");
      cap_str("cd");
      prompt_size = getPrompt(WRT_PRT);
      (*cmd) = (*cmd)->previous;
      printf("%ls", (*cmd)->cmd);
      fflush(stdout);
      (*cmd)->index = (*cmd)->line;
      if (!(((*cmd)->index + prompt_size) % tgetnum("co")))
      	cap_str("do");
    }
  return (key);
}

int		next_cmd(t_list_cmd **cmd, int key)
{
  int		prompt_size;

  key = 0;
  if ((*cmd)->next)
    {
      while ((*cmd)->index)
	goto_left(cmd, key);
      cap_str("up");
      cap_str("do");
      cap_str("cd");
      (*cmd) = (*cmd)->next;
      prompt_size = getPrompt(WRT_PRT);
      printf("%ls", (*cmd)->cmd);
      fflush(stdout);
      (*cmd)->index = (*cmd)->line;
      if (!(((*cmd)->index + prompt_size) % tgetnum("co")))
	cap_str("do");
    }
  return (key);
}
