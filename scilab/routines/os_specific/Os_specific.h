/* Copyright INRIA */
#ifndef SUNSCI_PROTO
#define  SUNSCI_PROTO

#include "../machine.h" 

typedef long int ftnlen ;

/*  "addinter-n.c.X1" */

void C2F(addinter)(int *iflag,int *descla, int *ptrdescla, int *nvla, char *iname,
		   int *desc, int *ptrdesc, int *nv, char *c_cpp, 
		   int *lib_cpp, int *err);

extern void RemoveInterf  __PARAMS((int Nshared));  
extern void C2F(userlk) __PARAMS((integer *k));  

/*  "bashos-n.c.X1" */

extern int C2F(bashos) __PARAMS((char *ligne, integer *n, integer *nout, integer *ierr, ftnlen ligne_len));  

/*  "basin-n.c.X1" */

extern int C2F(basin) __PARAMS((integer *ierr, integer *lunit, char *string, char *fmt, ftnlen string_len, ftnlen fmt_len));  

/*  "basout-n.c.X1" */
extern int C2F(basout) __PARAMS((integer *io, integer *lunit, char *string, ftnlen string_len));  
extern int C2F(basou1) __PARAMS((integer *lunit, char *string, ftnlen string_len));  
/*  "cgpath-n.c.X1" */
extern void C2F(cgpath) __PARAMS((char *nomfic, integer *ln));  
/*  "cluni0-n.c.X1" */
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, ftnlen name_len, ftnlen nams_len));  
/*  "clunit-n.c.X1" */
extern int C2F(clunit) __PARAMS((integer *lunit, char *name, integer *mode, ftnlen name_len));  
/*  "csignal-n.c.X1" */
extern void controlC_handler  __PARAMS((int sig));  
extern int C2F(csignal) __PARAMS((void));  
/*  "ctrlc-n.c.X1" */
extern int C2F(ctrlc) __PARAMS((void));  
/*  "dbasin-n.c.X1" */
extern int C2F(dbasin) __PARAMS((integer *ierr, integer *lunit, char *fmt, double *v, integer *iv, integer *n, ftnlen fmt_len));  
extern int C2F(s2val) __PARAMS((char *str, double *v, integer *iv, integer *n, integer *maxv, integer *ierr, ftnlen str_len));  
extern int C2F(nextv) __PARAMS((char *str, double *v, integer *nv, integer *ir, integer *ierr, ftnlen str_len));  
extern int C2F(s2int) __PARAMS((char *str, integer *nlz, integer *v, integer *ir, integer *ierr, ftnlen str_len));  
/*  "fgetarg-hpux-n.c.X1" */
extern int C2F(fgetarg) __PARAMS((integer *n, char *str, ftnlen str_len));  
/*  "fgetarg-n.c.X1" */
extern int C2F(fgetarg) __PARAMS((integer *n, char *str, ftnlen str_len));  
/*  "flags-n.c.X1" */
extern void set_echo_mode  __PARAMS((int mode));  
extern int get_echo_mode  __PARAMS((void));  
extern void set_is_reading  __PARAMS((int mode));  
extern int get_is_reading  __PARAMS((void));  
/*  "getenvc-n.c.X1" */

void C2F(getenvc) __PARAMS((int *ierr,char *var,char *buf,int *buflen,int *iflag));

/*  "getpidc-n.c.X1" */
extern int C2F(getpidc) __PARAMS((int *id1));  
/*  "getpro-n.c.X1" */
extern void C2F(getpro) __PARAMS((char *ret_val, ftnlen ret_val_len));  

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
extern int C2F(sigbas) __PARAMS((integer *n));  
/*  "systemc-n.c.X1" */
extern int C2F(systemc) __PARAMS((char *command, integer *stat));  
/*  "timer-n.c.X1" */
extern int C2F(timer) __PARAMS((double *etime));  
extern int C2F(stimer) __PARAMS((void));  
/*  "tmpdir-n.c.X1" */
extern void C2F(settmpdir) __PARAMS((void));  
extern void C2F(tmpdirc) __PARAMS((void));  
/*  "zzledt-n.c.X1" */
extern void C2F(zzledt) __PARAMS((char *buffer, int *buf_size, int *len_line, int *eof, int *interrupt, int *modex,long int dummy1));  

/* link.c */
extern int LinkStatus  __PARAMS((void));
extern void C2F(isciulink)(integer *i) ;


/* isanan.c */ 

integer C2F(isanan)  __PARAMS((double *x) ); 


#endif /* SUNSCI_PROTO */
