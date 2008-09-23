
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/*
**  -*- C -*-
**
** files.h
**
** Started on  Wed Oct 24 18:43:11 2007 bruno
** Last update Fri Oct 26 14:55:34 2007 bruno
*/

#ifndef __FILES_UTILS_H__
#define __FILES_UTILS_H__

#include <stdio.h>

#if !(defined _MSC_VER)
#if defined(netbsd) || defined(freebsd)
#include <sys/types.h>
#endif
#include <dirent.h>
#endif

#include <string.h>
#include <stdlib.h>

#include <math.h>

#ifdef _MSC_VER
#include <direct.h> /*_getcwd _chdir*/
#endif
#if (defined _MSC_VER)

#include <io.h>
#define  getcwd(x,y) _getcwd(x,y)
#define chdir(x) _chdir(x)
#endif

#if defined(netbsd) || defined(freebsd)|| defined(linux)
#include <unistd.h>
#endif

#include "MALLOC.h"

#define MAXNAM 80

#include "cerro.h"

typedef int (*PF)(const void *,const void *);

int CheckGraphName(char *name,char *dir);
char *StripGraph(char *name);
char *my_basename (char *name);
char* my_dirname (char *path);

#endif /* !__FILES_UTILS_H__ */
