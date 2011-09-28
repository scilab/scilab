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
#include		<signal.h>
#include		"cmd_func.h"
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"get_signal.h"
#include		"charEncoding.h"

wchar_t			*cmddup(t_list_cmd *cmd, wchar_t *wcs)
{
  wchar_t		*dup_cmd;

  dup_cmd = malloc(sizeof(*dup_cmd) * (((cmd->line / 1024) + 1) * 1024));
  if (dup_cmd == NULL)
    exit(EXIT_FAILURE);
  dup_cmd[cmd->line] = L'\0';
  cmd->nbr_line = 1 + (cmd->line + getPrompt(NOWRT_PRT)) / tgetnum("co");
  if (wcs != NULL)
    wcscpy(dup_cmd, wcs);
  else
    wcscpy(dup_cmd, cmd->cmd);
  return (dup_cmd);
}

t_list_cmd		*get_new_hist(t_list_cmd *cmd)
{
  t_list_cmd		*last_cmd;

  last_cmd = cmd;
  while (last_cmd->bin)
    last_cmd = last_cmd->next;
  last_cmd->line = cmd->line;
  last_cmd->nbr_line = cmd->nbr_line;
  last_cmd->cmd = cmddup(cmd, NULL);
  return (last_cmd);
}

void			*epur_cmd(t_list_cmd *cmd)
{
  int			i;
  wchar_t		*dup_cmd;

  i = 0;
  while (cmd->cmd[i] && (cmd->cmd[i] == L' ' || cmd->cmd[i] == L'\t'))
    i++;
  cmd->line = wcslen(&cmd->cmd[i]);
  dup_cmd = cmddup(cmd, &cmd->cmd[i]);
  free(cmd->cmd);
  cmd->cmd = dup_cmd;
  return (dup_cmd);
}

t_list_cmd		*core_reader(t_list_cmd *list_cmd)
{
  int			key;
  int			ret;

  list_cmd = get_new_cmd(list_cmd);
  getPrompt(WRT_PRT);
  key = 0;
  list_cmd->line = 0;
  list_cmd->cmd = malloc(sizeof(*list_cmd->cmd) * 1024);
  if (list_cmd->cmd == NULL)
    exit(EXIT_FAILURE);
  list_cmd->cmd[0] = L'\0';
  ret = 0;
  get_cmd(&list_cmd, &key);
  if (key == CTRL_D)
    return (NULL);
  epur_cmd(list_cmd);
  if (list_cmd->bin)
    list_cmd = get_new_hist(list_cmd);
  suppress_history(list_cmd, 200);
  return (list_cmd);
}

char			*reader(t_list_cmd **history)
{
  char *dest;

  signal(SIGWINCH, get_new_term);
  *history = core_reader(*history);
  if (*history == NULL)
    return (NULL);
  while (!wcscmp((*history)->cmd, L""))
    {
      free_cmd(history);
      *history = core_reader(*history);
      if (*history == NULL)
	return (NULL);
    }
  dest = wide_string_to_UTF8((*history)->cmd);
  return (dest);
}
