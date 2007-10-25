/*
**  -*- C -*-
**
** files.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Wed Oct 24 18:43:11 2007 bruno
** Last update Thu Oct 25 15:12:51 2007 bruno
**
** Copyright INRIA 2007
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

#include "machine.h"
#include "MALLOC.h"

#define MAXNAM 80

/** @TODO : crappy !!! Change it with a perfect IOSTREAM module */
#include "../../../intersci/src/lib/cerro.h"

typedef int (*PF)(const void *,const void *);

int CheckGraphName(char *name,char *dir);
char *StripGraph(char *name);
char *my_basename (char *name);
char* dirname (char *path);

#endif /* !__FILES_UTILS_H__ */
