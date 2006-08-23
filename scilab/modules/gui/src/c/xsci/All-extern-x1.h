
#ifndef ALL_EXTERN_X1
#define ALL_EXTERN_X1

#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif

/*  "x_button-n.c.X1"*/

extern void TrackMouse  __PARAMS((int func, int startrow, int startcol, int firstrow, int lastrow));  
extern int SetCharacterClassRange  __PARAMS((register int low, register int high, register int value));  
extern void TrackText  __PARAMS((register int frow, register int fcol, register int trow, register int tcol));  

/*  "x_charproc-n.c.X1"*/

extern int XEvorgetchar  __PARAMS((int interrupt));  
extern void Xputstring  __PARAMS((char *str, int n));  
extern void C2F (xscisncr) __PARAMS((char *str, integer *n, integer dummy));  
extern void C2F (xscistring) __PARAMS((char *str, int *n, long int dummy));  
extern void C2F (xscimore) __PARAMS((int *n));  
extern void Scisncr  __PARAMS((char *str));  
extern void sciprint __PARAMS((char *fmt, ...));
extern void sciprint_nd __PARAMS((char *fmt, ...));
extern int sciprint2 __PARAMS((int i,char *fmt, ...));
extern void Scistring __PARAMS((char *str));
extern void Xputchar  __PARAMS((unsigned char c));  
extern void v_write  __PARAMS((int f, char *d, int len));  
extern void xevents1  __PARAMS((void));  
extern int in_put  __PARAMS((int interrupt));  

extern void unparseputn  __PARAMS((unsigned int n, int fd));  
extern void unparseputc  __PARAMS((char c, int fd));  
extern void unparsefputs  __PARAMS((register char *s, int fd));  
extern void VTRun  __PARAMS((int nostartup));  
extern void VTReset  __PARAMS((Boolean full));  
extern int set_character_class  __PARAMS((register char *s));  
extern void FindFontSelection  __PARAMS((char *atom_name, int justprobe));  
extern void SetVTFont  __PARAMS((int i, int doresize, char *name1, char *name2));  


/*  "x_cursor-n.c.X1"*/


/*  "x_data-n.c.X1"*/
/*  "x_input-n.c.X1"*/


/*  "x_main-n.c.X1"*/

extern int C2F(winsci) __PARAMS((char *pname, int *nos, int *idisp, char *display,int *lpname, int *ldisp));
extern void getcolordef  __PARAMS((integer *c));  
extern void setcolordef  __PARAMS((int screenc));  
/* extern void initColors  __PARAMS((void));   */
extern void main_sci  __PARAMS((int argc, char **argv));  
extern int clear_ieee_warnings  __PARAMS((void));  
extern void sci_clear_and_exit  __PARAMS((integer n));  
extern int C2F(clearexit)  __PARAMS((int *n));  
extern int GetBytesAvailable  __PARAMS((int fd));  
extern int kill_process_group  __PARAMS((int pid, int sig)); 
     
     /*  "x_menu-n.c.X1"*/
     
extern void do_kill1  __PARAMS((int ));
     
     /*  "x_misc-n.c.X1"*/
     
extern void x_events  __PARAMS((void));  
extern void Bell  __PARAMS((void));  
extern void VisualBell  __PARAMS((void));  
extern void Redraw  __PARAMS((void));  
extern void do_osc  __PARAMS(( int (*func) ()));
extern void Changename  __PARAMS((register char *name));  
extern void Changetitle  __PARAMS((register char *name));  
extern void Panic  __PARAMS((char *s, int a));  
extern char *SysErrorMsg  __PARAMS((int n));  
extern void SysError  __PARAMS((int i));  
extern void Error  __PARAMS((int i));  
extern void Cleanup  __PARAMS((int code));  
extern char *strindex  __PARAMS((register char *s1, register char *s2));  
extern void xerror  __PARAMS((Display *d, register XErrorEvent *ev));  
extern void xioerror  __PARAMS((Display *dpy));  
extern void set_vt_visibility  __PARAMS((Boolean on));  
     
     /*  "x_screen-n.c.X1"*/
     
extern void Size2Scilab  __PARAMS((int rows, int cols));  
     
     /*  "x_scrollbar-n.c.X1"*/
     
     
     /* "x_tabs-n.c.X1" */
     
extern void TabReset  __PARAMS((unsigned int *tabs));  
extern void TabSet  __PARAMS((unsigned int *tabs, int col));  
extern void TabClear  __PARAMS((unsigned int *tabs, int col));  
extern int TabNext  __PARAMS((unsigned int *tabs, int col));  
extern void TabZonk  __PARAMS((unsigned int *tabs));  
     
     /* "x_test_loop-n.c.X1" */
     
     /* 1 "x_util-n.c.X1"*/
     
     /* 1 "x_zzledt-n.c.X1" */
extern void C2F (zzledt1) __PARAMS((char *buffer, int *buf_size, int *len_line, int *eof, long int dummy1));  


#endif /**  ALL_EXTERN_X1 **/


