/*  "x_misc-n.c.X1"*/
  static void DoSpecialEnterNotify  __PARAMS((register XEnterWindowEvent *ev));  
  static void DoSpecialLeaveNotify  __PARAMS((register XEnterWindowEvent *ev));  
  static int ChangeGroup  __PARAMS((String attribute, XtArgVal value));  
  static void withdraw_window  __PARAMS((Display *dpy, Window w, int scr));  
/*  "x_screen-n.c.X1"*/
  static int Reallocate  __PARAMS((ScrnBuf *sbuf, Char **sbufaddr, int nrow, int ncol, int oldrow, int oldcol));  
/*  "x_screen.nok-n.c.X1"*/
  static int Reallocate  __PARAMS((ScrnBuf *sbuf, Char **sbufaddr, int nrow, int ncol, int oldrow, int oldcol));  
/*  "x_scrollbar-n.c.X1"*/
  static void ResizeScreen  __PARAMS((register XtermWidget xw, int min_width, int min_height));  
  static Widget CreateScrollBar  __PARAMS((XtermWidget xw, int x, int y, int height));  
  static void RealizeScrollBar  __PARAMS((Widget sbw, TScreen *screen));  
  static void ScrollTextTo  __PARAMS((Widget scrollbarWidget, caddr_t closure, float *topPercent));  
  static void ScrollTextUpDownBy  __PARAMS((Widget scrollbarWidget, Opaque closure, int pixels));  
  static int specialcmplowerwiths  __PARAMS((char *a, char *b));  
  static int params_to_pixels  __PARAMS((TScreen *screen, String *params, int n));  
/* "x_tabs-n.c.X1" */
/* "x_test_loop-n.c.X1" */
  static int C2F __PARAMS((vide))(void);  
/* 1 "x_util-n.c.X1"*/
  static void copy_area  __PARAMS((TScreen *screen, int src_x, int src_y, unsigned int width, unsigned int height, int dest_x, int dest_y));  
  static void horizontal_copy_area  __PARAMS((TScreen *screen, int firstchar, int nchars, int amount));  
  static void vertical_copy_area  __PARAMS((TScreen *screen, int firstline, int nlines, int amount));  
/* 1 "x_zzledt-n.c.X1" */
  static void move_right  __PARAMS((char *source, int max_chars));  
  static void move_left  __PARAMS((char *source));  
  static void display_string  __PARAMS((char *string));  
  static void get_line  __PARAMS((int line_index, char *source));  
  static void save_line  __PARAMS((char *source));  
  static void backspace  __PARAMS((int n));  
  static void erase_nchar  __PARAMS((int n));  
  static int lines_equal  __PARAMS((char *source, int line_index));  
  static void strip_blank  __PARAMS((char *source));  
  static int gchar_no_echo  __PARAMS((void));  
  static int translate  __PARAMS((int ichar));  
  static int search_line_backward  __PARAMS((char *source));  
  static int search_line_forward  __PARAMS((char *source));  
