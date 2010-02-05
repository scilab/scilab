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


/*  "basin-n.c.X1" */

extern int C2F(basin)(int *ierr, int *lunit, char *string, char *fmt, ftnlen string_len, ftnlen fmt_len);

/*  "basout-n.c.X1" */
extern int C2F(basou1)(int *lunit, char *string, ftnlen string_len);
/*  "cgpath-n.c.X1" */
extern void C2F(cgpath)(char *nomfic, int *ln);
/*  "clunit-n.c.X1" */
extern int C2F(clunit)(int *lunit, char *name, int *mode, ftnlen name_len);
/*  "dbasin-n.c.X1" */
extern int C2F(dbasin)(int *ierr, int *lunit, char *fmt, double *v, int *iv, int *n, ftnlen fmt_len);
extern int C2F(s2val)(char *str, double *v, int *iv, int *n, int *maxv, int *ierr, ftnlen str_len);
extern int C2F(nextv)(char *str, double *v, int *nv, int *ir, int *ierr, ftnlen str_len);
extern int C2F(s2int)(char *str, int *nlz, int *v, int *ir, int *ierr, ftnlen str_len);
/*  "fgetarg-n.c.X1" */
extern int C2F(fgetarg)(int *n, char *str, ftnlen str_len);

/*  "getpidc-n.c.X1" */
extern int C2F(getpidc)(int *id1);

/*  "inffic-n.c.X1" */
extern void C2F(inffic)(int *iopt, char *name, int *nc);
extern void C2F(infficl)(int *iopt, int *nc);
/*  "inibrk-n.c.X1" */
extern int C2F(inibrk)(void);
extern int C2F(sunieee)(void);
extern int C2F(my_handler_)(int *sig, int *code, int *sigcontext, int *addr);
extern int C2F(my_ignore_)(int *sig, int *code, int *sigcontext, int *addr);

/*  "plevel-n.c.X1" */
extern int C2F(plevel)(int *n);
/*  "sigbas-n.c.X1" */
extern int C2F(systemc)(char *command, int *stat);
/*  "timer-n.c.X1" */
extern int C2F(stimer)(void);
/*  "tmpdir-n.c.X1" */
extern void C2F(settmpdir)(void);
extern void C2F(tmpdirc)(void);

/* link.c */
extern int LinkStatus (void);


#endif /* SUNSCI_PROTO */
