
#ifndef ALL_EXTERN_X
#define ALL_EXTERN_X


#ifdef __STDC__
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		()
#endif
#endif

#include "x_ptyxP.h"

/*  "x_button-n.c.X1"*/

extern Boolean SendMousePosition  _PARAMS((Widget w, XEvent *));  
extern void HandleSelectExtend  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleSelectEnd  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardSelectEnd  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleInsertSelection  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleSelectStart  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardSelectStart  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void TrackMouse  _PARAMS((int func, int startrow, int startcol, int firstrow, int lastrow));  
extern void HandleSelectSet  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleStartExtend  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardStartExtend  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void ScrollSelection  _PARAMS((register TScreen *, register int amount)); 
extern void ResizeSelection  _PARAMS((TScreen *, int rows, int cols));
extern int SetCharacterClassRange  _PARAMS((register int low, register int high, register int value));  
extern void TrackText  _PARAMS((register int frow, register int fcol, register int trow, register int tcol));  
extern void DisownSelection  _PARAMS((register XtermWidget termw));

/*  "x_charproc-n.c.X1"*/

extern int XEvorgetchar  _PARAMS((void));  
extern void Xputstring  _PARAMS((char *str, int n));  
extern void C2F (xscisncr) _PARAMS((char *str, integer *n,long int dummy));  
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

extern void unparseseq  _PARAMS((register ANSI *ap, int fd));  
extern void unparseputn  _PARAMS((unsigned int n, int fd));  
extern void unparseputc  _PARAMS((char c, int fd));  
extern void unparsefputs  _PARAMS((register char *s, int fd));  
extern void VTRun  _PARAMS((int nostartup));  
extern void VTInit1  _PARAMS((Widget parent));  
extern void ShowCursor  _PARAMS((void));  
extern void HideCursor  _PARAMS((void));  
extern void VTReset  _PARAMS((Boolean full));  
extern int set_character_class  _PARAMS((register char *s));  
extern void FindFontSelection  _PARAMS((char *atom_name, int justprobe));  
extern void HandleSetFont  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void SetVTFont  _PARAMS((int i, int doresize, char *name1, char *name2));  

extern void ansi_modes  _PARAMS((XtermWidget termw, void (*func) ( )));  
extern void dpmodes  _PARAMS((XtermWidget termw, void (*func) ( )));  
extern void savemodes  _PARAMS((XtermWidget termw));  
extern void restoremodes  _PARAMS((XtermWidget termw));  
extern void ToAlternate  _PARAMS((register TScreen *));  
extern void FromAlternate  _PARAMS((register TScreen *));  
extern void SwitchBufPtrs  _PARAMS((register TScreen *));  
extern int LoadNewFont  _PARAMS((TScreen *, char *nfontname, char *bfontname, int doresize, int fontnum));  
extern void update_font_info  _PARAMS((TScreen *, int doresize));  
extern void set_vt_box  _PARAMS((TScreen *));  
extern void set_cursor_gcs  _PARAMS((TScreen *));  

/*  "x_cursor-n.c.X1"*/

extern void CursorSet  _PARAMS((register TScreen *, register int row, register int col, unsigned int flags));  
extern void CursorBack  _PARAMS((register TScreen *, int n));  
extern void CursorForward  _PARAMS((register TScreen *, int n));  
extern void CursorDown  _PARAMS((register TScreen *, int n));  
extern void CursorUp  _PARAMS((register TScreen *, int n));  
extern void Index  _PARAMS((register TScreen *, register int amount));  
extern void RevIndex  _PARAMS((register TScreen *, register int amount));  
extern void CarriageReturn  _PARAMS((register TScreen *));  
extern void CursorSave  _PARAMS((register XtermWidget term1, register SavedCursor *sc));  
extern void CursorRestore  _PARAMS((register XtermWidget term1, register SavedCursor *sc));  

/*  "x_data-n.c.X1"*/
/*  "x_input-n.c.X1"*/

extern void Input  _PARAMS((register TKeyboard *keyboard, register TScreen *, register XKeyEvent *, int eightbit));  
extern void StringInput  _PARAMS((register TScreen *, register char *string, int nbytes));  

/*  "x_main-n.c.X1"*/

extern void DeleteWindow  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void KeyboardMapping  _PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern int Xscilab  _PARAMS((Display **dpy, Widget *topwid));  
extern int C2F(winsci) _PARAMS((char *pname, int *nos, int *idisp, char *display,int *lpname, int *ldisp));
extern void getcolordef  _PARAMS((integer *c));  
extern void setcolordef  _PARAMS((int screenc));  
extern Widget initColors  _PARAMS((Widget));  
extern void main_sci  _PARAMS((int argc, char **argv));  
extern int clear_ieee_warnings  _PARAMS((void));  
extern void ClearExit  _PARAMS((integer n));  
extern int C2F(clearexit)  _PARAMS((int *n));  
extern int GetBytesAvailable  _PARAMS((int fd));  
extern int kill_process_group  _PARAMS((int pid, int sig)); 
     
     /*  "x_menu-n.c.X1"*/
     
extern void HandleCreateMenu  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandlePopupMenu  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void do_kill  _PARAMS((Widget gw, caddr_t closure, caddr_t data));  
extern void do_kill1  _PARAMS((int ));
extern void HandleSetVisualBell  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleRedraw  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleSendSignal  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleQuit  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollbar  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleJumpscroll  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleReverseVideo  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAutoWrap  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleReverseWrap  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAutoLineFeed  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAppCursor  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAppKeypad  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollKey  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollTtyOutput  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAllow132  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleCursesEmul  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleMarginBell  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAltScreen  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleSoftReset  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleHardReset  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleClearSavedLines  _PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
     
     /*  "x_misc-n.c.X1"*/
     
extern void x_events  _PARAMS((void));  
extern Cursor make_colored_cursor  _PARAMS((int cursorindex, long unsigned int fg, long unsigned int bg));  
extern void HandleKeyPressed  _PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleEightBitKeyPressed  _PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleStringEvent  _PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleEnterWindow  _PARAMS((Widget w, caddr_t eventdata, register XEnterWindowEvent *));  
extern void HandleLeaveWindow  _PARAMS((Widget w, caddr_t eventdata, register XEnterWindowEvent *));  
extern void HandleFocusChange  _PARAMS((Widget w, caddr_t eventdata, register XFocusChangeEvent *));  
extern void selectwindow  _PARAMS((register TScreen *, register int flag));  
extern void unselectwindow  _PARAMS((register TScreen *, register int flag));  
extern void Bell  _PARAMS((void));  
extern void VisualBell  _PARAMS((void));  
extern void HandleBellPropertyChange  _PARAMS((Widget w, XtPointer data, XEvent *ev, Boolean *more));  
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
     
extern ScrnBuf Allocate  _PARAMS((register int nrow, register int ncol, Char **addr));  
extern void ScreenWrite  _PARAMS((TScreen *, char *str, register unsigned int flags, register int length));  
extern void ScrnInsertLine  _PARAMS((register ScrnBuf sb, int last, register int where, register int n, register int size));  
extern void ScrnDeleteLine  _PARAMS((register ScrnBuf sb, register int last, int where, register int n, register int size));  
extern void ScrnInsertChar  _PARAMS((ScrnBuf sb, int row, register int col, register int n, int size));  
extern void ScrnDeleteChar  _PARAMS((ScrnBuf sb, register int row, register int col, register int n, register int size));  
extern void ScrnRefresh  _PARAMS((register TScreen *, int toprow, int leftcol, int nrows, int ncols, Boolean force));  
extern void ClearBufRows  _PARAMS((register TScreen *, register int first, register int last));  
extern int ScreenResize  _PARAMS((register TScreen *, int width, int height, unsigned int *flags));  
extern void Size2Scilab  _PARAMS((int rows, int cols));  
extern void ScrnSetAttributes  _PARAMS((TScreen *, int row, int col, unsigned int mask, unsigned int value, register int length));  
extern int ScrnGetAttributes  _PARAMS((TScreen *, int row, int col, Char *str, register int length));  
extern int non_blank_line  _PARAMS((ScrnBuf sb, register int row, register int col, register int len));  
     
     /*  "x_scrollbar-n.c.X1"*/
     
extern void DoResizeScreen  _PARAMS((register XtermWidget xw));  
extern void ScrollBarReverseVideo  _PARAMS((register Widget scrollWidget));  
extern void ScrollBarDrawThumb  _PARAMS((register Widget scrollWidget));  
extern void ResizeScrollBar  _PARAMS((register Widget scrollWidget, int x, int y, unsigned int height));  
extern void WindowScroll  _PARAMS((register TScreen *, int top));  
extern void ScrollBarOn  _PARAMS((XtermWidget xw, int init, int doalloc));  
extern void ScrollBarOff  _PARAMS((register TScreen *));  
extern void HandleScrollForward  _PARAMS((Widget gw, XEvent *, String *, Cardinal *nparams));  
extern void HandleScrollBack  _PARAMS((Widget gw, XEvent *, String *, Cardinal *nparams));  
     
     /* "x_tabs-n.c.X1" */
     
extern void TabReset  _PARAMS((unsigned int *tabs));  
extern void TabSet  _PARAMS((unsigned int *tabs, int col));  
extern void TabClear  _PARAMS((unsigned int *tabs, int col));  
extern int TabNext  _PARAMS((unsigned int *tabs, int col));  
extern void TabZonk  _PARAMS((unsigned int *tabs));  
     
     /* "x_test_loop-n.c.X1" */
     /* 1 "x_util-n.c.X1"*/
     
extern void FlushScroll  _PARAMS((register TScreen *));  
extern int AddToRefresh  _PARAMS((register TScreen *));  
extern void Scroll  _PARAMS((register TScreen *, register int amount));  
extern void RevScroll  _PARAMS((register TScreen *, register int amount));  
extern void InsertLine  _PARAMS((register TScreen *, register int n));  
extern void DeleteLine  _PARAMS((register TScreen *, register int n));  
extern void InsertChar  _PARAMS((register TScreen *, register int n));  
extern void DeleteChar  _PARAMS((register TScreen *, register int n));  
extern void ClearAbove  _PARAMS((register TScreen *));  
extern void ClearBelow  _PARAMS((register TScreen *));  
extern void ClearRight  _PARAMS((register TScreen *));  
extern void ClearLeft  _PARAMS((register TScreen *));  
extern void ClearLine  _PARAMS((register TScreen *));  
extern void ClearScreen  _PARAMS((register TScreen *));  
extern void CopyWait  _PARAMS((register TScreen *));  
extern void scrolling_copy_area  _PARAMS((TScreen *, int firstline, int nlines, int amount));  
extern int HandleExposure  _PARAMS((register TScreen *, register XEvent *));  
extern int handle_translated_exposure  _PARAMS((register TScreen *, register int rect_x, register int rect_y, register int rect_width, register int rect_height));  
extern void ReverseVideo  _PARAMS((XtermWidget termw));  
extern void recolor_cursor  _PARAMS((Cursor cursor, long unsigned int fg, long unsigned int bg));  
     /* 1 "x_zzledt-n.c.X1" */
extern void C2F (zzledt1) _PARAMS((char *buffer, int *buf_size, int *len_line, int *eof, long int dummy1));  
extern int ctrl_action  _PARAMS((int i));  

#endif /**  ALL_EXTERN_X **/


