
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
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
** loadg.h
**
** Started on  Thu Oct 25 14:39:26 2007 bruno
** Last update Thu Oct 25 15:05:10 2007 bruno
*/

#ifndef __LOADG_H__
#define __LOADG_H__

#include "files.h"

void C2F(loadg)(char *path, int *lpath, char **name, int *lname, int *directed, int *n, int **tail, int **head,
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
