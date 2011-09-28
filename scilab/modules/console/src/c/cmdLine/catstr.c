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

#include		<unistd.h>
#include		<stdlib.h>
#include		<wchar.h>
#include		<wctype.h>

wchar_t			*catstr(wchar_t *wcs1, wchar_t *wcs2)
{
  wchar_t		*wcs;
  int			i;
  int			n;

  wcs = malloc(sizeof(*wcs) * (1 + wcslen(wcs1) + wcslen(wcs2)));
  if (wcs == NULL)
    exit(EXIT_FAILURE);
  n = 0;
  i = 0;
  while (wcs1[i])
    {
      wcs[n] = wcs1[i];
      i++;
      n++;
    }
  i = 0;
  while (wcs2[i])
    {
      wcs[n] = wcs2[i];
      i++;
      n++;
    }
  wcs[n] = '\0';
  return (wcs);
}
