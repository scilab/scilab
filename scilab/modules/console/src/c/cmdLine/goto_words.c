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

#include	"reader.h"
#include	"goto_func.h"

int		nextWord(t_list_cmd **_cmd, int _key)
{
  while ((*_cmd)->cmd[(*_cmd)->index] && (*_cmd)->cmd[(*_cmd)->index] != L' ')
    {
      goto_right(_cmd, _key);
    }
  while ((*_cmd)->cmd[(*_cmd)->index] && (*_cmd)->cmd[(*_cmd)->index] == L' ') /* then passing through spaces */
    {
      goto_right(_cmd, _key);
    }
  _key = 0;
  return (_key);
}

int		previousWord(t_list_cmd **_cmd, int _key)
{
  while ((*_cmd)->index && (*_cmd)->cmd[(*_cmd)->index - 1] == L' ')
    {
      goto_left(_cmd, _key);
    }
  while ((*_cmd)->index && (*_cmd)->cmd[(*_cmd)->index - 1] != L' ')
    {
      goto_left(_cmd, _key);
    }
  _key = 0;
  return (_key);
}
