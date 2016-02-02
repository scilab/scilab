/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Ledru
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#ifndef __NOGRAPHICS_H__
#define __NOGRAPHICS_H__

#include "machine.h"

int gw_graphics(void);

void loadGraphicModule(void);

BOOL TerminateGraphics(void);

int scig_2dzoom(int win_num);

int GetDriverId(void);

void scig_unzoom(int win_num);

int get_window_scale( int i, double * subwin );

double Maxi(double *vect, int n);

double Mini(double *vect, int n);

int scig_3drot(int win_num);

void scig_replay(int win_num);

void scig_erase(int win_num);

void scig_loadsg(int win_num, char *filename);

void del_window_scale(int i);

void scig_savesg(int win_num, char * filename);

void Cscale2default(void);

int scig_toPs(int win_num, int colored, char * bufname, char * driver);

void delete_sgwin_entities(int win_num);

int C2F(entier2d)( int * n, double * dx, int * s );

int C2F(dr)(char x0[], char x1[], int *x2, int *x3, int *x4, int *x5, int *x6, int *x7, double *dx1, double *dx2, double *dx3, double *dx4, int lx0, int lx1);

void scig_sel(int win_num);

void DeleteObjs(int win_num);

int C2F(dr1)( char x0[], char x1[], int *x2, int *x3, int *x4, int *x5, int *x6, int *x7, double *dx1, double *dx2, double *dx3, double *dx4, int lx0, int lx1);

void sciRedrawF(int *num);

int C2F(SetDriver)(void);

void GetDriver1(char *str, int *v2, int * v3, int * v4, int *v5, int * v6, int * v7, double * dv1, double * dv2, double * dv3, double * dv4);

void sciGetIdFigure (int *vect, int *id, int *flag);

#endif /* __NOGRAPHICS_H__ */
