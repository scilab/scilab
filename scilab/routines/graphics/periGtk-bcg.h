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

typedef int (*EVTHANDLER) __PARAMS((int win,int x,int y,int ibut));

typedef struct BCG 
{ 
  int CurWindow ;   /** Id of window **/
  int CWindowWidth ; /** graphic window width **/
  int CWindowHeight ; /** graphic window height **/
  int fontId ;
  int fontSize ;
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
  int IDLastPattern; /* number of last pattern or color 
		      in color mode = Numcolors - 1 */
  int CmapFlag ; /* set to 1 if the Cmap has default colors */
  int Numcolors; /* number of colors */
  int NumBackground;  /* number of Background in the color table */
  int NumForeground; /* number of Foreground in the color table */
  int NumHidden3d;  /* color for hidden 3d facets **/
  char EventHandler[25]; /* name of window event handler */
  /* new for Gtk */
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
} BCG; 

/* A revoir */

#define CoordModeOrigin 0
#define CoordModePrevious 1

void * graphic_initial_menu(int winid);

/** jpc_SGraph.c **/

extern int CheckClickQueue   __PARAMS((integer *,integer *x,integer *y,integer *ibut));
extern int ClearClickQueue  __PARAMS((integer));
extern int PushClickQueue __PARAMS((int,int ,int y,int ibut,int m,int r));


/** jpc_Xloop.c **/

extern integer C2F(ismenu) __PARAMS((void));
extern int C2F(getmen) __PARAMS((char *btn_cmd,integer *lb,integer *entry));
extern void MenuFixCurrentWin __PARAMS(( int ivalue));

extern struct BCG *GetWindowXgcNumber  __PARAMS((integer i));

#endif 

