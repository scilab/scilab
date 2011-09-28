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

#include		<wchar.h>
#include		<wctype.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<unistd.h>
#include		<curses.h>
#include		<termios.h>
#include		<term.h>
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"goto_words.h"

int			caseHomeOrEndKey(t_list_cmd **list_cmd)
{
  switch (getwchar())
    {
    case L'H':
     return (beg_line(list_cmd, SHRD_HOME));
    case L'F':
     return (end_line(list_cmd, SHRD_END));
    default:
     printf("\a");
     fflush(stdout);
     return (0);
    }
}

/*
 * If second key was L'['
 * It mean this an arrow key or delete key.
 */

int			caseDelOrArrowKey(t_list_cmd **list_cmd)
{
  switch (getwchar())
    {
    case L'A':
     return (previous_cmd(list_cmd, SHRD_UP));
    case L'B':
     return (next_cmd(list_cmd, SHRD_DOWN));
    case L'C':
     return (goto_right(list_cmd, SHRD_RIGHT));
    case L'D':
     return (goto_left(list_cmd, SHRD_LEFT));
    case L'3':
     if (getwchar() == L'~')
       return (rm_letter(list_cmd, SHRD_DELETE));
    default:
     printf("\a");
     fflush(stdout);
     return (0);
    }
}

/*
 * If last key was Meta...
 */
int			caseMetaKey(t_list_cmd **_list_cmd)
{
  switch (getwchar())
    {
    case L'f': case L'F':
     return (nextWord(_list_cmd, META_F));
    case L'b': case L'B':
     return (previousWord(_list_cmd, META_B));
    case L'[':
     return (caseDelOrArrowKey(_list_cmd));
    case L'O':
     return (caseHomeOrEndKey(_list_cmd));
    default:
     printf("\a");
     fflush(stdout);
     return (0);
    }
}

/*
 * Read keyboard a first time.
 */

int			getKey(t_list_cmd **list_cmd)
{
  int			key;

  key = getwchar();
  switch (key)
    {
    case CTRL_A:
     return (beg_line(list_cmd, key));
    case CTRL_B:
     return (goto_left(list_cmd, key));
    case CTRL_E:
     return (end_line(list_cmd, key));
    case CTRL_F:
     return (goto_right(list_cmd, key));
    case CTRL_K:
     return (delete_line_from_curs(list_cmd, key));
    case CTRL_N:
     return (next_cmd(list_cmd, key));
    case CTRL_P:
     return (previous_cmd(list_cmd, key));
    case ESCAPE:
     return (caseMetaKey(list_cmd));
    case SHRD_BACKSPACE:
       return (rm_letter(list_cmd, SHRD_BACKSPACE));
    default:
     if (key == L'\n')
       return ('\n');
     return (add_letter(list_cmd, key));
    }
}

/*
 * If there is a string the function save it.
 * else The function write the saved str.
 */

void			mem_cmd(t_list_cmd **cmd)
{
  static t_list_cmd	**mem_list;
  static int		i;

  if (cmd != NULL)
    {
      mem_list = cmd;
      i = (*cmd)->index;
    }
  else
    {
      i = (*mem_list)->index;
      (*mem_list)->index = (*mem_list)->line;
      printf("%ls", (*mem_list)->cmd);
      fflush(stdout);
      while ((*mem_list)->index != i)
        goto_left(mem_list, 0);
    }
}

t_list_cmd		*get_new_cmd(t_list_cmd *last_cmd)
{
  t_list_cmd		*new_cmd;

  new_cmd = malloc(sizeof(*new_cmd));
  if (new_cmd == NULL)
    exit(EXIT_FAILURE);
  if (last_cmd != NULL)
    {
      last_cmd->next = new_cmd;
      last_cmd->bin = 1;
    }
  new_cmd->previous = last_cmd;
  new_cmd->next = NULL;
  new_cmd->line = 0;
  new_cmd->bin = 0;
  new_cmd->nbr_line = 0;
  return (new_cmd);
}

void			get_cmd(t_list_cmd **list_cmd, int *key)
{
  int			bin;

  (*list_cmd)->index = 0;
  *key = 0;
  bin = 1;
  while (bin)
    {
      mem_cmd(list_cmd);
      if (getKey(list_cmd) == '\n')
        {
          putchar('\n');
          bin = 0;
        }
    }
}

t_list_cmd		*free_cmd(t_list_cmd **cmd)
{
  t_list_cmd		*save;

  save = *cmd;
  *cmd = (*cmd)->previous;
  free(save->cmd);
  free(save);
  if (*cmd)
    (*cmd)->next = NULL;
  return (*cmd);
}

void			suppress_history(t_list_cmd *cmd, int limit)
{
  while (limit && cmd->previous)
    {
      cmd = cmd->previous;
      limit--;
    }
  if (!limit)
    {
      cmd = cmd->next;
      free(cmd->previous->cmd);
      free(cmd->previous);
      cmd->previous = NULL;
    }
}
