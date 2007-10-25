/*
**  -*- C -*-
**
** loadg.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Oct 25 14:39:26 2007 bruno
** Last update Thu Oct 25 15:05:10 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef __LOADG_H__
#define __LOADG_H__

#include "files.h"

void C2F(loadg)(char *path, int *lpath, int *directed, int *n, int **tail, int **head,
		char **name, int *lname,
		char ***node_name,
		int **node_type, int **node_x, int **node_y, int **node_color, int **node_diam, int **node_border,
		int **node_font_size,
		double **node_demand,
		char ***edge_name,
		int **edge_color,int **edge_width, int **edge_hi_width, int **edge_font_size,
		double **edge_length, double **edge_cost, double **edge_min_cap, double **edge_max_cap,
		double **edge_q_weight, double **edge_q_orig, double **edge_weight,
		int *default_node_diam, int *default_node_border, int *default_edge_width,
		int *default_edge_hi_width, int *default_font_size,
		int *ndim, int *ma);

#endif /* !__LOADG_H__ */
