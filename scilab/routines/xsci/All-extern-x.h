
#ifndef ALL_EXTERN_X
#define ALL_EXTERN_X


#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif

#include "x_ptyxP.h"

/*  "x_button-n.c.X1"*/

extern Boolean SendMousePosition  __PARAMS((Widget w, XEvent *));  
extern void HandleSelectExtend  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleSelectEnd  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardSelectEnd  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleInsertSelection  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleSelectStart  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardSelectStart  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void TrackMouse  __PARAMS((int func, int startrow, int startcol, int firstrow, int lastrow));  
extern void HandleSelectSet  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleStartExtend  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void HandleKeyboardStartExtend  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void ScrollSelection  __PARAMS((register TScreen *, register int amount)); 
extern void ResizeSelection  __PARAMS((TScreen *, int rows, int cols));
extern int SetCharacterClassRange  __PARAMS((register int low, register int high, register int value));  
extern void TrackText  __PARAMS((register int frow, register int fcol, register int trow, register int tcol));  
extern void DisownSelection  __PARAMS((register XtermWidget termw));

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

extern void unparseseq  __PARAMS((register ANSI *ap, int fd));  
extern void unparseputn  __PARAMS((unsigned int n, int fd));  
extern void unparseputc  __PARAMS((char c, int fd));  
extern void unparsefputs  __PARAMS((register char *s, int fd));  
extern void VTRun  __PARAMS((char *,int lstartup,int memory));  
extern void VTInit1  __PARAMS((Widget parent));  
extern void ShowCursor  __PARAMS((void));  
extern void HideCursor  __PARAMS((void));  
extern void VTReset  __PARAMS((Boolean full));  
extern int set_character_class  __PARAMS((register char *s));  
extern void FindFontSelection  __PARAMS((char *atom_name, int justprobe));  
extern void HandleSetFont  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void SetVTFont  __PARAMS((int i, int doresize, char *name1, char *name2));  

extern void ansi_modes  __PARAMS((XtermWidget termw, void (*func) ( )));  
extern void dpmodes  __PARAMS((XtermWidget termw, void (*func) ( )));  
extern void savemodes  __PARAMS((XtermWidget termw));  
extern void restoremodes  __PARAMS((XtermWidget termw));  
extern void ToAlternate  __PARAMS((register TScreen *));  
extern void FromAlternate  __PARAMS((register TScreen *));  
extern void SwitchBufPtrs  __PARAMS((register TScreen *));  
extern int LoadNewFont  __PARAMS((TScreen *, char *nfontname, char *bfontname, int doresize, int fontnum));  
extern void update_font_info  __PARAMS((TScreen *, int doresize));  
extern void set_vt_box  __PARAMS((TScreen *));  
extern void set_cursor_gcs  __PARAMS((TScreen *));  

/*  "x_cursor-n.c.X1"*/

extern void CursorSet  __PARAMS((register TScreen *, register int row, register int col, unsigned int flags));  
extern void CursorBack  __PARAMS((register TScreen *, int n));  
extern void CursorForward  __PARAMS((register TScreen *, int n));  
extern void CursorDown  __PARAMS((register TScreen *, int n));  
extern void CursorUp  __PARAMS((register TScreen *, int n));  
extern void Index  __PARAMS((register TScreen *, register int amount));  
extern void RevIndex  __PARAMS((register TScreen *, register int amount));  
extern void CarriageReturn  __PARAMS((register TScreen *));  
extern void CursorSave  __PARAMS((register XtermWidget term1, register SavedCursor *sc));  
extern void CursorRestore  __PARAMS((register XtermWidget term1, register SavedCursor *sc));  

/*  "x_data-n.c.X1"*/
/*  "x_input-n.c.X1"*/

extern void Input  __PARAMS((register TKeyboard *keyboard, register TScreen *, register XKeyEvent *, int eightbit));  
extern void StringInput  __PARAMS((register TScreen *, register char *string, int nbytes));  

/*  "x_main-n.c.X1"*/

extern void DeleteWindow  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern void KeyboardMapping  __PARAMS((Widget w, XEvent *, String *, Cardinal *));  
extern int Xscilab  __PARAMS((Display **dpy, Widget *topwid));  
extern int C2F(winsci) __PARAMS((char *pname, int *nos, int *idisp, char *display,int *lpname, int *ldisp));
extern void getcolordef  __PARAMS((integer *c));  
extern void setcolordef  __PARAMS((int screenc));  
extern Widget initColors  __PARAMS((Widget));  
extern void main_sci  __PARAMS((int argc, char **argv, char *, int,int ));  
extern int clear_ieee_warnings  __PARAMS((void));  
extern void sci_clear_and_exit  __PARAMS((integer n));  
extern int C2F(clearexit)  __PARAMS((int *n));  
extern int GetBytesAvailable  __PARAMS((int fd));  
extern int kill_process_group  __PARAMS((int pid, int sig)); 
     
     /*  "x_menu-n.c.X1"*/
     
extern void HandleCreateMenu  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandlePopupMenu  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void do_kill  __PARAMS((Widget gw, caddr_t closure, caddr_t data));  
extern void do_kill1  __PARAMS((int ));
extern void HandleSetVisualBell  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleRedraw  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleSendSignal  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleQuit  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollbar  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleJumpscroll  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleReverseVideo  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAutoWrap  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleReverseWrap  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAutoLineFeed  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAppCursor  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAppKeypad  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollKey  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleScrollTtyOutput  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAllow132  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleCursesEmul  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleMarginBell  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleAltScreen  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleSoftReset  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleHardReset  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
extern void HandleClearSavedLines  __PARAMS((Widget w, XEvent *, String *, Cardinal *param_count));  
     
     /*  "x_misc-n.c.X1"*/
     
extern void x_events  __PARAMS((void));  
extern Cursor make_colored_cursor  __PARAMS((int cursorindex, long unsigned int fg, long unsigned int bg));  
extern void HandleKeyPressed  __PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleEightBitKeyPressed  __PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleStringEvent  __PARAMS((Widget w, XEvent *, String *, Cardinal *nparams));  
extern void HandleEnterWindow  __PARAMS((Widget w, caddr_t eventdata, register XEnterWindowEvent *));  
extern void HandleLeaveWindow  __PARAMS((Widget w, caddr_t eventdata, register XEnterWindowEvent *));  
extern void HandleFocusChange  __PARAMS((Widget w, caddr_t eventdata, register XFocusChangeEvent *));  
extern void selectwindow  __PARAMS((register TScreen *, register int flag));  
extern void unselectwindow  __PARAMS((register TScreen *, register int flag));  
extern void Bell  __PARAMS((void));  
extern void VisualBell  __PARAMS((void));  
extern void HandleBellPropertyChange  __PARAMS((Widget w, XtPointer data, XEvent *ev, Boolean *more));  
extern void Redraw  __PARAMS((void));  
extern void do_osc  __PARAMS(( int (*func) ()));
extern void Changename  __PARAMS((register char *name));  
extern void Changetitle  __PARAMS((register char *name));  
extern void Panic  __PARAMS((char *s, int a));  
extern const char *SysErrorMsg  __PARAMS((int n));  
extern void SysError  __PARAMS((int i));  
extern void Error  __PARAMS((int i));  
extern void Cleanup  __PARAMS((int code));  
extern char *strindex  __PARAMS((register char *s1, register char *s2));  
extern void xerror  __PARAMS((Display *d, register XErrorEvent *ev));  
extern void xioerror  __PARAMS((Display *dpy));  
extern void set_vt_visibility  __PARAMS((Boolean on));  
     
     /*  "x_screen-n.c.X1"*/
     
extern ScrnBuf Allocate  __PARAMS((register int nrow, register int ncol, Char **addr));  
extern void ScreenWrite  __PARAMS((TScreen *, char *str, register unsigned int flags, register int length));  
extern void ScrnInsertLine  __PARAMS((register ScrnBuf sb, int last, register int where, register int n, register int size));  
extern void ScrnDeleteLine  __PARAMS((register ScrnBuf sb, register int last, int where, register int n, register int size));  
extern void ScrnInsertChar  __PARAMS((ScrnBuf sb, int row, register int col, register int n, int size));  
extern void ScrnDeleteChar  __PARAMS((ScrnBuf sb, register int row, register int col, register int n, register int size));  
extern void ScrnRefresh  __PARAMS((register TScreen *, int toprow, int leftcol, int nrows, int ncols, Boolean force));  
extern void ClearBufRows  __PARAMS((register TScreen *, register int first, register int last));  
extern int ScreenResize  __PARAMS((register TScreen *, int width, int height, unsigned int *flags));  
extern void Size2Scilab  __PARAMS((int rows, int cols));  
extern void ScrnSetAttributes  __PARAMS((TScreen *, int row, int col, unsigned int mask, unsigned int value, register int length));  
extern int ScrnGetAttributes  __PARAMS((TScreen *, int row, int col, Char *str, register int length));  
extern int non_blank_line  __PARAMS((ScrnBuf sb, register int row, register int col, register int len));  
     
     /*  "x_scrollbar-n.c.X1"*/
     
extern void DoResizeScreen  __PARAMS((register XtermWidget xw));  
extern void ScrollBarReverseVideo  __PARAMS((register Widget scrollWidget));  
extern void ScrollBarDrawThumb  __PARAMS((register Widget scrollWidget));  
extern void ResizeScrollBar  __PARAMS((register Widget scrollWidget, int x, int y, unsigned int height));  
extern void WindowScroll  __PARAMS((register TScreen *, int top));  
extern void ScrollBarOn  __PARAMS((XtermWidget xw, int init, int doalloc));  
extern void ScrollBarOff  __PARAMS((register TScreen *));  
extern void HandleScrollForward  __PARAMS((Widget gw, XEvent *, String *, Cardinal *nparams));  
extern void HandleScrollBack  __PARAMS((Widget gw, XEvent *, String *, Cardinal *nparams));  
     
     /* "x_tabs-n.c.X1" */
     
extern void TabReset  __PARAMS((unsigned int *tabs));  
extern void TabSet  __PARAMS((unsigned int *tabs, int col));  
extern void TabClear  __PARAMS((unsigned int *tabs, int col));  
extern int TabNext  __PARAMS((unsigned int *tabs, int col));  
extern void TabZonk  __PARAMS((unsigned int *tabs));  
     
     /* "x_test_loop-n.c.X1" */
     /* 1 "x_util-n.c.X1"*/
     
extern void FlushScroll  __PARAMS((register TScreen *));  
extern int AddToRefresh  __PARAMS((register TScreen *));  
extern void Scroll  __PARAMS((register TScreen *, register int amount));  
extern void RevScroll  __PARAMS((register TScreen *, register int amount));  
extern void InsertLine  __PARAMS((register TScreen *, register int n));  
extern void DeleteLine  __PARAMS((register TScreen *, register int n));  
extern void InsertChar  __PARAMS((register TScreen *, register int n));  
extern void DeleteChar  __PARAMS((register TScreen *, register int n));  
extern void ClearAbove  __PARAMS((register TScreen *));  
extern void ClearBelow  __PARAMS((register TScreen *));  
extern void ClearRight  __PARAMS((register TScreen *));  
extern void ClearLeft  __PARAMS((register TScreen *));  
extern void ClearLine  __PARAMS((register TScreen *));  
extern void ClearScreen  __PARAMS((register TScreen *));  
extern void CopyWait  __PARAMS((register TScreen *));  
extern void scrolling_copy_area  __PARAMS((TScreen *, int firstline, int nlines, int amount));  
extern int HandleExposure  __PARAMS((register TScreen *, register XEvent *));  
extern int handle_translated_exposure  __PARAMS((register TScreen *, register int rect_x, register int rect_y, register int rect_width, register int rect_height));  
extern void ReverseVideo  __PARAMS((XtermWidget termw));  
extern void recolor_cursor  __PARAMS((Cursor cursor, long unsigned int fg, long unsigned int bg));  
     /* 1 "x_zzledt-n.c.X1" */
extern void C2F (zzledt1) __PARAMS((char *buffer, int *buf_size, int *len_line, int *eof, long int dummy1));  


#endif /**  ALL_EXTERN_X **/


