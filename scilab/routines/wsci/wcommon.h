/* wcommon.h */

/*
 * Copyright (C) 1992   Maurice Castro, Russell Lang
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 * AUTHORS
 *   Maurice Castro
 *   Russell Lang
 * Modified for Scilab : Jean-Philippe Chancelier 
 */

#ifndef SCI_WCOMMON
#define SCI_WCOMMON


#include <stdio.h>
#include "../graphics/Math.h" /* Scilab graphic window */
#include "../graphics/bcg.h" /* Scilab graphic window */
#ifndef __GNUC__ 
#include <shellapi.h>
#endif
#include "wadd.h" 

#include "common.h" 

/* wgnuplib.c */

extern HINSTANCE hdllInstance;
extern LPSTR szParentClass;
extern LPSTR szTextClass;
extern LPSTR szPauseClass;
extern LPSTR szGraphClass;
extern LPSTR szParentGraphClass;
extern LPSTR szAboutClass;

extern void * LocalAllocPtr(UINT flags, UINT size);
extern void LocalFreePtr(void  *ptr);
extern LPSTR GetInt(LPSTR str, LPINT pval);
extern LPSTR GetLInt(LPSTR str, long int *pval);

/* wtext.c */

extern void TextSelectFont (LPTW lptw);
extern void TextCopyClip (LPTW lptw);
extern int CtrlCHit(LPTW lptw);
extern void UpdateText(LPTW, int);
extern void NewLine(LPTW);
extern void TextPutStr(LPTW lptw, LPSTR str);
extern void WriteTextIni(LPTW lptw);
extern void ReadTextIni(LPTW lptw);
extern void DragFunc(LPTW lptw, HDROP hdrop);
extern int C2F(sxevents)();

/* wmenu.c - Menu */

#define GBUFSIZE 512

typedef struct tagGFILE {
	HFILE	hfile;
	char 	getbuf[GBUFSIZE];
	int	getnext;
	int	getleft;
} GFILE;

extern void SendMacro(LPTW lptw, UINT m);
extern void LoadMacros(LPTW lptw);
extern void CloseMacros(LPTW lptw);
extern void MenuFixCurrentWin(int ivalue);
extern BOOL SciOpenSave(HWND hWndParent,  BYTE **s,BOOL save,  char **d,int *ierr);
extern int GetLine(char * buffer, int len, GFILE *gfile);
extern void LeftJustify(char *d, char *s);
extern GFILE * Gfopen(LPSTR lpszFileName, int fnOpenMode);
extern void Gfclose(GFILE * gfile);
extern int Gfgets(LPSTR lp, int size, GFILE *gfile);

/* wprinter.c - Printer setup and dump */

extern BOOL PrintSize(HDC printer, HWND hwnd, LPRECT lprect);
extern void PrintRegister(LP_PRINT lpr);
extern void PrintUnregister(LP_PRINT lpr);

extern EXPORT BOOL CALLBACK  PrintAbortProc(HDC hdcPrn, int code);
extern EXPORT BOOL CALLBACK  PrintDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/* wstatbar.c */

extern HWND InitStatusBar (HWND hwndParent);

/* command.c */

extern void do_shell();
extern void do_system();
extern int com_line(void);
extern void switch_rlgets(int);
extern int read_line (char *prompt, int interrupt);
extern int winsystem(char *,int );

/* winmain.c */

extern void sciprint(char *fmt, ...);
extern void sciprint_nd(char *fmt, ...);
extern int sciprint2(int v,char *fmt,...);

extern int Pause(LPSTR mess); 
extern void WinExit(void);
extern EXPORT int CALLBACK ShutDown(void);
extern void Scistring(char *str);
extern int C2F(clearexit)(integer *n);
extern int ReadKey(void);
extern int WriteStr(char *s, FILE *file);
extern int WriteKey(char c, FILE *file);

/* ../menusX */

extern BOOL  DialogWindow();

/* readline-nowin */

extern void add_history_nw();
extern char * readline_nw (char *prompt, int interrupt);

/* readline */

extern void add_history_win();
extern char * readline_win (char *prompt,int interrupt);

/* wgmenu */

extern int WGFindMenuPos( BYTE **macros);
extern void ScilabMenuAction(char *buf);
extern void SendGraphMacro(struct BCG *ScilabGC, UINT m);
extern void write_scilab(char *buf);
extern void  LoadGraphMacros(struct BCG *ScilabGC);
extern void CloseGraphMacros(struct BCG *ScilabGC);
extern int C2F(setmen)(integer *,char *,integer *,integer *,integer *,
		       integer *);
extern int C2F(unsmen)(integer *,char *,integer *,integer *,integer *,
		       integer *);
extern int C2F(chmenu)(integer *,char *,char *);
extern int C2F(delbtn)(integer *,char *);
extern int C2F(addmen)(integer *,char *,integer *,integer *,integer *,
		       integer *,
		       char *,	integer *);
extern void AddMenu(integer *win_num,char * button_name,char ** entries,
	     integer * ne,integer *typ,char * fname, integer * ierr);

/* gvwprn.c */

extern int gp_printfile(HINSTANCE hInstance,HWND hwnd,char *filename, char *port);

/* jpc_Xloop */

extern void SetXsciOn();
extern int C2F(xscion)(int *i);
extern void C2F(winsci) (char *pname,int *nos,int *idisp,char *display,
	     long int dummy1,long int dummy2);
extern int C2F(getmen)(char *,integer *,integer *);
extern integer C2F(ismenu)(void);
extern int GetCommand(char *);
extern int StoreCommand1(char *,int);
extern int StoreCommand(char *);

/* misc */

extern char *alloc(unsigned long size,char * message);
extern void squash_spaces(char *s);
extern void lower_case(char *s);
extern void os_error(char *str,int t_num);
extern void int_error(char *str,int t_num);
extern int instring(char *str,char  c);

/* wtloop */

extern void sci_windows_main(int,int *,char *,int,int *,int);
extern void SciMenuSelect(char *Name);
extern void  C2F(setfbutn)(char *buf,int *rep);
extern void  C2F(fbutn)(char *buf,int *win,int *ent);
extern void  SignalCtrC(void);

/* wgnuplib */

#ifndef __DLL__
extern void  NoDll(HINSTANCE hInstance);
#endif

/** wgraph */
extern void DebugGW (char *fmt, ...);
extern EXPORT void WINAPI  GraphPrint(struct BCG *ScilabGC);
extern EXPORT void WINAPI  GraphRedraw(struct BCG *ScilabGC);
extern int C2F(deletewin)(integer *number) ;
extern void NewCopyClip(struct BCG *ScilabGC);
extern void CopyClip(struct BCG *ScilabGC);
extern int CopyPrint(struct BCG *ScilabGC);
extern void WriteGraphIni(struct BCG *ScilabGC);
extern void ReadGraphIni(struct BCG *ScilabGC);
extern int PushClickQueue(int win,int x,int y,int ibut,int motion,int release);
extern int CheckClickQueue(int *win,integer *x,integer* y,integer *ibut);
extern int ClearClickQueue(int win);

/** ../graphics* **/

extern void Tape_Replay(char *,integer *,integer *,integer *,integer *,integer *,integer *,double *,double *,double *,double *);
extern void C2F(SetDriver)();
extern int C2F(dr)();

/* wmprint */

extern int ScilabPsToEps(char orientation,char *filenamein,char *filenameout);

/* wmtex.c */

extern int ScilabPsToTeX(char orientation,char *filenamein,char *filenameout,double xs,double ys);

/* elsewhere in scilab : in Fortran code maybe */

extern int C2F(mainsci)(int *, char *, int *, integer);
extern void set_is_reading(int);
extern int get_is_reading(void);
extern int C2F(timer)(double *);
extern int C2F(stimer)(void);
extern int C2F(sigbas)(integer *);

/* handlers */

extern void  reset_scig_handler __PARAMS((void));
extern void  reset_scig_click_handler __PARAMS((void));
extern void  reset_scig_deletegwin_handler __PARAMS((void));
extern void  reset_scig_command_handler __PARAMS((void));

typedef int (*Scig_command_handler) __PARAMS((char *));
extern Scig_command_handler set_scig_command_handler __PARAMS((Scig_command_handler f));

typedef int (*Scig_click_handler) __PARAMS((int,int,int,int,int,int));
extern Scig_click_handler set_scig_click_handler __PARAMS((Scig_click_handler f));

typedef void (*Scig_deletegwin_handler) __PARAMS((int));
extern Scig_deletegwin_handler set_scig_deletegwin_handler __PARAMS((Scig_deletegwin_handler f));




#endif 
