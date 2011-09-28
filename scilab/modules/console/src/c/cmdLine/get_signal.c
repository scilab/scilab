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

#include	<unistd.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<curses.h>
#include	<term.h>
#include	"cmd_func.h"
#include	"cap_func.h"
#include	"aff_prompt.h"

void		get_new_term(int signum)
{
  char		*term_env;

  cap_str("cl");
  if ((term_env = getenv("TERM")) == NULL)
    exit(EXIT_FAILURE);
  if (tgetent(NULL, term_env) == ERR)
    exit(EXIT_FAILURE);
  getPrompt(WRT_PRT);
  mem_cmd(NULL);
  signal(signum, get_new_term);
}
