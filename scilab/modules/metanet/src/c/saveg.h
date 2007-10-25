/*
**  -*- C -*-
**
** saveg.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Oct 25 14:06:13 2007 bruno
** Last update Thu Oct 25 14:16:02 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __SAVEG_H__
#define __SAVEG_H__

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
/** only used for x=dir[1024] **/
#include <io.h>
#define  getwd(x) _getcwd(x,1024)
#define chdir(x) _chdir(x)
#endif

#if defined(netbsd) || defined(freebsd)|| defined(linux)
#include <unistd.h>
#endif

#include "machine.h"
#include "MALLOC.h"

#define MAXNAM 80

#include "files.h"

/** @TODO : crappy !!! Change it with a perfect IOSTREAM module */
#include "../../../intersci/src/lib/cerro.h"

typedef int (*PF)(const void *,const void *);

void C2F(saveg) (char *path, int *lpath,
		 char *name, int *lname,int *directed,int *node_number,int *tail,int *head,
		 char ***node_name,
		 int *node_type,int *node_x,int *node_y,int *node_color,int *node_diam,int *node_border,
		 int *node_font_size,
		 double *node_demand,
		 char ***edge_name,
		 int *edge_color,int *edge_width,int *edge_hi_width,int *edge_font_size,
		 double *edge_length,double *edge_cost,double *edge_min_cap,double *edge_max_cap,
		 double *edge_q_weight,double *edge_q_orig,double *edge_weight,
		 int *default_node_diam,int *default_node_border,int *default_edge_width,
		 int *default_edge_hi_width,int *default_font_size,
		 int *ma);

#endif /* !__SAVEG_H__ */
