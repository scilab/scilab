/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985-1988 by Supoj Sutanthavibul
 * Parts Copyright (c) 1989-2002 by Brian V. Smith
 * Parts Copyright (c) 1991 by Paul King
 *
 * Any party obtaining a copy of these files is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish and/or distribute copies of
 * the Software, and to permit persons who receive copies from any such 
 * party to do so, with the only requirement being that this copyright 
 * notice remain intact.
 *
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include "paintop.h"

#define NUMSHADEPATS	21
#define NUMTINTPATS	20
#define NUMPATTERNS	22
#define NUMFILLPATS	NUMSHADEPATS+NUMTINTPATS+NUMPATTERNS

/* page sizes used when imported EPS has no bounding box or pdf has no /MediaBox */
#define LETTER_WIDTH	10200
#define LETTER_HEIGHT	13200
#define A4_WIDTH	 9921
#define A4_HEIGHT	14031

/* min, max zoom allowed */
#define	MIN_ZOOM	0.01
#define	MAX_ZOOM	100

/* starting, min, max depth of objects on canvas */
#define DEF_DEPTH	50
#define MIN_DEPTH	0
#define MAX_DEPTH	999

/* min, max font size (points) */
#define MIN_FONT_SIZE	1
#define MAX_FONT_SIZE	500

/* min, max font size on the display (after zooming) */
/* this is necessary because some X servers crash completely
   when rendering very small fonts */
#define MIN_X_FONT_SIZE	5
#define MAX_X_FONT_SIZE	MAX_FONT_SIZE

/* maximum width of lines (Fig units) */
#define MAX_LINE_WIDTH 500

/* max number of sides for regular polygon */
#define MIN_POLY_SIDES	3
#define MAX_POLY_SIDES	200

/* min, max vertical spacing when entering text (fraction of font size) */
#define MIN_TEXT_STEP	0
#define MAX_TEXT_STEP	100

/* min, max arc-box corner radius (1/80 inch) */
#define MIN_BOX_RADIUS	2
#define MAX_BOX_RADIUS	1000

/* number of standard colors */
#define NUM_STD_COLS	32
/* max number of user-defined colors */
#define MAX_USR_COLS	512

/* number of paper sizes (A4, B5, letter, etc. [see resources.c]) */
#define NUMPAPERSIZES	29

/* define these positions so we can start with default paper size */
#define PAPER_LETTER	0
#define PAPER_A4	13

#define	Color		int

/* default number of colors to use for GIF/XPM */
/* this can be overridden in resources or command-line arg */
#define DEF_MAX_IMAGE_COLS 64

/* max and default number of recently used files for File menu */
#define MAX_RECENT_FILES	9
#define DEF_RECENT_FILES	5

/* for picture files */
#define	MAX_COLORMAP_SIZE	MAX_USR_COLS

/* for JPEG export */
#define	DEF_JPEG_QUALITY	75

/* default border margin for export */
#define DEF_EXPORT_MARGIN	0

/* for screen capture */

#define	IMAGE_PALETTE	0	/* colormapped screen capture */
#define IMAGE_RGB	1	/* RGB (TrueColor) screen capture */

struct Cmap {
	unsigned short red, green, blue;
	unsigned long pixel;
};

typedef struct {
		char *name,
		     *rgb;
		} fig_colors ;

/* for the xfig icon */
extern Pixmap		 fig_icon;

/* version string for xfig (generated in main() )*/
extern char		 xfig_version[];

/* these are allocated in main() in case we aren't using default colormap 
   (so we can't use BlackPixelOfScreen... */

extern XColor		 black_color, white_color;

/* original directory where xfig started */
extern char		 orig_dir[PATH_MAX+2];

/* whether user is updating Fig files or loading one to view */
extern Boolean		 update_figs;

#ifdef USE_XPM
#include <xpm.h>
extern XpmAttributes	 xfig_icon_attr;
#endif
extern fig_colors	 colorNames[NUM_STD_COLS + 1];
extern char		*short_clrNames[NUM_STD_COLS + 1];
extern Pixel		 colors[NUM_STD_COLS+MAX_USR_COLS];
extern XColor		 user_colors[MAX_USR_COLS];
extern XColor		 undel_user_color;
extern XColor		 n_user_colors[MAX_USR_COLS];
extern XColor		 save_colors[MAX_USR_COLS];
extern int		 num_usr_cols, n_num_usr_cols;
extern int		 current_memory;
extern Boolean		 colorUsed[MAX_USR_COLS];
extern Boolean		 colorFree[MAX_USR_COLS];
extern Boolean		 n_colorFree[MAX_USR_COLS];
extern Boolean		 all_colors_available;
extern Pixel		 dark_gray_color, med_gray_color, lt_gray_color;
extern Pixel		 pageborder_color;
extern Pixel		 zero_lines_color;
extern int		 max_depth, min_depth;
extern char		 tool_name[200];
extern int		 export_background_color; /* current export/print background color */
extern Boolean		 display_fractions;	/* whether to display fractions in lengths */
extern char		*userhome;		/* user's home directory */
extern struct _pics	*pictures;		/* common repository to share imported pictures */
extern float		 scale_factor;		/* scale drawing as it is read in */

/* number of colors we want to use for GIF/XPM images */
extern int		avail_image_cols;
/* colormap used for same */
extern XColor		image_cells[MAX_COLORMAP_SIZE];

/* resources structure */

typedef struct _appres {
    Boolean	    allownegcoords;	/* allow negative x/y coordinates for panning */
    int		    balloon_delay;	/* delay (ms) before balloon pops up on */
    char	   *boldFont;
    char	   *browser;		/* browser for viewing html docs */
    int		    but_per_row;	/* number of buttons wide for the mode panel */
    char	   *buttonFont;
    char	   *canvasbackground;
    char	   *canvasforeground;
    Boolean	    DEBUG;
    Boolean	    dontswitchcmap;	/* don't allow switching of colormap */
    Boolean	    installowncmap;	/* install our own private colormap */
    Boolean	    drawzerolines;	/* draw lines through 0,0 (useful w/allow_neg_coords) */
    char	   *exportLanguage;
    Boolean	    flushleft;		/* center/flush-left printing */
    char	   *geometry;
    char	   *iconGeometry;
    char	   *image_editor;	/* image editor (xv, etc) */
    Boolean	    INCHES;
    int		    internalborderwidth;
    int		    jpeg_quality;	/* jpeg image quality */
    char	   *keyFile;
    Boolean	    landscape;
    Boolean	    latexfonts;
    char	   *library_dir;	/* for object library path */
    float	    magnification;	/* export/print magnification */
    int		    max_image_colors;	/* max colors to use for GIF/XPM images */
    Boolean	    monochrome;
    Boolean	    multiple;		/* multiple/single page for export/print */
    char	   *normalFont;
    char	   *pageborder;		/* color of page border */
    char	   *paper_size;		/* ASCII size of paper (from command-line) */
    int		    papersize;		/* size of paper */
    Boolean	    RHS_PANEL;
    char	   *pdf_viewer;		/* viewer for pdf docs */
    int		    rulerthick;		/* thickness of rulers */
    Boolean	    scalablefonts;	/* hns 5 Nov 91 */
    Boolean	    ShowAllButtons;
    Boolean	    showballoons;	/* show popup messages when user passes over buttons */
    Boolean	    showlengths;	/* length/width lines when drawing or moving */
    Boolean	    shownums;		/* print point numbers above polyline points */
    Boolean	    show_pageborder;	/* show page border in color on canvas */
    Boolean	    specialtext;
    char	   *spellcheckcommand;	/* spell check command e.g. 
					   "spell %s" or  "ispell -l < %s | sort -u" */
    int		    spinner_delay;	/* delay (ms) before spinner counts automatically */
    int		    spinner_rate;	/* rate (ms) at which spinner counts */
    int		    startfillstyle;	/* starting fill style */
    float	    startfontsize;	/* ges 6 Feb 91 */
    int		    startgridmode;	/* starting grid mode */
    char	   *startlatexFont;	/* bab 11 Jan 92 */
    int		    startlinewidth;	/* starting line width */
    int		    startposnmode;	/* starting point position mode */
    char	   *startpsFont;	/* bab 11 Jan 92 */
    float	    starttextstep;	/* starting multi-line text spacing */
    Boolean	    tablet;		/* input tablet extension */
    float	    tmp_height;
    float	    tmp_width;
    Boolean	    tracking;		/* mouse tracking in rulers */
    int		    transparent;	/* transparent color for GIF export
						(-2=none, -1=background) */
    float	    userscale;		/* scale screen units to user units */
    char	   *userunit;		/* user defined unit name */
    float	    zoom;		/* starting zoom scale */
    char	   *version;		/* version of the app-defaults file (compared with
					   the version/patchlevel of xfig when starting */
    int		    export_margin;	/* size of border around figure for export */
    Boolean	    flipvisualhints;	/* switch left/right mouse indicator messages */
    Boolean	    rigidtext;
    Boolean	    hiddentext;
    Boolean	    showdepthmanager;	/* whether or not to display the depth manager */
    char	   *grid_color;		/* color of grid (when on) */
    int		    smooth_factor;	/* smoothing factor when export to bitmap formats */
    Boolean	    icon_view;		/* icon or list view of library objects */
    int		    library_icon_size;	/* size of those icons */
    Boolean	    splash;		/* whether or not to show the splash screen on startup */
    char	   *zerolines;		/* color of zero-crossing lines on canvas */
    int		    freehand_resolution; /* minimum spacing of points when drawing freehand */
    char	   *tgrid_unit;		/* units of grid/point positioning (1/10" or 1/16") */
    Boolean	    overlap;		/* overlap/no-overlap multiple pages for export/print */
    char	   *ghostscript;	/* name of ghostscript (e.g. gs or gswin32) */
    Boolean	    correct_font_size;	/* adjust for difference in Fig screen res vs points (80/72) */
    int		    encoding;		/* encoding for latex escape translation */
#ifdef I18N
    Boolean international;
    String font_menu_language;
    Boolean euc_encoding;
    Boolean latin_keyboard;
    Boolean always_use_fontset;
    XFontSet fixed_fontset;
    XFontSet normal_fontset;
    XFontSet bold_fontset;
    int fontset_size;
    String xim_input_style;
    String fig2dev_localize_option;
#ifdef I18N_USE_PREEDIT
    String text_preedit;
#endif
#endif  /* I18N */

}		appresStruct, *appresPtr;
extern appresStruct appres;

typedef struct {
    int		    length, ascent, descent;
}		pr_size;

typedef struct {
    unsigned int    r_width, r_height, r_left, r_top;
}		RectRec;

typedef struct {
    int		    type;
    char	   *label;
    caddr_t	    info;
}		MenuItemRec;

struct Menu {
    int		    m_imagetype;
#define MENU_IMAGESTRING	0x00	/* imagedata is char * */
#define MENU_GRAPHIC		0x01	/* imagedata is pixrect * */
    caddr_t	    m_imagedata;
    int		    m_itemcount;
    MenuItemRec	   *m_items;
    struct Menu	   *m_next;
    caddr_t	    m_data;
};

/* def for paper size list */
struct paper_def {
    char  *sname;		/* short name e.g. 'A' */
    char  *fname;		/* full name e.g. 'A     (8.5" x 11")' */
    int	   width,height;	/* size in Fig units e.g. 10200 13200 */
};

typedef struct Menu MenuRec;

typedef XImage	*PIXRECT;
typedef pr_size	PR_SIZE;
typedef RectRec	RECT;

extern float	ZOOM_FACTOR;
extern float	PIC_FACTOR;

extern Window	main_canvas;		/* main canvas window */
extern Window	canvas_win;		/* current canvas */
extern Window	msg_win, sideruler_win, topruler_win;

extern Cursor	cur_cursor;
extern Cursor	arrow_cursor, bull_cursor, buster_cursor, crosshair_cursor,
		null_cursor, text_cursor, pick15_cursor, pick9_cursor,
		panel_cursor, l_arrow_cursor, lr_arrow_cursor, r_arrow_cursor,
		u_arrow_cursor, ud_arrow_cursor, d_arrow_cursor, wait_cursor,
		magnify_cursor;

extern Widget	tool;
extern XtAppContext tool_app;

extern Widget	canvas_sw, ps_fontmenu,		/* printer font menu tool */
		latex_fontmenu, 		/* printer font menu tool */
		msg_form, msg_panel, name_panel, cmd_form, mode_panel, 
		d_label, e_label, mousefun,
		ind_panel, ind_box, upd_ctrl,	/* indicator panel */
		unitbox_sw, sideruler_sw, topruler_sw;

extern Display *tool_d;
extern Screen  *tool_s;
extern Window	tool_w;
extern Widget   tool_form;
extern int	tool_sn;
extern int	tool_vclass;
extern Visual  *tool_v;
extern int	tool_dpth;
extern int	tool_cells;
extern int	image_bpp;		/* # of bytes-per-pixel for images at this visual */
extern int	screen_wd, screen_ht;	/* width and height of screen */
extern Colormap	tool_cm, newcmap;
extern Boolean	swapped_cmap;
extern Atom	wm_delete_window;
extern int	num_recent_files;	/* number of recent files in list */
extern int	max_recent_files;	/* user max number of recent files */
extern int	splash_onscreen;	/* flag used to clear off splash graphic */

extern GC	gc, button_gc, ind_button_gc, mouse_button_gc,
		fill_color_gc, pen_color_gc, blank_gc, ind_blank_gc, 
		mouse_blank_gc, gccache[NUMOPS], grid_gc,
		fillgc, fill_gc[NUMFILLPATS],	/* fill style gc's */
		tr_gc, tr_xor_gc, tr_erase_gc,	/* for the rulers */
		sr_gc, sr_xor_gc, sr_erase_gc;

extern Color	grid_color;
extern Pixmap	fill_pm[NUMFILLPATS],fill_but_pm[NUMPATTERNS];
extern float	fill_pm_zoom[NUMFILLPATS],fill_but_pm_zoom[NUMFILLPATS];
extern XColor	x_fg_color, x_bg_color;
extern unsigned long but_fg, but_bg;
extern unsigned long ind_but_fg, ind_but_bg;
extern unsigned long mouse_but_fg, mouse_but_bg;

/* will contain environment variable XFIGTMPDIR, if any */
extern char    *TMPDIR;

/* will contain environment variable FIG2DEV_DIR, if any */
extern char    *fig2dev_path;
extern char     fig2dev_cmd[PATH_MAX];

extern String  text_translations;

/* for w_export.c and w_print.c */

extern char    *orient_items[2];
extern char    *just_items[2];
extern struct   paper_def paper_sizes[NUMPAPERSIZES];
extern char    *multiple_pages[2], *overlap_pages[2];

/* for w_file.c and w_export.c */

extern char    *offset_unit_items[3];
#endif /* RESOURCES_H */

extern int	RULER_WD;

/* environment variable name definition for image editor used for screen capture */

#define XFIG_ENV_GIF_EDITOR    getenv("XFIG_GIF_EDITOR")

/* flag for when picture object is read in merge_file to see if need to remap
   existing picture colors */

extern Boolean	pic_obj_read;

typedef struct _recent_file_struct {
	char	*name;
	Widget	menu;
} _recent_files;

extern _recent_files recent_files[];

