
#ifndef ALL_EXTERN_X1
#define ALL_EXTERN_X1

#ifdef __STDC__
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		()
#endif
#endif

/*  "x_button-n.c.X1"*/

extern void TrackMouse  _PARAMS((int func, int startrow, int startcol, int firstrow, int lastrow));  
extern int SetCharacterClassRange  _PARAMS((register int low, register int high, register int value));  
extern void TrackText  _PARAMS((register int frow, register int fcol, register int trow, register int tcol));  

/*  "x_charproc-n.c.X1"*/

extern int XEvorgetchar  _PARAMS((void));  
extern void Xputstring  _PARAMS((char *str, int n));  
extern void C2F (xscisncr) _PARAMS((char *str, integer *n, integer dummy));  
extern void C2F (xscistring) _PARAMS((char *str, int *n, long int dummy));  
extern void C2F (xscimore) _PARAMS((int *n));  
extern void Scisncr  _PARAMS((char *str));  
extern void sciprint _PARAMS((char *fmt, ...));
extern int sciprint2 _PARAMS((int i,char *fmt, ...));
extern void Scistring _PARAMS((char *str));
extern void Xputchar  _PARAMS((unsigned char c));  
extern void v_write  _PARAMS((int f, char *d, int len));  
extern void xevents1  _PARAMS((void));  
extern int in_put  _PARAMS((void));  

extern void unparseputn  _PARAMS((unsigned int n, int fd));  
extern void unparseputc  _PARAMS((char c, int fd));  
extern void unparsefputs  _PARAMS((register char *s, int fd));  
extern void VTRun  _PARAMS((int nostartup));  
extern void VTReset  _PARAMS((Boolean full));  
extern int set_character_class  _PARAMS((register char *s));  
extern void FindFontSelection  _PARAMS((char *atom_name, int justprobe));  
extern void SetVTFont  _PARAMS((int i, int doresize, char *name1, char *name2));  


/*  "x_cursor-n.c.X1"*/


/*  "x_data-n.c.X1"*/
/*  "x_input-n.c.X1"*/


/*  "x_main-n.c.X1"*/

extern int C2F(winsci) _PARAMS((char *pname, int *nos, int *idisp, char *display,int *lpname, int *ldisp));
extern void getcolordef  _PARAMS((integer *c));  
extern void setcolordef  _PARAMS((int screenc));  
/* extern void initColors  _PARAMS((void));   */
extern void main_sci  _PARAMS((int argc, char **argv));  
extern int clear_ieee_warnings  _PARAMS((void));  
extern void ClearExit  _PARAMS((integer n));  
extern int C2F(clearexit)  _PARAMS((int *n));  
extern int GetBytesAvailable  _PARAMS((int fd));  
extern int kill_process_group  _PARAMS((int pid, int sig)); 
     
     /*  "x_menu-n.c.X1"*/
     
extern void do_kill1  _PARAMS((int ));
     
     /*  "x_misc-n.c.X1"*/
     
extern void x_events  _PARAMS((void));  
extern void Bell  _PARAMS((void));  
extern void VisualBell  _PARAMS((void));  
extern void Redraw  _PARAMS((void));  
extern void do_osc  _PARAMS(( int (*func) ()));
extern void Changename  _PARAMS((register char *name));  
extern void Changetitle  _PARAMS((register char *name));  
extern void Panic  _PARAMS((char *s, int a));  
extern char *SysErrorMsg  _PARAMS((int n));  
extern void SysError  _PARAMS((int i));  
extern void Error  _PARAMS((int i));  
extern void Cleanup  _PARAMS((int code));  
extern char *strindex  _PARAMS((register char *s1, register char *s2));  
extern void xerror  _PARAMS((Display *d, register XErrorEvent *ev));  
extern void xioerror  _PARAMS((Display *dpy));  
extern void set_vt_visibility  _PARAMS((Boolean on));  
     
     /*  "x_screen-n.c.X1"*/
     
extern void Size2Scilab  _PARAMS((int rows, int cols));  
     
     /*  "x_scrollbar-n.c.X1"*/
     
     
     /* "x_tabs-n.c.X1" */
     
extern void TabReset  _PARAMS((unsigned int *tabs));  
extern void TabSet  _PARAMS((unsigned int *tabs, int col));  
extern void TabClear  _PARAMS((unsigned int *tabs, int col));  
extern int TabNext  _PARAMS((unsigned int *tabs, int col));  
extern void TabZonk  _PARAMS((unsigned int *tabs));  
     
     /* "x_test_loop-n.c.X1" */
     
     /* 1 "x_util-n.c.X1"*/
     
     /* 1 "x_zzledt-n.c.X1" */
extern void C2F (zzledt1) _PARAMS((char *buffer, int *buf_size, int *len_line, int *eof, long int dummy1));  
extern int ctrl_action  _PARAMS((int i));  

#endif /**  ALL_EXTERN_X1 **/


