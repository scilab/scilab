/*
**  -*- C -*-
**
** files.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Wed Oct 24 18:43:11 2007 bruno
** Last update Wed Oct 24 18:44:50 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __FILES_UTILS_H__
#define __FILES_UTILS_H__


int CheckGraphName(char *name,char *dir);
char *StripGraph(char *name);
char *my_basename (char *name);
char* dirname (char *path);

#endif /* !__FILES_UTILS_H__ */
