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

#include	<curses.h>
#include	<stdlib.h>
#include	<term.h>
#include	<termios.h>
#include	<unistd.h>
#include	<wchar.h>
#include	<wctype.h>
#include	"reader.h"
#include	"cap_func.h"
#include	"goto_func.h"
#include	"aff_prompt.h"

static void	_set_cmd(t_list_cmd **cmd, int prompt_size)
{
  int		save_line;

  save_line = (*cmd)->nbr_line;
  (*cmd)->line++;
  (*cmd)->nbr_line = 1 + ((*cmd)->line + prompt_size + 1) / tgetnum("co");
  (*cmd)->cmd[(*cmd)->line] = L'\0';
  (*cmd)->index++;
}

int		add_letter(t_list_cmd **cmd, int key)
{
  int		i_max_decr;
  int		prompt_size;

  prompt_size = getPrompt(NOWRT_PRT);
  if (32 <= key)
    {
      if ((*cmd)->line && !((*cmd)->line % 1023))
        (*cmd)->cmd = realloc((*cmd)->cmd, (*(*cmd)->cmd) * (*cmd)->line + 1025);
      i_max_decr = (*cmd)->line;
      while (i_max_decr > (*cmd)->index)
	{
	  (*cmd)->cmd[i_max_decr] = (*cmd)->cmd[i_max_decr - 1];
	  i_max_decr--;
	}
      (*cmd)->cmd[(*cmd)->index] = (wchar_t)key;
      printf("%lc", (*cmd)->cmd[(*cmd)->index]);
      _set_cmd(cmd, prompt_size);
      i_max_decr++;
      cap_str("sc");
      printf("%ls", &(*cmd)->cmd[i_max_decr]);
      fflush(stdout);
      cap_str("rc");
      if (!(((*cmd)->index + prompt_size) % tgetnum("co")))
	cap_str("do");
    }
  return (key);
}

int		rm_letter(t_list_cmd **cmd, int key)
{
  int		i_incr;
  int		prompt_size;

  prompt_size = getPrompt(NOWRT_PRT);
  if (((*cmd)->index && key == SHRD_BACKSPACE)
      || (((*cmd)->line != (*cmd)->index) && key == SHRD_DELETE))
    {
      if (key == SHRD_BACKSPACE)
	goto_left(cmd, key);
      i_incr = (*cmd)->index;
      cap_str("sc");
      while (i_incr < (*cmd)->line)
	{
	  (*cmd)->cmd[i_incr] = (*cmd)->cmd[i_incr + 1];
	  printf("%lc", (*cmd)->cmd[i_incr]);
	  i_incr++;
	}
      fflush(stdout);
      (*cmd)->cmd[i_incr] = L'\0';
      (*cmd)->line--;
      (*cmd)->nbr_line = 1 + ((*cmd)->line + prompt_size) / tgetnum("co");
      putchar(' ');
      cap_str("rc");
    }
  key = 0;
  return (key);
}

int		delete_line_from_curs(t_list_cmd **cmd, int key)
{
  while ((*cmd)->cmd[(*cmd)->index])
    rm_letter(cmd, SHRD_DELETE);
  key = 0;
  return (key);
}
