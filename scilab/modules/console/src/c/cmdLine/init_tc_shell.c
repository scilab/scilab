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

#include		<termios.h>
#include		<curses.h>
#include		<term.h>
#include		<unistd.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<locale.h>
#include		"init_tc_shell.h"

int			canonic_mode(struct termios *t)
{
  t->c_lflag |= ICANON;
  t->c_lflag |= ECHO;
  return (0);
}

int			raw_mode(struct termios *t)
{
  t->c_lflag &= ~ICANON;
  t->c_lflag &= ~ECHO;
  t->c_cc[VMIN] = 1;
  t->c_cc[VTIME] = 0;
  return (0);
}

void			set_attr(int bin)
{
  struct termios	t;

  if (tcgetattr(0, &t) == -1)
    {
      puts("Cannot access to the term attributes.");
      exit(EXIT_FAILURE);
    }
  if (bin == CANON)
    canonic_mode(&t);
  else
    raw_mode(&t);
  if (tcsetattr(0, 0, &t) == -1)
    {
      puts("Cannot change the term attributes.");
      exit(EXIT_FAILURE);
    }
}

int			init_shell(int bin)
{
  if (tgetent(NULL, getenv("TERM")) == ERR
      && tgetent(NULL, "xterm") == ERR
      && tgetent(NULL, "linux") == ERR)
    {
      puts("Cannot init termcaps");
      return (2);
    }
  set_attr(bin);

  if (setlocale(LC_CTYPE, "") == NULL)
    {
      puts("Cannot set wide char.");
      return (2);
    }

  return (0);
}
