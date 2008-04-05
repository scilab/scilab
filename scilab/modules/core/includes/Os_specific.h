/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) INRIA - 2008 - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef SUNSCI_PROTO
#define  SUNSCI_PROTO

#include "machine.h"

typedef long int ftnlen ;

/*  "addinter-n.c.X1" */

void C2F(addinter)(int *iflag,int *descla, int *ptrdescla, int *nvla, char *iname,
		   int *desc, int *ptrdesc, int *nv, char *c_cpp,
		   int *lib_cpp, int *err);

extern void C2F(userlk)(integer *k);

/*  "basin-n.c.X1" */

extern int C2F(basin)(integer *ierr, integer *lunit, char *string, char *fmt, ftnlen string_len, ftnlen fmt_len);

/*  "basout-n.c.X1" */
extern int C2F(basou1)(integer *lunit, char *string, ftnlen string_len);
/*  "cgpath-n.c.X1" */
extern void C2F(cgpath)(char *nomfic, integer *ln);
/*  "clunit-n.c.X1" */
extern int C2F(clunit)(integer *lunit, char *name, integer *mode, ftnlen name_len);
/*  "dbasin-n.c.X1" */
extern int C2F(dbasin)(integer *ierr, integer *lunit, char *fmt, double *v, integer *iv, integer *n, ftnlen fmt_len);
extern int C2F(s2val)(char *str, double *v, integer *iv, integer *n, integer *maxv, integer *ierr, ftnlen str_len);
extern int C2F(nextv)(char *str, double *v, integer *nv, integer *ir, integer *ierr, ftnlen str_len);
extern int C2F(s2int)(char *str, integer *nlz, integer *v, integer *ir, integer *ierr, ftnlen str_len);
/*  "fgetarg-n.c.X1" */
extern int C2F(fgetarg)(integer *n, char *str, ftnlen str_len);

/*  "getpidc-n.c.X1" */
extern int C2F(getpidc)(int *id1);

/*  "inffic-n.c.X1" */
extern void C2F(inffic)(integer *iopt, char *name, integer *nc);
extern void C2F(infficl)(integer *iopt, integer *nc);
/*  "inibrk-n.c.X1" */
extern int C2F(inibrk)(void);
extern int C2F(sunieee)(void);
extern integer C2F(my_handler_)(integer *sig, integer *code, integer *sigcontext, integer *addr);
extern integer C2F(my_ignore_)(integer *sig, integer *code, integer *sigcontext, integer *addr);

/*  "plevel-n.c.X1" */
extern int C2F(plevel)(integer *n);
/*  "sigbas-n.c.X1" */
extern int C2F(systemc)(char *command, integer *stat);
/*  "timer-n.c.X1" */
extern int C2F(stimer)(void);
/*  "tmpdir-n.c.X1" */
extern void C2F(settmpdir)(void);
extern void C2F(tmpdirc)(void);

/* link.c */
extern int LinkStatus (void);


#endif /* SUNSCI_PROTO */
