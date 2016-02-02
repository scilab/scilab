/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#ifndef __COMMOM_STRUCTURE_H__
#define __COMMOM_STRUCTURE_H__

#include "machine.h"
#include "dynlib_differential_equations.h"

typedef struct
{
    int err1;
    int err2;
    int errct;
    int toperr;
    int errpt;
    int ieee;
    int errcatch;
} STR_ERRGST;

typedef struct
{
    int iero;
} STR_IERODE;

typedef struct
{
    int mesflg, lunit;
} STR_EH0001;

typedef struct
{
    double tsw;
    double rowns2[20];
    double pdnorm;
    int insufr;
    int insufi;
    int ixpr;
    int iowns2[2];
    int jtyp;
    int mused;
    int mxordn;
    int mxords;
} STR_LSA001;

typedef struct
{
    double rownr3[2];
    double t0;
    double tlast;
    double toutc;
    int lg0;
    int lg1;
    int lgx;
    int iownr3[2];
    int irfnd;
    int itaskc;
    int ngc;
    int nge;
} STR_LSR001;

typedef struct
{
    double tret;
    double rowns[209];
    double ccmax;
    double el0;
    double h__;
    double hmin;
    double hmxi;
    double hu;
    double rc;
    double tn;
    double uround;
    int illin;
    int init;
    int lyh;
    int lewt;
    int lacor;
    int lsavf;
    int lwm;
    int liwm;
    int mxstep;
    int mxhnil;

    int nhnil;
    int ntrep;
    int nslast;
    int nyh;
    int iowns[6];
    int icf;
    int ierpj;
    int iersl;
    int jcur;

    int jstart;
    int kflag;
    int l;
    int meth;
    int miter;
    int maxord;
    int maxcor;
    int msbp;
    int mxncf;
    int n;
    int nq;
    int nst;
    int nfe;
    int nje;
    int nqu;
} STR_LS0001;

#ifdef _MSC_VER
DIFFERENTIAL_EQUATIONS_IMPEXP STR_IERODE C2F(ierode);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_EH0001 C2F(eh0001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LSA001 C2F(lsa001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LSR001 C2F(lsr001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LS0001 C2F(ls0001);
#else
extern STR_IERODE C2F(ierode);
extern STR_EH0001 C2F(eh0001);
extern STR_LSA001 C2F(lsa001);
extern STR_LSR001 C2F(lsr001);
extern STR_LS0001 C2F(ls0001);
#endif
#endif /* !__COMMOM_STRUCTURE_H__ */
