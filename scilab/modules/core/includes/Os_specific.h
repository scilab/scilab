/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

extern void C2F(userlk) __PARAMS((integer *k));  

/*  "basin-n.c.X1" */

extern int C2F(basin) __PARAMS((integer *ierr, integer *lunit, char *string, char *fmt, ftnlen string_len, ftnlen fmt_len));  

/*  "basout-n.c.X1" */
extern int C2F(basou1) __PARAMS((integer *lunit, char *string, ftnlen string_len));  
/*  "cgpath-n.c.X1" */
extern void C2F(cgpath) __PARAMS((char *nomfic, integer *ln));  
/*  "clunit-n.c.X1" */
extern int C2F(clunit) __PARAMS((integer *lunit, char *name, integer *mode, ftnlen name_len));  
/*  "dbasin-n.c.X1" */
extern int C2F(dbasin) __PARAMS((integer *ierr, integer *lunit, char *fmt, double *v, integer *iv, integer *n, ftnlen fmt_len));  
extern int C2F(s2val) __PARAMS((char *str, double *v, integer *iv, integer *n, integer *maxv, integer *ierr, ftnlen str_len));  
extern int C2F(nextv) __PARAMS((char *str, double *v, integer *nv, integer *ir, integer *ierr, ftnlen str_len));  
extern int C2F(s2int) __PARAMS((char *str, integer *nlz, integer *v, integer *ir, integer *ierr, ftnlen str_len));  
/*  "fgetarg-n.c.X1" */
extern int C2F(fgetarg) __PARAMS((integer *n, char *str, ftnlen str_len));  
/*  "flags-n.c.X1" */
extern void set_echo_mode  __PARAMS((int mode));  
extern int get_echo_mode  __PARAMS((void));  
extern void set_is_reading  __PARAMS((int mode));  
extern int get_is_reading  __PARAMS((void));  
/*  "getenvc-n.c.X1" */

/*  "getpidc-n.c.X1" */
extern int C2F(getpidc) __PARAMS((int *id1));  

/*  "inffic-n.c.X1" */
extern void C2F(inffic) __PARAMS((integer *iopt, char *name, integer *nc));  
extern void C2F(infficl) __PARAMS((integer *iopt, integer *nc));  
/*  "inibrk-n.c.X1" */
extern int C2F(inibrk) __PARAMS((void));  
extern int C2F(sunieee) __PARAMS((void));  
extern integer C2F(my_handler_) __PARAMS((integer *sig, integer *code, integer *sigcontext, integer *addr));  
extern integer C2F(my_ignore_) __PARAMS((integer *sig, integer *code, integer *sigcontext, integer *addr));  

/*  "plevel-n.c.X1" */
extern int C2F(plevel) __PARAMS((integer *n));  
/*  "sigbas-n.c.X1" */
extern int C2F(systemc) __PARAMS((char *command, integer *stat));  
/*  "timer-n.c.X1" */
extern int C2F(stimer) __PARAMS((void));  
/*  "tmpdir-n.c.X1" */
extern void C2F(settmpdir) __PARAMS((void));  
extern void C2F(tmpdirc) __PARAMS((void));  

/* link.c */
extern int LinkStatus  __PARAMS((void));


#endif /* SUNSCI_PROTO */
