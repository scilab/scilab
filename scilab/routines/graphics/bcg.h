/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifdef WIN32
#ifndef PERIWIN_BCG 
#define PERIWIN_BCG 
#endif 
#include <winuser.h>
#define Window HWND
#else
#ifndef WITH_GTK
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#endif 
#endif 

#ifdef WITH_GTK
#ifndef PERIGTK_BCG 
#define PERIGTK_BCG 

/** Structure to keep the graphic state  **/


typedef struct _menu_entry { 
  char *name; /* name of the entry */
  char *accel;/* name of accelerator */
  int status; /* active 1, not active 0, deleted -1*/
  int nsub  ; /* if menu_entry is a sub_menu nsub gives its number else its value is one */
  struct _menu_entry *subs; /* menu_entry has some submenus */
  int winid ; /* associated window */
  int action_type ; /* hard coded or macro */
  char *fname;/* name of function to be called*/
  struct _menu_entry *menu; /* point to the father for sub_menus */
  struct _menu_entry *next; /* next one .... */
} menu_entry;
#endif
#endif


#include "Entities.h"

typedef int (*EVTHANDLER) __PARAMS((int win,int x,int y,int ibut));

struct BCG 
{ 
  int CurWindow ;   /** Id of window **/
  int CWindowWidth ; /** graphic window width **/
  int CWindowHeight ; /** graphic window height **/
  int FontSize ;
  int FontId ;
  int CurHardSymb;
  int CurHardSymbSize;
  int CurLineWidth;
  int CurPattern;
  int CurColor;
  int CurPixmapStatus;
  int CurResizeStatus;
  int CurVectorStyle;
  int CurDrawFunction;
  int ClipRegionSet;
  int CurClipRegion[4];
  int CurDashStyle;
  char CurNumberDispFormat[20]; 
  int CurColorStatus;
  int IDLastPattern; /* number of last patter or color */
  /** Colormap Cmap; color map of current graphic window */
  int CmapFlag ; /* set to 1 if the Cmap has default colors */
  int Numcolors; /* number of colors */
  float *Red; /* red value: between 0 and 1 */
  float *Green; /* green value: between 0 and 1 */
  float *Blue; /* blue value: between 0 and 1 */
  int NumBackground;  /* number of Background */
  int NumForeground; /* number of Foreground in the color table */
  int NumHidden3d;  /* color for hidden 3d facets **/
  char EventHandler[25]; /* name of window event handler */
  /* NG beg */
  int CWindowWidthView;	 /** graphic window width **/
  int CWindowHeightView;  /** graphic window height **/
  sciPointObj *mafigure;  /* Entities root */
  int graphicsversion;
  /* NG end */
#ifdef WIN32
  /** windows specific objects **/
  HWND	hWndParent;     /* parent window handle */
  HWND CWindow ;   /** window of the top level widget of the graphic window **/
  HWND Statusbar;    /* status window */
  HDC hdcCompat ;  /** for pixmap version */
  HBITMAP hbmCompat ;  /** for pixmap version */
  MW  lpmw;        /** graphic window menu **/
  /** A reprendre avec des pointeurs comme en X11 XXXXXX **/
  COLORREF *Colors;/* vector of colors */ 
  HPEN hPen ;  /** the current pen for graphic window **/
  HBRUSH hBrush; /** the current brush **/
  LPGW   lpgw; /** to store a pointer to the graphwin structure **/
  int Inside_init; /** a flag to inhibit Graproc actions while in 
		     initgraphic **/
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;
#else 
#ifdef WITH_GTK
/** GTK specific objects **/
  guchar *Red;   /* vector of red value: between 0 and 255 */
  guchar *Green; /* vector of green value: between 0 and 255 */
  guchar *Blue;  /* vector of blue value: between 0 and 255 */
  gint bg;				/* Background */
  gint fg;                              /* Foreground */
  GtkWidget *window;			/* Graphics frame */
  GtkWidget *drawing;                   /* Drawable window */
  GtkWidget *scrolled;                  /* scrolled window */
  GtkWidget *CinfoW ;                   /* info widget */
  GtkWidget *vbox;                      /* vbox */
  GtkWidget *menubar;
  GtkItemFactory *item_factory;
  menu_entry * menu_entries;            /* */
  GdkPixmap *pixmap;                    /* Backing store */
  GdkDrawable *Cdrawable;               /* set to drawing->window or to a pixmap 
					 * when using animation mode 
					 * See CurPixmapStatus */
  GdkGC *wgc;
  GdkGC *stdgc;
  GdkColor gcol_bg; 
  GdkColor gcol_fg; 
  GdkRectangle clip;
  GdkCursor *gcursor;                   /* xclick cursor */ 
  GdkCursor *ccursor;                   /* standard cursor */
  GdkFont *font;
  int resize;				/* Window resized */
#else 
  /** Xwindow specific objects **/
  Drawable Cdrawable ; /** The drawable = CWindow or a Pixmap */
  Widget CinfoW ;  /** info widget of graphic window **/
  Widget Viewport; /** Viewport for graphic window **/
  Widget Panner ;  /** Panner widget **/
  Widget drawbox ; /** the widget associated to CWindow **/
  Widget popup   ; /** graphic window popupc **/
  Window CWindow ; /** the graphic window **/
  Window CBGWindow ; /** window of the top level graphic popup widget **/
  XID FontXID;
  Colormap Cmap; /* color map of current graphic window */
  Pixel *Colors; /* vector of colors 
		    Note that there are 2 colors more than Numcolors,
		    ie black and white at the end of this vector */
#endif
#endif
}  ;


extern struct BCG *GetWindowXgcNumber  (int i);

