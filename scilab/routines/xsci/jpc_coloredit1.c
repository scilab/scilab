/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1991 by Paul King
 * Parts Copyright (c) 1994 by Brian V. Smith
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so, with the only requirement being
 * that this copyright notice remain intact.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 */

#include "fig.h"
#include "figx.h"
#include "resources.h"
#include "object.h"
#include "mode.h"
#include "paintop.h"
#include "u_fonts.h"e
#include "w_drawprim.h"
#include "w_icons.h"
#include "w_indpanel.h"
#include "w_mousefun.h"
#include "w_setup.h"
#include "w_util.h"
#include "w_zoom.h"

#define MAX_SCROLL_WD 50
extern Pixmap	psfont_menu_bitmaps[], latexfont_menu_bitmaps[];
extern Atom	wm_delete_window;
extern struct	_fstruct ps_fontinfo[], latex_fontinfo[];
extern char    *panel_get_value();
extern int	show_zoom();
extern int	show_depth();
extern int	cur_updatemask;
extern Widget	make_popup_menu();
extern ind_sw_info *pen_color_button, *fill_color_button;

/**************	    local variables and routines   **************/

       float	display_zoomscale=1.0;

static int	cur_anglegeom = L_UNCONSTRAINED;
static int	cur_indmask = I_MIN1;
static int	cur_flagshown = 0;
#define MAX_FLAGS 2 /* maximum value for cur_flagshown */

/***** default text widget actions and translations *****/
static String indpanel_text_translations =
	"<Key>Return: no-op(RingBell)\n\
	Ctrl<Key>J: no-op(RingBell)\n\
	Ctrl<Key>M: no-op(RingBell)\n\
	Ctrl<Key>X: EmptyTextKey()\n\
	Ctrl<Key>U: multiply(4)\n\
	<Key>F18: PastePanelKey()\n";

/***** value panel set actions and translations *****/
static String set_translations =
	"<Key>Return: ClearMousefunKbd()SetValue()\n\
	Ctrl<Key>J: no-op(RingBell)\n\
	Ctrl<Key>M: no-op(RingBell)\n\
	Ctrl<Key>X: EmptyTextKey()\n\
	Ctrl<Key>U: multiply(4)\n\
	<Key>F18: PastePanelKey()\n";

static void	nval_panel_set();
static XtActionsRec set_actions[] =
{
    {"SetValue", (XtActionProc) nval_panel_set},
};

/***** value panel cancel actions and translations *****/
static String   nval_translations =
        "<Message>WM_PROTOCOLS: QuitNval()\n";
static void	nval_panel_cancel();
static XtActionsRec     nval_actions[] =
{
    {"QuitNval", (XtActionProc) nval_panel_cancel},
};

/***** choice panel cancel actions and translations *****/
static String   choice_translations =
        "<Message>WM_PROTOCOLS: QuitChoice()\n";
static void     choice_panel_cancel();
static XtActionsRec     choice_actions[] =
{
    {"QuitChoice", (XtActionProc) choice_panel_cancel},
};

DeclareStaticArgs(15);

/* declarations for value buttons */

/* these are in w_color.c */
extern int	show_pen_color(), next_pen_color(), prev_pen_color();
extern int	show_fill_color(), next_fill_color(), prev_fill_color();

/* these need to be referenced outside of this file */
       int	inc_zoom(), dec_zoom();
       int	show_fill_style();

/* and these can be static */

/* declarations for choice buttons */
static int	darken_fill(), lighten_fill();
static int	inc_choice(), dec_choice();
static int	show_valign(), show_halign(), show_textjust();
static int	show_arrowmode(), show_arrowtype();
static int	show_linestyle(), show_joinstyle(), show_capstyle();
static int	show_anglegeom(), show_arctype();
static int	show_pointposn(), show_gridmode(), show_linkmode();
static int	show_linewidth(), inc_linewidth(), dec_linewidth();
static int	show_boxradius(), inc_boxradius(), dec_boxradius();
static int	show_font(), inc_font(), dec_font();
static int	show_flags(), inc_flags(), dec_flags();
static int	show_fontsize(), inc_fontsize(), dec_fontsize();
static int	show_textstep(), inc_textstep(), dec_textstep();
static int	show_rotnangle(), inc_rotnangle(), dec_rotnangle();
static int	show_elltextangle(), inc_elltextangle(), dec_elltextangle();
static int	show_numsides(), inc_numsides(), dec_numsides();
static int	show_numcopies(), inc_numcopies(), dec_numcopies();
static int	show_numxcopies(), inc_numxcopies(), dec_numxcopies();
static int	show_numycopies(), inc_numycopies(), dec_numycopies();
static int	inc_depth(), dec_depth();

static int	popup_fonts();
static void	note_state();
static void	set_all_update(), clr_all_update(), tog_all_update();

static char	indbuf[12];
static float	old_display_zoomscale = -1.0;
static int	old_rotnangle = -1;
static float	old_elltextangle = -1.0;

Dimension	UPD_CTRL_WD = 0;

Widget		ind_box, set_upd, upd_tog,
		clr_upd, tog_upd, upd_ctrl_lab, upd_ctrl_btns;

#define		DEF_IND_SW_HT		32
#define		DEF_IND_SW_WD		64
#define		FONT_IND_SW_WD		(40+PS_FONTPANE_WD)
#define		NARROW_IND_SW_WD	56
#define		WIDE_IND_SW_WD		76
#define		XWIDE_IND_SW_WD		86

/* indicator switch definitions */

static choice_info anglegeom_choices[] = {
    {L_UNCONSTRAINED, &unconstrained_ic,},
    {L_LATEXLINE, &latexline_ic,},
    {L_LATEXARROW, &latexarrow_ic,},
    {L_MOUNTHATTAN, &mounthattan_ic,},
    {L_MANHATTAN, &manhattan_ic,},
    {L_MOUNTAIN, &mountain_ic,},
};
#define NUM_ANGLEGEOM_CHOICES (sizeof(anglegeom_choices)/sizeof(choice_info))

static choice_info valign_choices[] = {
    {ALIGN_NONE, &none_ic,},
    {ALIGN_TOP, &valignt_ic,},
    {ALIGN_CENTER, &valignc_ic,},
    {ALIGN_BOTTOM, &valignb_ic,},
    {ALIGN_DISTRIB_C, &valigndc_ic,},
    {ALIGN_DISTRIB_E, &valignde_ic,},
    {ALIGN_ABUT, &valigna_ic,},
};
#define NUM_VALIGN_CHOICES (sizeof(valign_choices)/sizeof(choice_info))

static choice_info halign_choices[] = {
    {ALIGN_NONE, &none_ic,},
    {ALIGN_LEFT, &halignl_ic,},
    {ALIGN_CENTER, &halignc_ic,},
    {ALIGN_RIGHT, &halignr_ic,},
    {ALIGN_DISTRIB_C, &haligndc_ic,},
    {ALIGN_DISTRIB_E, &halignde_ic,},
    {ALIGN_ABUT, &haligna_ic,},
};
#define NUM_HALIGN_CHOICES (sizeof(halign_choices)/sizeof(choice_info))

static choice_info gridmode_choices[] = {
    {GRID_0, &none_ic,},
    {GRID_1, &grid1_ic,},
    {GRID_2, &grid2_ic,},
    {GRID_3, &grid3_ic,},
};
#define NUM_GRIDMODE_CHOICES (sizeof(gridmode_choices)/sizeof(choice_info))

static choice_info pointposn_choices[] = {
    {P_ANY, &any_ic,},
    {P_MAGNET, &fine_grid_ic,},
    {P_GRID1, &grid1_ic,},
    {P_GRID2, &grid2_ic,},
};
#define NUM_POINTPOSN_CHOICES (sizeof(pointposn_choices)/sizeof(choice_info))

static choice_info arrowmode_choices[] = {
    {L_NOARROWS, &noarrows_ic,},
    {L_FARROWS, &farrows_ic,},
    {L_FBARROWS, &fbarrows_ic,},
    {L_BARROWS, &barrows_ic,},
};
#define NUM_ARROWMODE_CHOICES (sizeof(arrowmode_choices)/sizeof(choice_info))

/* e_edit.c uses these also */

choice_info arrowtype_choices[NUM_ARROW_TYPES] = {
    {0, &arrow0_ic,},
    {1, &arrow1_ic,},
    {2, &arrow2_ic,},
    {3, &arrow3_ic,},
    {4, &arrow4_ic,},
    {5, &arrow5_ic,},
    {6, &arrow6_ic,},
};
#define NUM_ARROWTYPE_CHOICES (sizeof(arrowtype_choices)/sizeof(choice_info))

static choice_info textjust_choices[] = {
    {T_LEFT_JUSTIFIED, &textL_ic,},
    {T_CENTER_JUSTIFIED, &textC_ic,},
    {T_RIGHT_JUSTIFIED, &textR_ic,},
};
#define NUM_TEXTJUST_CHOICES (sizeof(textjust_choices)/sizeof(choice_info))

static choice_info arctype_choices[] = {
    {T_OPEN_ARC, &open_arc_ic,},
    {T_PIE_WEDGE_ARC, &pie_wedge_arc_ic,},
};
#define NUM_ARCTYPE_CHOICES (sizeof(arctype_choices)/sizeof(choice_info))

static choice_info linestyle_choices[] = {
    {SOLID_LINE, &solidline_ic,},
    {DASH_LINE, &dashline_ic,},
    {DOTTED_LINE, &dottedline_ic,},
};
#define NUM_LINESTYLE_CHOICES (sizeof(linestyle_choices)/sizeof(choice_info))

static choice_info joinstyle_choices[] = {
    {JOIN_MITER, &joinmiter_ic,},
    {JOIN_ROUND, &joinround_ic,},
    {JOIN_BEVEL, &joinbevel_ic,},
};
#define NUM_JOINSTYLE_CHOICES (sizeof(joinstyle_choices)/sizeof(choice_info))

static choice_info capstyle_choices[] = {
    {CAP_BUTT,    &capbutt_ic,},
    {CAP_ROUND,   &capround_ic,},
    {CAP_PROJECT, &capproject_ic,},
};
#define NUM_CAPSTYLE_CHOICES (sizeof(capstyle_choices)/sizeof(choice_info))

static choice_info linkmode_choices[] = {
    {SMART_OFF, &smartoff_ic,},
    {SMART_MOVE, &smartmove_ic,},
    {SMART_SLIDE, &smartslide_ic,},
};
#define NUM_LINKMODE_CHOICES (sizeof(linkmode_choices)/sizeof(choice_info))

choice_info	fillstyle_choices[NUMFILLPATS + 1];

choice_info	color_choices[NUM_STD_COLS + 1];
ind_sw_info	*zoom_sw;
ind_sw_info	*fill_style_sw;

#define I_CHOICE	0
#define I_IVAL		1
#define I_FVAL		2

#define		inc_action(z)	(z->inc_func)(z)
#define		dec_action(z)	(z->dec_func)(z)
#define		show_action(z)	(z->show_func)(z)

ind_sw_info	ind_switches[] = {
    {I_FVAL, I_ZOOM, "Zoom", "Scale", NARROW_IND_SW_WD,
	NULL, &display_zoomscale, inc_zoom, dec_zoom, show_zoom,},
    {I_CHOICE, I_GRIDMODE, "Grid", "Mode", DEF_IND_SW_WD,
	&cur_gridmode, NULL, inc_choice, dec_choice, show_gridmode,
	gridmode_choices, NUM_GRIDMODE_CHOICES, NUM_GRIDMODE_CHOICES,},
    {I_CHOICE, I_POINTPOSN, "Point", "Posn", DEF_IND_SW_WD,
	&cur_pointposn, NULL, inc_choice, dec_choice, show_pointposn,
	pointposn_choices, NUM_POINTPOSN_CHOICES, NUM_POINTPOSN_CHOICES,},
    {I_IVAL, I_DEPTH, "Depth", "", NARROW_IND_SW_WD,
	&cur_depth, NULL, inc_depth, dec_depth, show_depth,},
    {I_IVAL, I_ROTNANGLE, "Rotn", "Angle", NARROW_IND_SW_WD,
	&cur_rotnangle, NULL, inc_rotnangle, dec_rotnangle, show_rotnangle,},
    {I_IVAL, I_NUMSIDES, "Num", "Sides", NARROW_IND_SW_WD,
	&cur_numsides, NULL, inc_numsides, dec_numsides, show_numsides,},
    {I_IVAL, I_NUMCOPIES, "Num", "Copies", NARROW_IND_SW_WD,
	&cur_numcopies, NULL, inc_numcopies, dec_numcopies, show_numcopies,},
    {I_IVAL, I_NUMXCOPIES, "Num X", "Copies", NARROW_IND_SW_WD,
	&cur_numxcopies, NULL, inc_numxcopies, dec_numxcopies, show_numxcopies,},
    {I_IVAL, I_NUMYCOPIES, "Num Y", "Copies", NARROW_IND_SW_WD,
	&cur_numycopies, NULL, inc_numycopies, dec_numycopies, show_numycopies,},
    {I_CHOICE, I_VALIGN, "Vert", "Align", DEF_IND_SW_WD,
	&cur_valign, NULL, inc_choice, dec_choice, show_valign,
	valign_choices, NUM_VALIGN_CHOICES, NUM_VALIGN_CHOICES,},
    {I_CHOICE, I_HALIGN, "Horiz", "Align", DEF_IND_SW_WD,
	&cur_halign, NULL, inc_choice, dec_choice, show_halign,
	halign_choices, NUM_HALIGN_CHOICES, NUM_HALIGN_CHOICES,},
    {I_CHOICE, I_ANGLEGEOM, "Angle", "Geom", DEF_IND_SW_WD,
	&cur_anglegeom, NULL, inc_choice, dec_choice, show_anglegeom,
	anglegeom_choices, NUM_ANGLEGEOM_CHOICES, NUM_ANGLEGEOM_CHOICES / 2,},
    {I_CHOICE, I_PEN_COLOR, "PenColor", "", XWIDE_IND_SW_WD,
	(int *) &cur_pencolor, NULL, next_pen_color, prev_pen_color, show_pen_color,
	color_choices, NUM_STD_COLS + 1, 7},
    {I_CHOICE, I_FILL_COLOR, "FillColor", "", XWIDE_IND_SW_WD,
	(int *) &cur_fillcolor, NULL, next_fill_color, prev_fill_color, show_fill_color,
	color_choices, NUM_STD_COLS + 1, 7},
    {I_CHOICE, I_FILLSTYLE, "Fill", "Style", DEF_IND_SW_WD,
	&cur_fillstyle, NULL, darken_fill, lighten_fill, show_fill_style,
	fillstyle_choices, NUMFILLPATS + 1, 11},
    {I_CHOICE, I_ARCTYPE, "Arc", "Type", DEF_IND_SW_WD,
	&cur_arctype, NULL, inc_choice, dec_choice, show_arctype,
	arctype_choices, NUM_ARCTYPE_CHOICES, NUM_ARCTYPE_CHOICES},
    {I_CHOICE, I_LINKMODE, "Smart", "Links", DEF_IND_SW_WD,
	&cur_linkmode, NULL, inc_choice, dec_choice, show_linkmode,
	linkmode_choices, NUM_LINKMODE_CHOICES, NUM_LINKMODE_CHOICES},
    {I_IVAL, I_LINEWIDTH, "Line", "Width", NARROW_IND_SW_WD,
	&cur_linewidth, NULL, inc_linewidth, dec_linewidth, show_linewidth,},
    {I_CHOICE, I_LINESTYLE, "Line", "Style", DEF_IND_SW_WD,
	&cur_linestyle, NULL, inc_choice, dec_choice, show_linestyle,
	linestyle_choices, NUM_LINESTYLE_CHOICES, NUM_LINESTYLE_CHOICES,},
    {I_CHOICE, I_JOINSTYLE, "Join", "Style", DEF_IND_SW_WD,
	&cur_joinstyle, NULL, inc_choice, dec_choice, show_joinstyle,
	joinstyle_choices, NUM_JOINSTYLE_CHOICES, NUM_JOINSTYLE_CHOICES,},
    {I_CHOICE, I_CAPSTYLE, "Cap", "Style", DEF_IND_SW_WD,
	&cur_capstyle, NULL, inc_choice, dec_choice, show_capstyle,
	capstyle_choices, NUM_CAPSTYLE_CHOICES, NUM_CAPSTYLE_CHOICES,},
    {I_CHOICE, I_ARROWMODE, "Arrow", "Mode", DEF_IND_SW_WD,
	&cur_arrowmode, NULL, inc_choice, dec_choice, show_arrowmode,
	arrowmode_choices, NUM_ARROWMODE_CHOICES, NUM_ARROWMODE_CHOICES,},
    {I_CHOICE, I_ARROWTYPE, "Arrow", "Type", DEF_IND_SW_WD,
	&cur_arrowtype, NULL, inc_choice, dec_choice, show_arrowtype,
	arrowtype_choices, NUM_ARROWTYPE_CHOICES, NUM_ARROWTYPE_CHOICES,},
    {I_IVAL, I_BOXRADIUS, "Box", "Curve", DEF_IND_SW_WD,
	&cur_boxradius, NULL, inc_boxradius, dec_boxradius, show_boxradius,},
    {I_CHOICE, I_TEXTJUST, "Text", "Just", DEF_IND_SW_WD,
	&cur_textjust, NULL, inc_choice, dec_choice, show_textjust,
	textjust_choices, NUM_TEXTJUST_CHOICES, NUM_TEXTJUST_CHOICES,},
    {I_FVAL, I_ELLTEXTANGLE, "Text/Ellipse", "Angle", XWIDE_IND_SW_WD,
	NULL, &cur_elltextangle, inc_elltextangle, dec_elltextangle,
	show_elltextangle,},
    {I_IVAL, I_TEXTFLAGS, "Text Flags", "", WIDE_IND_SW_WD,
	&cur_fontsize, NULL, inc_flags, dec_flags, show_flags,},
    {I_IVAL, I_FONTSIZE, "Text", "Size", NARROW_IND_SW_WD,
	&cur_fontsize, NULL, inc_fontsize, dec_fontsize, show_fontsize,},
    {I_FVAL, I_TEXTSTEP, "Text", "Step", NARROW_IND_SW_WD,
	NULL, &cur_textstep, inc_textstep, dec_textstep, show_textstep,},
    {I_IVAL, I_FONT, "Text", "Font", FONT_IND_SW_WD,
	&cur_ps_font, NULL, inc_font, dec_font, show_font,},
};

#define		NUM_IND_SW	(sizeof(ind_switches) / sizeof(ind_sw_info))


/* button selection event handler */
static void	sel_ind_but();

/* arguments for the update indicator boxes in the indicator buttons */

static Arg	upd_args[] =
{
    /* 0 */ {XtNwidth, (XtArgVal) 8},
    /* 1 */ {XtNheight, (XtArgVal) 8},
    /* 2 */ {XtNborderWidth, (XtArgVal) 1},
    /* 3 */ {XtNtop, (XtArgVal) XtChainTop},
    /* 4 */ {XtNright, (XtArgVal) XtChainRight},
    /* 5 */ {XtNstate, (XtArgVal) True},
    /* 6 */ {XtNvertDistance, (XtArgVal) 0},
    /* 7 */ {XtNhorizDistance, (XtArgVal) 0},
    /* 8 */ {XtNlabel, (XtArgVal) " "},
    /* 9 */ {XtNhighlightThickness, (XtArgVal) 0},
};

static XtActionsRec ind_actions[] =
{
    {"EnterIndSw", (XtActionProc) draw_mousefun_ind},
    {"LeaveIndSw", (XtActionProc) clear_mousefun},
    {"ZoomIn", (XtActionProc) inc_zoom},
    {"ZoomOut", (XtActionProc) dec_zoom},
};

static String	ind_translations =
"<EnterWindow>:EnterIndSw()highlight()\n\
    <LeaveWindow>:LeaveIndSw()unhighlight()\n";

/* bitmaps for set/clear and toggle buttons (10x10) */
static unsigned char set_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static unsigned char clr_bits[] = {
   0xff, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02,
   0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03};
static unsigned char tog_bits[] = {
   0xff, 0x03, 0x01, 0x02, 0x03, 0x02, 0x07, 0x02, 0x0f, 0x02, 0x1f, 0x02,
   0x3f, 0x02, 0x7f, 0x02, 0xff, 0x02, 0xff, 0x03};

init_ind_panel(tool)
    Widget	    tool;
{
    ind_sw_info	*sw;
    Widget	 tw; /* temporary widget to get scrollbar widget */
    int		 i;

    /* Make a widget which contains the label and toggle/set/clear form.
       This will be managed and unmanaged as needed.  It's parent is the 
       main form (tool) and the ind_panel will be chained to it when it
       is managed */

    FirstArg(XtNdefaultDistance, 0);
    NextArg(XtNborderWidth, INTERNAL_BW);
    NextArg(XtNorientation, XtorientVertical);
    NextArg(XtNhSpace, 0);
    NextArg(XtNvSpace, 1);
    NextArg(XtNresizable, False);
    NextArg(XtNfromVert, canvas_sw);
    NextArg(XtNvertDistance, -INTERNAL_BW);
    NextArg(XtNtop, XtChainBottom);	/* don't resize when form changes */
    NextArg(XtNbottom, XtChainBottom);
    NextArg(XtNleft, XtChainLeft);
    NextArg(XtNright, XtChainLeft);

    upd_ctrl = XtCreateWidget("upd_ctrl_form", boxWidgetClass,
			tool, Args, ArgCount);

    /* make a widget which contains the buttons to toggle/set/clear
       the update buttons in the indicator panel buttons */

    FirstArg(XtNborderWidth, 0);
    NextArg(XtNjustify, XtJustifyCenter);
    NextArg(XtNfont, button_font);
    NextArg(XtNlabel, " Update\n Control");
    upd_ctrl_lab = XtCreateManagedWidget("upd_ctrl_label", labelWidgetClass,
			upd_ctrl, Args, ArgCount);

    /* make a widget which contains the buttons to toggle/set/clear
       the update buttons in the indicator panel buttons */

    FirstArg(XtNdefaultDistance, 0);
    NextArg(XtNborderWidth, 0);
    NextArg(XtNorientation, XtorientHorizontal);
    NextArg(XtNhSpace, 3);
    NextArg(XtNvSpace, 0);
    upd_ctrl_btns = XtCreateManagedWidget("upd_ctrl_btns", boxWidgetClass,
			upd_ctrl, Args, ArgCount);

    FirstArg(XtNheight, UPD_BITS);
    NextArg(XtNwidth, UPD_BITS);
    NextArg(XtNinternalWidth, UPD_INT);
    NextArg(XtNinternalHeight, UPD_INT);
    NextArg(XtNborderWidth, UPD_BORD);
    set_upd = XtCreateManagedWidget("set_upd", commandWidgetClass,
			upd_ctrl_btns, Args, ArgCount);
    XtAddEventHandler(set_upd, ButtonReleaseMask, (Boolean) 0,
			set_all_update, (XtPointer) 0);
    clr_upd = XtCreateManagedWidget("clr_upd", commandWidgetClass,
			upd_ctrl_btns, Args, ArgCount);
    XtAddEventHandler(clr_upd, ButtonReleaseMask, (Boolean) 0,
			clr_all_update, (XtPointer) 0);
    tog_upd = XtCreateManagedWidget("tog_upd", commandWidgetClass,
			upd_ctrl_btns, Args, ArgCount);
    XtAddEventHandler(tog_upd, ButtonReleaseMask, (Boolean) 0,
			tog_all_update, (XtPointer) 0);

    /* start with all components affected by update */
    cur_updatemask = I_UPDATEMASK;


    /* make a scrollable viewport in case all the buttons don't fit */
    /* resize this later when we know how high the scrollbar is */
    /* When the update control is managed, make the fromHoriz that widget */

    FirstArg(XtNallowHoriz, True);
    NextArg(XtNwidth, INDPANEL_WD);
    /* does he want to always see ALL of the indicator buttons? */
    if (appres.ShowAllButtons) {	/* yes, but set cur_indmask to all later */
	i = 2*DEF_IND_SW_HT+5*INTERNAL_BW; /* two rows high when showing all buttons */
    } else {
	i = DEF_IND_SW_HT + 2*INTERNAL_BW;   
    }
    /* account for the scrollbar thickness */
    i += MAX_SCROLL_WD;
    /* and force it to be created so we can see how thick it is */
    NextArg(XtNforceBars, True);

    NextArg(XtNheight, i);
    NextArg(XtNborderWidth, 0);
    NextArg(XtNuseBottom, True);
    NextArg(XtNfromVert, canvas_sw);
    NextArg(XtNvertDistance, -INTERNAL_BW);
    NextArg(XtNtop, XtChainBottom);	/* don't resize height when form changes */
    NextArg(XtNbottom, XtChainBottom);
    NextArg(XtNleft, XtChainLeft);	/* but do resize width with form widens */
    NextArg(XtNright, XtChainRight);

    ind_panel = XtCreateManagedWidget("ind_panel", viewportWidgetClass, tool,
			Args, ArgCount);

    FirstArg(XtNheight, i);
    NextArg(XtNhSpace, 0);
    NextArg(XtNvSpace, 0);
    NextArg(XtNresizable, True);
    NextArg(XtNborderWidth, 0);
    if (appres.ShowAllButtons) {
	NextArg(XtNorientation, XtorientVertical);	/* use two rows */
    } else {
	NextArg(XtNorientation, XtorientHorizontal);	/* expand horizontally */
    }

    ind_box = XtCreateManagedWidget("ind_box", boxWidgetClass, ind_panel,
			       Args, ArgCount);

    XtAppAddActions(tool_app, ind_actions, XtNumber(ind_actions));

    for (i = 0; i < NUM_IND_SW; ++i) {
	sw = &ind_switches[i];
	sw->panel = (Widget) NULL;	/* not created yet */

	FirstArg(XtNwidth, sw->sw_width);
	NextArg(XtNheight, DEF_IND_SW_HT);
	NextArg(XtNdefaultDistance, 0);
	NextArg(XtNborderWidth, INTERNAL_BW);
	sw->formw = XtCreateWidget("button_form", formWidgetClass,
			     ind_box, Args, ArgCount);

	/* make an update button in the upper-right corner of the main button */
	if (sw->func & I_UPDATEMASK)
	    {
	    upd_args[7].value = sw->sw_width
					- upd_args[0].value
					- 2*upd_args[2].value;
	    sw->updbut = XtCreateWidget("update", toggleWidgetClass,
			     sw->formw, upd_args, XtNumber(upd_args));
	    sw->update = True;
	    XtAddEventHandler(sw->updbut, ButtonReleaseMask, (Boolean) 0,
			     note_state, (XtPointer) sw);
	    }

	/* now create the command button */
	FirstArg(XtNlabel, " ");
	NextArg(XtNwidth, sw->sw_width);
	NextArg(XtNheight, DEF_IND_SW_HT);
	NextArg(XtNresizable, False);
	NextArg(XtNborderWidth, 0);
	NextArg(XtNresize, False);
	NextArg(XtNbackgroundPixmap, NULL);
	sw->button = XtCreateManagedWidget("button", commandWidgetClass,
			     sw->formw, Args, ArgCount);
	/* map this button if it is needed */
	if (sw->func & cur_indmask)
	    XtManageChild(sw->formw);

	/* allow left & right buttons */
	/* (callbacks pass same data for ANY button) */
	XtAddEventHandler(sw->button, ButtonReleaseMask, (Boolean) 0,
			  sel_ind_but, (XtPointer) sw);
	XtOverrideTranslations(sw->button,
			       XtParseTranslationTable(ind_translations));
    }

    /* now get the real height of the scrollbar and resize the ind_panel if necessary */
    tw = XtNameToWidget(ind_panel, "horizontal");
    if (tw != NULL) {
	Dimension    td1; /* temporary variable to get scrollbar thickness */
	Dimension    td2; /* temporary variable to get indpanel height */
	Dimension    bdw; /* temporary variable to get scrollbar border width */

	FirstArg(XtNthumb, None);	/* make solid scrollbar instead of tiled */
	SetValues(tw);

	FirstArg(XtNthickness, &td1);
	NextArg(XtNborderWidth, &bdw);
	GetValues(tw);
	FirstArg(XtNheight, &td2);
	GetValues(ind_panel);
	/* make it no shorter than the control panel height */
	td2 = max2(td2 - MAX_SCROLL_WD + td1 + 4 + bdw, UPD_CTRL_HT);
	XtUnmanageChild(ind_panel);
	FirstArg(XtNheight, td2);
	SetValues(ind_box);
	SetValues(ind_panel);
	/* now turn off the scrollbar until we need it */
	FirstArg(XtNforceBars, False);
	SetValues(ind_panel);
	XtManageChild(ind_panel);
    }

    if (appres.ShowAllButtons) {
	cur_indmask = I_ALL;	/* now turn all indicator buttons */
	appres.ShowAllButtons = False;
	update_indpanel(cur_indmask);
	appres.ShowAllButtons = True;
    }
    update_indpanel(cur_indmask);
    /* add translation and action to set value on CR in nval popup */
    XtAppAddActions(tool_app, set_actions, XtNumber(set_actions));
    /* set up action and translation for mousefun kbd icon */
    init_kbd_actions();
}

static void
note_state(w, closure, ev, continue_to_dispatch)
    Widget	    w;
    XtPointer	    closure;
    XEvent         *ev;
    Boolean        *continue_to_dispatch;

{
    ind_sw_info *sw = (ind_sw_info *) closure;
    XButtonEvent *event = &ev->xbutton;

    if (event->button != Button1)
	return;

    /* toggle update status of this indicator */
    sw->update = !sw->update;
    if (sw->update)
	cur_updatemask |= sw->func;	/* turn on update status */
    else
	cur_updatemask &= ~sw->func;	/* turn off update status */
}

/* toggle the update buttons in all the widgets */
static void
tog_all_update()
{
    int i;

    cur_updatemask = ~cur_updatemask;	/* tuggle all */
    for (i = 0; i < NUM_IND_SW; ++i) {
	if (ind_switches[i].updbut == NULL)
		continue;
	ind_switches[i].update = !ind_switches[i].update;
	FirstArg(XtNstate, ind_switches[i].update);
	SetValues(ind_switches[i].updbut);
    }
    put_msg("Update command status TOGGLED for all buttons");
}

/* turn on the update buttons in all the widgets */
static void
set_all_update()
{
    int i;

    cur_updatemask = I_UPDATEMASK;	/* turn all on */
    for (i = 0; i < NUM_IND_SW; ++i) {
	if (ind_switches[i].updbut == NULL)
		continue;
	ind_switches[i].update = True;
	FirstArg(XtNstate, True);
	SetValues(ind_switches[i].updbut);
    }
    put_msg("Update commands are now ENABLED for all buttons");
}

/* turn off the update buttons in all the widgets */
static void
clr_all_update()
{
    int i;

    for (i = 0; i < NUM_IND_SW; ++i) {
    cur_updatemask = 0;			/* turn all off */
	if (ind_switches[i].updbut == NULL)
		continue;
	ind_switches[i].update = False;
	FirstArg(XtNstate, False);
	SetValues(ind_switches[i].updbut);
    }
    put_msg("Update commands will be IGNORED for all buttons");
}

manage_update_buts()
{
    int		    i;
    for (i = 0; i < NUM_IND_SW; ++i)
	if (ind_switches[i].func & I_UPDATEMASK)
	    XtManageChild(ind_switches[i].updbut);
}
		
unmanage_update_buts()
{
    int		    i;
    for (i = 0; i < NUM_IND_SW; ++i)
	if (ind_switches[i].func & I_UPDATEMASK)
	    XtUnmanageChild(ind_switches[i].updbut);
}
		
setup_ind_panel()
{
    int		    i;
    ind_sw_info	   *isw;
    Display	   *d = tool_d;
    Screen	   *s = tool_s;
    Pixmap	    p;
    Pixel	    fg,bg;

    /* get the foreground and background from the indicator widget */
    /* and create a gc with those values */
    ind_button_gc = XCreateGC(tool_d, XtWindow(ind_panel), (unsigned long) 0, NULL);
    FirstArg(XtNforeground, &ind_but_fg);
    NextArg(XtNbackground, &ind_but_bg);
    GetValues(ind_switches[0].button);
    XSetBackground(tool_d, ind_button_gc, ind_but_bg);
    XSetForeground(tool_d, ind_button_gc, ind_but_fg);
    XSetFont(tool_d, ind_button_gc, button_font->fid);

    /* also create gc with fore=background for blanking areas */
    ind_blank_gc = XCreateGC(tool_d, XtWindow(ind_panel), (unsigned long) 0, NULL);
    XSetBackground(tool_d, ind_blank_gc, ind_but_bg);
    XSetForeground(tool_d, ind_blank_gc, ind_but_bg);

    /* create a gc for the fill and border color 'palettes' */
    fill_color_gc = XCreateGC(tool_d, XtWindow(ind_panel), (unsigned long) 0, NULL);
    pen_color_gc = XCreateGC(tool_d, XtWindow(ind_panel), (unsigned long) 0, NULL);

    /* initialize the fill style gc and pixmaps */
    init_fill_pm();
    init_fill_gc();

    FirstArg(XtNbackgroundPixmap, fillstyle_choices[NUMFILLPATS-1].blackPM);
    SetValues(ind_panel);

    for (i = 0; i < NUM_IND_SW; ++i) {
	isw = &ind_switches[i];
	/* keep track of a few switches */
	if (ind_switches[i].func == I_FILLSTYLE)
	    fill_style_sw = isw;
	else if (ind_switches[i].func == I_ZOOM)
	    zoom_sw = isw;
	else if (ind_switches[i].func == I_PEN_COLOR)
	    pen_color_button = isw; /* to update its pixmap in the indicator panel */
	else if (ind_switches[i].func == I_FILL_COLOR)
	    fill_color_button = isw; /* to update its pixmap in the indicator panel */

	p = XCreatePixmap(d, XtWindow(isw->button), isw->sw_width,
			  DEF_IND_SW_HT, DefaultDepthOfScreen(s));
	XFillRectangle(d, p, ind_blank_gc, 0, 0,
		       isw->sw_width, DEF_IND_SW_HT);
	XDrawImageString(d, p, ind_button_gc, 3, 12, isw->line1, strlen(isw->line1));
	XDrawImageString(d, p, ind_button_gc, 3, 25, isw->line2, strlen(isw->line2));

	isw->normalPM = p;
	FirstArg(XtNbackgroundPixmap, p);
	SetValues(isw->button);
	/* generate pixmaps if this is a choice panel */
	if (ind_switches[i].type == I_CHOICE)
		generate_choice_pixmaps(isw);
    }

    /* setup the pixmap in the color button */
    show_pen_color();
    show_fill_color();

    /* now put cute little images in update buttons (full box (set), empty box (clear)
       and half full (toggle) */
    FirstArg(XtNforeground, &fg);
    NextArg(XtNbackground, &bg);
    for (i = 0; i < NUM_IND_SW; ++i)		/* find one of the update buttons */
	if (ind_switches[i].func & I_UPDATEMASK) { /* and get its fg and bg colors */
	    GetValues(ind_switches[i].updbut);
	    break;
	}

    p = XCreatePixmapFromBitmapData(tool_d, XtWindow(ind_panel),
		    (char *) set_bits, UPD_BITS, UPD_BITS, fg, bg,
		    DefaultDepthOfScreen(tool_s));
    FirstArg(XtNbitmap, p);
    SetValues(set_upd);
    p = XCreatePixmapFromBitmapData(tool_d, XtWindow(ind_panel),
		    (char *) clr_bits, UPD_BITS, UPD_BITS, fg, bg,
		    DefaultDepthOfScreen(tool_s));
    FirstArg(XtNbitmap, p);
    SetValues(clr_upd);
    p = XCreatePixmapFromBitmapData(tool_d, XtWindow(ind_panel),
		    (char *) tog_bits, UPD_BITS, UPD_BITS, fg, bg,
		    DefaultDepthOfScreen(tool_s));
    FirstArg(XtNbitmap, p);
    SetValues(tog_upd);

    XDefineCursor(d, XtWindow(ind_panel), arrow_cursor);
    update_current_settings();

    FirstArg(XtNmappedWhenManaged, True);
    SetValues(ind_panel);
}

generate_choice_pixmaps(isw)
    ind_sw_info	   *isw;
{
    int		    i;
    choice_info	   *tmp_choice;

    tmp_choice = isw->choices;
    for (i = 0; i < isw->numchoices; tmp_choice++, i++) {
	if (tmp_choice->icon != 0)
	    tmp_choice->normalPM = XCreatePixmapFromBitmapData(tool_d,
				XtWindow(ind_panel),
				tmp_choice->icon->data,
				tmp_choice->icon->width,
				tmp_choice->icon->height, ind_but_fg, ind_but_bg,
				DefaultDepthOfScreen(tool_s));
    }
}

update_indpanel(mask)
    int		    mask;
{
    register int    i;
    register ind_sw_info *isw;

    /* only update current mask if user wants to see relevant ind buttons */
    if (appres.ShowAllButtons)
	return;

    cur_indmask = mask;
    XtUnmanageChild(ind_box);
    for (isw = ind_switches, i = 0; i < NUM_IND_SW; isw++, i++) {
	if (isw->func & cur_indmask) {
	    XtManageChild(isw->formw);
	} else {
	    XtUnmanageChild(isw->formw);
	}
    }
    XtManageChild(ind_box);
}

/* come here when a button is pressed in the indicator panel */

static void
sel_ind_but(widget, closure, event, continue_to_dispatch)
    Widget	    widget;
    XtPointer	    closure;
    XEvent*	    event;
    Boolean*	    continue_to_dispatch;
{
    XButtonEvent xbutton;
    ind_sw_info *isw = (ind_sw_info *) closure;
    xbutton = event->xbutton;
    if ((xbutton.button == Button2)  ||
              (xbutton.button == Button3 && xbutton.state & Mod1Mask)) { /* middle button */
	dec_action(isw);
    } else if (xbutton.button == Button3) {	/* right button */
	inc_action(isw);
    } else {			/* left button */
	if (isw->func == I_FONT)
	    popup_fonts(isw);
	else if (isw->func == I_TEXTFLAGS)
	    popup_flags_panel(isw);
	else if (isw->type == I_IVAL || isw->type == I_FVAL)
	    popup_nval_panel(isw);
	else if (isw->type == I_CHOICE)
	    popup_choice_panel(isw);
    }
}

static
update_string_pixmap(isw, buf, xpos, ypos)
    ind_sw_info	   *isw;
    char	   *buf;
    int		    xpos, ypos;
{
    XDrawImageString(tool_d, isw->normalPM, ind_button_gc,
		     xpos, ypos, buf, strlen(buf));
    /*
     * Fool the toolkit by changing the background pixmap to 0 then giving it
     * the modified one again.	Otherwise, it sees that the pixmap ID is not
     * changed and doesn't actually draw it into the widget window
     */
    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(isw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, isw->normalPM);
    SetValues(isw->button);
}

static
update_choice_pixmap(isw, mode)
    ind_sw_info	   *isw;
    int		    mode;
{
    choice_info	   *tmp_choice;
    register Pixmap p;

    /* put the pixmap in the widget background */
    p = isw->normalPM;
    tmp_choice = isw->choices + mode;
    XPutImage(tool_d, p, ind_button_gc, tmp_choice->icon, 0, 0, 32, 0, 32, 32);
    /*
     * Fool the toolkit by changing the background pixmap to 0 then giving it
     * the modified one again.	Otherwise, it sees that the pixmap ID is not
     * changed and doesn't actually draw it into the widget window
     */
    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(isw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, isw->normalPM);
    SetValues(isw->button);
}

/********************************************************

	auxiliary functions

********************************************************/

Widget	choice_popup;
static ind_sw_info *choice_i;
static Widget	nval_popup, form, cancel, set, beside, below, newvalue,
		label;
static Widget	dash_length, dot_gap;
static ind_sw_info *nval_i;

/* handle choice settings */

void
choice_panel_dismiss()
{
    XtPopdown(choice_popup);
    XtSetSensitive(choice_i->button, True);
}

static void
choice_panel_cancel(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    choice_panel_dismiss();
}

static void
choice_panel_set(w, sel_choice, ev)
    Widget	    w;
    choice_info	   *sel_choice;
    XButtonEvent   *ev;
{
    (*choice_i->i_varadr) = sel_choice->value;
    show_action(choice_i);

    /* auxiliary info */
    switch (choice_i->func) {
    case I_LINESTYLE:
	/* dash length */
	cur_dashlength = (float) atof(panel_get_value(dash_length));
	if (cur_dashlength <= 0.0)
	    cur_dashlength = DEF_DASHLENGTH;

	/* dot gap */
	cur_dotgap = (float) atof(panel_get_value(dot_gap));
	if (cur_dotgap <= 0.0)
	    cur_dotgap = DEF_DOTGAP;

	if(cur_linestyle==DASH_LINE)
	  cur_styleval=cur_dashlength;
	else if(cur_linestyle==DOTTED_LINE)
	  cur_styleval=cur_dotgap;

	break;
    }

    choice_panel_dismiss();
}

popup_choice_panel(isw)
    ind_sw_info	   *isw;
{
    Position	    x_val, y_val;
    Dimension	    width, height;
    char	    buf[32];
    choice_info	   *tmp_choice;
    Pixmap	    p;
    int		    i, offset;
    static int      actions_added=0;

    choice_i = isw;
    XtSetSensitive(choice_i->button, False);

    /* if already created, just pop it up */
    if (isw->panel) {
	choice_popup = isw->panel;
	if (isw->func == I_PEN_COLOR || isw->func == I_FILL_COLOR) {
		/* activate the one the user pressed (pen or fill) */
		pen_fill_activate(isw->func);
		/* and store current pen and fill colors in the panels */
		restore_mixed_colors();
	}
	XtPopup(choice_popup, XtGrabExclusive);
	/* insure that the most recent colormap is installed */
	set_cmap(XtWindow(isw->panel));
	return;
    }

    FirstArg(XtNwidth, &width);
    NextArg(XtNheight, &height);
    GetValues(tool);
    if (isw->func == I_PEN_COLOR || isw->func == I_FILL_COLOR) {
        /* initially position the popup 1/3 in from left and 1/5 down from top */
        XtTranslateCoords(tool, (Position) (width / 3), (Position) (height / 5),
		      &x_val, &y_val);
    } else {
        /* initially position the popup 1/3 in from left and 2/3 down from top */
        XtTranslateCoords(tool, (Position) (width / 3), (Position) (2 * height / 3),
		      &x_val, &y_val);
    }

    FirstArg(XtNx, x_val);
    NextArg(XtNy, y_val);
    NextArg(XtNresize, False);
    NextArg(XtNresizable, False);
    NextArg(XtNtitle, "Xfig: Set indicator panel");
    NextArg(XtNcolormap, tool_cm);

    choice_popup = XtCreatePopupShell("set_indicator_panel",
				      transientShellWidgetClass, tool,
				      Args, ArgCount);

    /* put in the widget so we don't have to create it next time */
    isw->panel = choice_popup;

    /* if this is a color popup button copy the panel id to the other one too */
    /* (e.g. if this is the pen color pop, copy id to fill color panel */
    if (isw->func == I_FILL_COLOR)
	pen_color_button->panel = isw->panel;
    else if (isw->func == I_PEN_COLOR)
	fill_color_button->panel = isw->panel;

    XtOverrideTranslations(choice_popup,
                       XtParseTranslationTable(choice_translations));
    if (!actions_added) {
        XtAppAddActions(tool_app, choice_actions, XtNumber(choice_actions));
	actions_added = 1;
    }

    form = XtCreateManagedWidget("form", formWidgetClass, choice_popup, NULL, 0);

    FirstArg(XtNborderWidth, 0);
    if (isw->func == I_PEN_COLOR || isw->func == I_FILL_COLOR)
	sprintf(buf, "Colors");
    else
	sprintf(buf, "%s %s", isw->line1, isw->line2);
    label = XtCreateManagedWidget(buf, labelWidgetClass, form, Args, ArgCount);

    FirstArg(XtNlabel, "Cancel");
    NextArg(XtNfromVert, label);
    NextArg(XtNresize, False);
    NextArg(XtNresizable, False);
    NextArg(XtNheight, 32);
    NextArg(XtNborderWidth, INTERNAL_BW);
    cancel = XtCreateManagedWidget("cancel", commandWidgetClass,
				   form, Args, ArgCount);
    XtAddEventHandler(cancel, ButtonReleaseMask, (Boolean) 0,
		      (XtEventHandler)choice_panel_cancel, (XtPointer) NULL);

    /* colors have the additional "extended color" panel */
    if (isw->func == I_PEN_COLOR || isw->func == I_FILL_COLOR) {
	create_color_panel(form,label,cancel,isw);
	return;
    }

    tmp_choice = isw->choices;

    for (i = 0; i < isw->numchoices; tmp_choice++, i++) {
	if (isw->func == I_FILLSTYLE) {
	    if (cur_fillcolor == BLACK || cur_fillcolor == WHITE ||
		cur_fillcolor == DEFAULT) {
		    if (i > NUMSHADEPATS && i <= NUMSHADEPATS+NUMTINTPATS)
			continue;		/* skip the tints for black */
	    }
	    p = ((cur_fillcolor == BLACK || cur_fillcolor == DEFAULT)?
		fillstyle_choices[i].blackPM: fillstyle_choices[i].normalPM);
	    tmp_choice->value = i-1;		/* fill value = i-1 */
	} else {
	    p = tmp_choice->normalPM;
	}
	/* check for new row of buttons */
	offset = 0;
	if (isw->func == I_FILLSTYLE &&
	       ((cur_fillcolor == BLACK || cur_fillcolor == DEFAULT ||
		 cur_fillcolor == WHITE) && i > NUMSHADEPATS))
			offset = NUMTINTPATS;
	if ((i-offset) % isw->sw_per_row == 0) {
	    if (i == 0)
		below = label;
	    else
		below = beside;
	    beside = cancel;
	}
	FirstArg(XtNfromVert, below);
	NextArg(XtNfromHoriz, beside);
	NextArg(XtNbackgroundPixmap, p);
	NextArg(XtNwidth, tmp_choice->icon->width);
	NextArg(XtNheight, tmp_choice->icon->height);
	NextArg(XtNresize, False);
	NextArg(XtNresizable, False);
	NextArg(XtNborderWidth, INTERNAL_BW);
	beside = XtCreateManagedWidget((String)" ", commandWidgetClass,
				       form, Args, ArgCount);
	XtAddEventHandler(beside, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)choice_panel_set, (XtPointer) tmp_choice);
    }

    /* auxiliary info */
    switch (isw->func) {
    case I_LINESTYLE:
	/* dash length */
	FirstArg(XtNfromVert, beside);
	NextArg(XtNborderWidth, 0);
	NextArg(XtNlabel, "Default dash length =");
	label = XtCreateManagedWidget("default_dash_length",
				    labelWidgetClass, form, Args, ArgCount);
	sprintf(buf, "%1.1f", cur_dashlength);
	FirstArg(XtNfromVert, beside);
	NextArg(XtNborderWidth, INTERNAL_BW);
	NextArg(XtNfromHoriz, label);
	NextArg(XtNstring, buf);
	NextArg(XtNinsertPosition, strlen(buf));
	NextArg(XtNeditType, XawtextEdit);
	NextArg(XtNwidth, 40);
	dash_length = XtCreateManagedWidget(buf, asciiTextWidgetClass,
					    form, Args, ArgCount);
	/* ignore CR, enable paste key */
	XtOverrideTranslations(dash_length, 
		XtParseTranslationTable(indpanel_text_translations));
	/* enable mousefun kbd icon */
	XtAugmentTranslations(dash_length, 
		XtParseTranslationTable(kbd_translations));

	/* dot gap */
	FirstArg(XtNfromVert, dash_length);
	NextArg(XtNborderWidth, 0);
	NextArg(XtNlabel, "    Default dot gap =");
	label = XtCreateManagedWidget("default_dot_gap",
				    labelWidgetClass, form, Args, ArgCount);
	sprintf(buf, "%1.1f", cur_dotgap);
	FirstArg(XtNfromVert, dash_length);
	NextArg(XtNborderWidth, INTERNAL_BW);
	NextArg(XtNfromHoriz, label);
	NextArg(XtNstring, buf);
	NextArg(XtNinsertPosition, strlen(buf));
	NextArg(XtNeditType, XawtextEdit);
	NextArg(XtNwidth, 40);
	dot_gap = XtCreateManagedWidget(buf, asciiTextWidgetClass,
					form, Args, ArgCount);
	/* ignore CR, enable paste key */
	XtOverrideTranslations(dot_gap, 
		XtParseTranslationTable(indpanel_text_translations));
	/* enable mousefun kbd icon */
	XtAugmentTranslations(dot_gap, 
		XtParseTranslationTable(kbd_translations));
	break;
    }

    XtPopup(choice_popup, XtGrabExclusive);
    (void) XSetWMProtocols(XtDisplay(choice_popup), XtWindow(choice_popup),
                           &wm_delete_window, 1);
    /* insure that the most recent colormap is installed */
    set_cmap(XtWindow(choice_popup));
}

/* handle text flag settings */

static int      hidden_text_flag, special_text_flag, rigid_text_flag;
static Widget   hidden_text_panel, rigid_text_panel, special_text_panel;
static Widget   hidden_text_menu, special_text_menu, rigid_text_menu;

static void
flags_panel_dismiss()
{
    XtPopdown(nval_popup);
    XtSetSensitive(nval_i->button, True);
}

static void
flags_panel_cancel(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    flags_panel_dismiss();
}

static void
flags_panel_set(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    if (hidden_text_flag)
	cur_textflags |= HIDDEN_TEXT;
    else
	cur_textflags &= ~HIDDEN_TEXT;
    if (special_text_flag)
	cur_textflags |= SPECIAL_TEXT;
    else
	cur_textflags &= ~SPECIAL_TEXT;
    if (rigid_text_flag)
	cur_textflags |= RIGID_TEXT;
    else
	cur_textflags &= ~RIGID_TEXT;
    flags_panel_dismiss();
    show_action(nval_i);
}

static void
hidden_text_select(w, new_hidden_text, garbage)
    Widget          w;
    XtPointer       new_hidden_text, garbage;
{
    FirstArg(XtNlabel, XtName(w));
    SetValues(hidden_text_panel);
    hidden_text_flag = (int) new_hidden_text;
    if (hidden_text_flag)
	put_msg("Text will be displayed as hidden");
    else
	put_msg("Text will be displayed normally");
}

static void
rigid_text_select(w, new_rigid_text, garbage)
    Widget          w;
    XtPointer       new_rigid_text, garbage;
{
    FirstArg(XtNlabel, XtName(w));
    SetValues(rigid_text_panel);
    rigid_text_flag = (int) new_rigid_text;
    if (rigid_text_flag)
	put_msg("Text in compound group will not scale with compound");
    else
	put_msg("Text in compound group will scale with compound");
}

static void
special_text_select(w, new_special_text, garbage)
    Widget          w;
    XtPointer       new_special_text, garbage;
{
    FirstArg(XtNlabel, XtName(w));
    SetValues(special_text_panel);
    special_text_flag = (int) new_special_text;
    if (special_text_flag)
	put_msg("Text will be printed as special during print/export");
    else
	put_msg("Text will be printed as normal during print/export");
}

popup_flags_panel(isw)
    ind_sw_info	   *isw;
{
    Position	    x_val, y_val;
    Dimension	    width, height;
    char	    buf[32];
    static int      actions_added=0;
    static char    *hidden_text_items[] = {
    "Normal ", "Hidden "};
    static char    *rigid_text_items[] = {
    "Normal ", "Rigid  "};
    static char    *special_text_items[] = {
    "Normal ", "Special"};

    nval_i = isw;
    XtSetSensitive(nval_i->button, False);

    /* if already created, just pop it up */
    if (isw->panel) {
	nval_popup = isw->panel;
	XtPopup(nval_popup, XtGrabExclusive);
	/* insure that the most recent colormap is installed */
	set_cmap(XtWindow(isw->panel));
	return;
    }

    rigid_text_flag = (cur_textflags & RIGID_TEXT) ? 1 : 0;
    special_text_flag = (cur_textflags & SPECIAL_TEXT) ? 1 : 0;
    hidden_text_flag = (cur_textflags & HIDDEN_TEXT) ? 1 : 0;

    FirstArg(XtNwidth, &width);
    NextArg(XtNheight, &height);
    GetValues(tool);
    /* position the popup 1/3 in from left and 2/3 down from top */
    XtTranslateCoords(tool, (Position) (width / 3), (Position) (2 * height / 3),
		      &x_val, &y_val);

    FirstArg(XtNx, x_val);
    NextArg(XtNy, y_val);
    NextArg(XtNwidth, 240);
    NextArg(XtNtitle, "Xfig: Set indicator panel");
    NextArg(XtNcolormap, tool_cm);

    nval_popup = XtCreatePopupShell("set_indicator_panel",
				    transientShellWidgetClass, tool,
				    Args, ArgCount);
    /* put in the widget so we don't have to create it next time */
    isw->panel = nval_popup;

    XtOverrideTranslations(nval_popup,
                       XtParseTranslationTable(nval_translations));
    if (!actions_added) {
        XtAppAddActions(tool_app, nval_actions, XtNumber(nval_actions));
	actions_added = 1;
    }

    form = XtCreateManagedWidget("form", formWidgetClass, nval_popup, NULL, 0);

    FirstArg(XtNborderWidth, 0);
    sprintf(buf, "%s %s", isw->line1, isw->line2);
    label = XtCreateManagedWidget(buf, labelWidgetClass, form, Args, ArgCount);

    /* make hidden text menu */

    FirstArg(XtNfromVert, label);
    NextArg(XtNborderWidth, 0);
    beside = XtCreateManagedWidget(" Hidden Flag     =", labelWidgetClass,
                                   form, Args, ArgCount);

    FirstArg(XtNfromVert, label);
    NextArg(XtNfromHoriz, beside);
    hidden_text_panel = XtCreateManagedWidget(
                 hidden_text_items[hidden_text_flag], menuButtonWidgetClass,
                                              form, Args, ArgCount);
    below = hidden_text_panel;
    hidden_text_menu = make_popup_menu(hidden_text_items,
                                       XtNumber(hidden_text_items),
                                     hidden_text_panel, hidden_text_select);

    /* make rigid text menu */

    FirstArg(XtNfromVert, below);
    NextArg(XtNborderWidth, 0);
    beside = XtCreateManagedWidget(" Rigid Flag      =", labelWidgetClass,
                                   form, Args, ArgCount);

    FirstArg(XtNfromVert, below);
    NextArg(XtNfromHoriz, beside);
    rigid_text_panel = XtCreateManagedWidget(
                   rigid_text_items[rigid_text_flag], menuButtonWidgetClass,
                                             form, Args, ArgCount);
    below = rigid_text_panel;
    rigid_text_menu = make_popup_menu(rigid_text_items,
                                      XtNumber(rigid_text_items),
                                      rigid_text_panel, rigid_text_select);

    /* make special text menu */

    FirstArg(XtNfromVert, below);
    NextArg(XtNborderWidth, 0);
    beside = XtCreateManagedWidget(" Special Flag    =", labelWidgetClass,
                                   form, Args, ArgCount);

    FirstArg(XtNfromVert, below);
    NextArg(XtNfromHoriz, beside);
    special_text_panel = XtCreateManagedWidget(
                                      special_text_items[special_text_flag],
                               menuButtonWidgetClass, form, Args, ArgCount);
    below = special_text_panel;
    special_text_menu = make_popup_menu(special_text_items,
                                        XtNumber(special_text_items),
                                   special_text_panel, special_text_select);

    FirstArg(XtNlabel, "Cancel");
    NextArg(XtNfromVert, below);
    NextArg(XtNborderWidth, INTERNAL_BW);
    cancel = XtCreateManagedWidget("cancel", commandWidgetClass,
				   form, Args, ArgCount);
    XtAddEventHandler(cancel, ButtonReleaseMask, (Boolean) 0,
		      (XtEventHandler)flags_panel_cancel, (XtPointer) NULL);

    FirstArg(XtNlabel, "Set");
    NextArg(XtNfromVert, below);
    NextArg(XtNfromHoriz, cancel);
    NextArg(XtNborderWidth, INTERNAL_BW);
    set = XtCreateManagedWidget("set", commandWidgetClass,
				form, Args, ArgCount);
    XtAddEventHandler(set, ButtonReleaseMask, (Boolean) 0,
		      (XtEventHandler)flags_panel_set, (XtPointer) NULL);

    XtPopup(nval_popup, XtGrabExclusive);
    (void) XSetWMProtocols(XtDisplay(nval_popup), XtWindow(nval_popup),
                           &wm_delete_window, 1);
    /* insure that the most recent colormap is installed */
    set_cmap(XtWindow(nval_popup));
}

/* handle integer and floating point settings */

static void
nval_panel_dismiss()
{
    XtDestroyWidget(nval_popup);
    XtSetSensitive(nval_i->button, True);
}

static void
nval_panel_cancel(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    nval_panel_dismiss();
}

static void
nval_panel_set(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    int		    new_i_value;
    float	    new_f_value;


    if (nval_i->type == I_IVAL) {
	    new_i_value = atoi(panel_get_value(newvalue));
	    (*nval_i->i_varadr) = new_i_value;
    } else {
	    new_f_value = atof(panel_get_value(newvalue));
	    (*nval_i->f_varadr) = new_f_value;
    }
    nval_panel_dismiss();
    show_action(nval_i);
}

popup_nval_panel(isw)
    ind_sw_info	   *isw;
{
    Position	    x_val, y_val;
    Dimension	    width, height;
    char	    buf[32];
    static int      actions_added=0;

    nval_i = isw;
    XtSetSensitive(nval_i->button, False);

    FirstArg(XtNwidth, &width);
    NextArg(XtNheight, &height);
    GetValues(tool);
    /* position the popup 1/3 in from left and 2/3 down from top */
    XtTranslateCoords(tool, (Position) (width / 3), (Position) (2 * height / 3),
		      &x_val, &y_val);

    FirstArg(XtNx, x_val);
    NextArg(XtNy, y_val);
    NextArg(XtNwidth, 240);
    NextArg(XtNtitle, "Xfig: Set indicator panel");
    NextArg(XtNcolormap, tool_cm);

    nval_popup = XtCreatePopupShell("set_indicator_panel",
				    transientShellWidgetClass, tool,
				    Args, ArgCount);
    XtOverrideTranslations(nval_popup,
                       XtParseTranslationTable(nval_translations));
    if (!actions_added) {
        XtAppAddActions(tool_app, nval_actions, XtNumber(nval_actions));
	actions_added = 1;
    }

    form = XtCreateManagedWidget("form", formWidgetClass, nval_popup, NULL, 0);

    FirstArg(XtNborderWidth, 0);
    sprintf(buf, "%s %s", isw->line1, isw->line2);
    label = XtCreateManagedWidget(buf, labelWidgetClass, form, Args, ArgCount);

    FirstArg(XtNfromVert, label);
    NextArg(XtNborderWidth, 0);
    NextArg(XtNlabel, "Value =");
    newvalue = XtCreateManagedWidget("valueLabel", labelWidgetClass,
				     form, Args, ArgCount);
    /* int or float? */
    if (isw->type == I_IVAL)
	    sprintf(buf, "%d", (*isw->i_varadr));
    else
	    sprintf(buf, "%4.2lf", (*isw->f_varadr));
    FirstArg(XtNfromVert, label);
    NextArg(XtNborderWidth, INTERNAL_BW);
    NextArg(XtNfromHoriz, newvalue);
    NextArg(XtNstring, buf);
    NextArg(XtNinsertPosition, strlen(buf));
    NextArg(XtNeditType, XawtextEdit);
    NextArg(XtNwidth, 40);
    newvalue = XtCreateManagedWidget("valueText", asciiTextWidgetClass,
				     form, Args, ArgCount);

    /* set value on carriage return */
    XtOverrideTranslations(newvalue, XtParseTranslationTable(set_translations));
    /* enable mousefun kbd icon */
    XtAugmentTranslations(newvalue, XtParseTranslationTable(kbd_translations));


    FirstArg(XtNlabel, "Cancel");
    NextArg(XtNfromVert, newvalue);
    NextArg(XtNborderWidth, INTERNAL_BW);
    cancel = XtCreateManagedWidget("cancel", commandWidgetClass,
				   form, Args, ArgCount);
    XtAddEventHandler(cancel, ButtonReleaseMask, (Boolean) 0,
		      (XtEventHandler)nval_panel_cancel, (XtPointer) NULL);

    FirstArg(XtNlabel, "Set");
    NextArg(XtNfromVert, newvalue);
    NextArg(XtNfromHoriz, cancel);
    NextArg(XtNborderWidth, INTERNAL_BW);
    set = XtCreateManagedWidget("set", commandWidgetClass,
				form, Args, ArgCount);
    XtAddEventHandler(set, ButtonReleaseMask, (Boolean) 0,
		      (XtEventHandler)nval_panel_set, (XtPointer) NULL);

    XtPopup(nval_popup, XtGrabExclusive);
    (void) XSetWMProtocols(XtDisplay(nval_popup), XtWindow(nval_popup),
                           &wm_delete_window, 1);
    /* insure that the most recent colormap is installed */
    set_cmap(XtWindow(nval_popup));
}

/********************************************************

	commands to change indicator settings

********************************************************/

update_current_settings()
{
    int		    i;
    ind_sw_info	   *isw;

    for (i = 0; i < NUM_IND_SW; ++i) {
	isw = &ind_switches[i];
	show_action(isw);
    }
}

static
dec_choice(sw)
    ind_sw_info	   *sw;
{
    if (--(*sw->i_varadr) < 0)
	(*sw->i_varadr) = sw->numchoices - 1;
    show_action(sw);
}

static
inc_choice(sw)
    ind_sw_info	   *sw;
{
    if (++(*sw->i_varadr) > sw->numchoices - 1)
	(*sw->i_varadr) = 0;
    show_action(sw);
}

/* ARROW MODE		 */

static
show_arrowmode(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_arrowmode);
    switch (cur_arrowmode) {
    case L_NOARROWS:
	autobackwardarrow_mode = 0;
	autoforwardarrow_mode = 0;
	put_msg("NO ARROWS");
	break;
    case L_FARROWS:
	autobackwardarrow_mode = 0;
	autoforwardarrow_mode = 1;
	put_msg("Auto FORWARD ARROWS (for ARC, POLYLINE and SPLINE)");
	break;
    case L_FBARROWS:
	autobackwardarrow_mode = 1;
	autoforwardarrow_mode = 1;
	put_msg("Auto FORWARD and BACKWARD ARROWS (for ARC, POLYLINE and SPLINE)");
	break;
    case L_BARROWS:
	autobackwardarrow_mode = 1;
	autoforwardarrow_mode = 0;
	put_msg("Auto BACKWARD ARROWS (for ARC, POLYLINE and SPLINE)");
	break;
    }
}
/* ARROW TYPE		 */

static
show_arrowtype(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_arrowtype);
    if (cur_arrowtype == 0)
	put_msg("Arrow type 0");
    else if (cur_arrowtype % 2)
	put_msg("Hollow arrow type %d",cur_arrowtype/2);
    else
	put_msg("Solid arrow type %d",cur_arrowtype/2);
}

/* LINE WIDTH		 */

static
dec_linewidth(sw)
    ind_sw_info	   *sw;
{
    --cur_linewidth;
    show_linewidth(sw);
}

static
inc_linewidth(sw)
    ind_sw_info	   *sw;
{
    ++cur_linewidth;
    show_linewidth(sw);
}

static
show_linewidth(sw)
    ind_sw_info	   *sw;
{
    if (cur_linewidth > MAXLINEWIDTH)
	cur_linewidth = MAXLINEWIDTH;
    else if (cur_linewidth < 0)
	cur_linewidth = 0;

    /* erase by drawing wide, inverted (white) line */
    pw_vector(sw->normalPM, DEF_IND_SW_WD / 2 + 2, DEF_IND_SW_HT / 2,
	      sw->sw_width - 2, DEF_IND_SW_HT / 2, ERASE,
	      DEF_IND_SW_HT, PANEL_LINE, 0.0, DEFAULT);
    /* draw current line thickness into pixmap */
    if (cur_linewidth > 0)	/* don't draw line for zero-thickness */
	pw_vector(sw->normalPM, DEF_IND_SW_WD / 2 + 2, DEF_IND_SW_HT / 2,
		  sw->sw_width - 2, DEF_IND_SW_HT / 2, PAINT,
		  cur_linewidth, PANEL_LINE, 0.0, DEFAULT);

    /*
     * Fool the toolkit by changing the background pixmap to 0 then giving it
     * the modified one again.	Otherwise, it sees that the pixmap ID is not
     * changed and doesn't actually draw it into the widget window
     */
    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(sw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, sw->normalPM);
    SetValues(sw->button);
    put_msg("LINE Thickness = %d", cur_linewidth);
}

/* ANGLE GEOMETRY		 */

static
show_anglegeom(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_anglegeom);
    switch (cur_anglegeom) {
    case L_UNCONSTRAINED:
	manhattan_mode = 0;
	mountain_mode = 0;
	latexline_mode = 0;
	latexarrow_mode = 0;
	put_msg("UNCONSTRAINED geometry (for POLYLINE and SPLINE)");
	break;
    case L_MOUNTHATTAN:
	mountain_mode = 1;
	manhattan_mode = 1;
	latexline_mode = 0;
	latexarrow_mode = 0;
	put_msg("MOUNT-HATTAN geometry (for POLYLINE and SPLINE)");
	break;
    case L_MANHATTAN:
	manhattan_mode = 1;
	mountain_mode = 0;
	latexline_mode = 0;
	latexarrow_mode = 0;
	put_msg("MANHATTAN geometry (for POLYLINE and SPLINE)");
	break;
    case L_MOUNTAIN:
	mountain_mode = 1;
	manhattan_mode = 0;
	latexline_mode = 0;
	latexarrow_mode = 0;
	put_msg("MOUNTAIN geometry (for POLYLINE and SPLINE)");
	break;
    case L_LATEXLINE:
	latexline_mode = 1;
	manhattan_mode = 0;
	mountain_mode = 0;
	latexarrow_mode = 0;
	put_msg("LATEX LINE geometry: allow only LaTeX line slopes");
	break;
    case L_LATEXARROW:
	latexarrow_mode = 1;
	manhattan_mode = 0;
	mountain_mode = 0;
	latexline_mode = 0;
	put_msg("LATEX ARROW geometry: allow only LaTeX arrow slopes");
	break;
    }
}

/* ARC TYPE */

static
show_arctype(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_arctype);
    switch (cur_arctype) {
    case T_OPEN_ARC:
	put_msg("OPEN arc");
	break;
    case T_PIE_WEDGE_ARC:
	put_msg("PIE WEDGE (closed) arc");
	break;
    }
}


/* JOIN STYLE		 */

static
show_joinstyle(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_joinstyle);
    switch (cur_joinstyle) {
    case JOIN_MITER:
	put_msg("MITER line join style");
	break;
    case JOIN_ROUND:
	put_msg("ROUND line join style");
	break;
    case JOIN_BEVEL:
	put_msg("BEVEL line join style");
	break;
    }
}

/* CAP STYLE		 */

static
show_capstyle(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_capstyle);
    switch (cur_capstyle) {
    case CAP_BUTT:
	put_msg("BUTT line cap style");
	break;
    case CAP_ROUND:
	put_msg("ROUND line cap style");
	break;
    case CAP_PROJECT:
	put_msg("PROJECTING line cap style");
	break;
    }
}

/* LINE STYLE		 */

static
show_linestyle(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_linestyle);
    switch (cur_linestyle) {
    case SOLID_LINE:
	cur_styleval = 0.0;
	put_msg("SOLID LINE STYLE (for BOX, POLYGON and POLYLINE)");
	break;
    case DASH_LINE:
	cur_styleval = cur_dashlength;
	put_msg("DASH LINE STYLE (for BOX, POLYGON and POLYLINE)");
	break;
    case DOTTED_LINE:
	cur_styleval = cur_dotgap;
	put_msg("DOTTED LINE STYLE (for BOX, POLYGON and POLYLINE)");
	break;
    }
}

/* VERTICAL ALIGNMENT	 */

static
show_valign(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_valign);
    switch (cur_valign) {
    case ALIGN_NONE:
	put_msg("No vertical alignment");
	break;
    case ALIGN_TOP:
	put_msg("Vertically align to TOP");
	break;
    case ALIGN_CENTER:
	put_msg("Center vertically when aligning");
	break;
    case ALIGN_BOTTOM:
	put_msg("Vertically align to BOTTOM");
	break;
    case ALIGN_DISTRIB_C:
	put_msg("Vertically DISTRIBUTE objects, equal distance between CENTRES");
	break;
    case ALIGN_DISTRIB_E:
	put_msg("Vertically DISTRIBUTE objects, equal distance between EDGES");
	break;
    case ALIGN_ABUT:
	put_msg("Vertically ABUT the objects together");
	break;
    }
}

/* HORIZ ALIGNMENT	 */

static
show_halign(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_halign);
    switch (cur_halign) {
    case ALIGN_NONE:
	put_msg("No horizontal alignment");
	break;
    case ALIGN_LEFT:
	put_msg("Horizontally align to LEFT");
	break;
    case ALIGN_CENTER:
	put_msg("Center horizontally when aligning");
	break;
    case ALIGN_RIGHT:
	put_msg("Horizontally align to RIGHT");
	break;
    case ALIGN_DISTRIB_C:
	put_msg("Horizontally DISTRIBUTE objects, equal distance between CENTRES");
	break;
    case ALIGN_DISTRIB_E:
	put_msg("Horizontally DISTRIBUTE objects, equal distance between EDGES");
	break;
    case ALIGN_ABUT:
	put_msg("Horizontally ABUT the objects together");
	break;
    }
}

/* GRID MODE	 */

static
show_gridmode(sw)
    ind_sw_info	   *sw;
{
    static int	    prev_gridmode = -1;

    update_choice_pixmap(sw, cur_gridmode);
    switch (cur_gridmode) {
    case GRID_0:
	put_msg("No grid");
	break;
    case GRID_1:
	put_msg("Small grid");
	break;
    case GRID_2:
	put_msg("Medium grid");
	break;
    case GRID_3:
	put_msg("Large grid");
	break;
    }
    if (cur_gridmode != prev_gridmode)
	setup_grid(cur_gridmode);
    prev_gridmode = cur_gridmode;
}

/* POINT POSITION	 */

static
show_pointposn(sw)
    ind_sw_info	   *sw;
{
    char	    buf[80];

    update_choice_pixmap(sw, cur_pointposn);
    switch (cur_pointposn) {
    case P_ANY:
	put_msg("Arbitrary Positioning of Points");
	break;
    case P_MAGNET:
    case P_GRID1:
    case P_GRID2:
	sprintf(buf,
	  "MAGNET MODE: entered points rounded to the nearest %s increment",
		grid_name[cur_pointposn]);
	put_msg(buf);
	break;
    }
}

/* SMART LINK MODE */

static
show_linkmode(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_linkmode);
    switch (cur_linkmode) {
    case SMART_OFF:
	put_msg("Do not adjust links automatically");
	break;
    case SMART_MOVE:
	put_msg("Adjust links automatically by moving endpoint");
	break;
    case SMART_SLIDE:
	put_msg("Adjust links automatically by sliding endlink");
	break;
    }
}

/* TEXT JUSTIFICATION	 */

static
show_textjust(sw)
    ind_sw_info	   *sw;
{
    update_choice_pixmap(sw, cur_textjust);
    switch (cur_textjust) {
    case T_LEFT_JUSTIFIED:
	put_msg("Left justify text");
	break;
    case T_CENTER_JUSTIFIED:
	put_msg("Center text");
	break;
    case T_RIGHT_JUSTIFIED:
	put_msg("Right justify text");
	break;
    }
}

/* BOX RADIUS	 */

static
dec_boxradius(sw)
    ind_sw_info	   *sw;
{
    --cur_boxradius;
    show_boxradius(sw);
}

static
inc_boxradius(sw)
    ind_sw_info	   *sw;
{
    ++cur_boxradius;
    show_boxradius(sw);
}

#define MAXRADIUS 30

static
show_boxradius(sw)
    ind_sw_info	   *sw;
{
    if (cur_boxradius > MAXRADIUS)
	cur_boxradius = MAXRADIUS;
    else if (cur_boxradius < 3)
	cur_boxradius = 3;
    /* erase by drawing wide, inverted (white) line */
    pw_vector(sw->normalPM, DEF_IND_SW_WD / 2, DEF_IND_SW_HT / 2,
	      DEF_IND_SW_WD, DEF_IND_SW_HT / 2, ERASE,
	      DEF_IND_SW_HT, PANEL_LINE, 0.0, DEFAULT);
    /* draw current radius into pixmap */
    curve(sw->normalPM, 0, cur_boxradius, -cur_boxradius, 0, False, 1, 50,
	  cur_boxradius, cur_boxradius, DEF_IND_SW_WD - 2, DEF_IND_SW_HT - 2,
	  PAINT, 1, PANEL_LINE, 0.0, 0, DEFAULT, DEFAULT, CAP_BUTT);

    /*
     * Fool the toolkit by changing the background pixmap to 0 then giving it
     * the modified one again.	Otherwise, it sees that the pixmap ID is not
     * changed and doesn't actually draw it into the widget window
     */
    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(sw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, sw->normalPM);
    SetValues(sw->button);
    put_msg("ROUNDED-CORNER BOX Radius = %d", cur_boxradius);
}

/* FILL STYLE */

static
darken_fill(sw)
    ind_sw_info	   *sw;
{
    cur_fillstyle++;
    if ((cur_fillcolor == BLACK || cur_fillcolor == DEFAULT ||
	 cur_fillcolor == WHITE) && cur_fillstyle == NUMSHADEPATS)
		cur_fillstyle = NUMSHADEPATS+NUMTINTPATS;	/* skip to patterns */
    else if (cur_fillstyle >= NUMFILLPATS)
	    cur_fillstyle = UNFILLED;
    show_fill_style(sw);
}

static
lighten_fill(sw)
    ind_sw_info	   *sw;
{
    cur_fillstyle--;
    if (cur_fillstyle < UNFILLED)
	    cur_fillstyle = NUMFILLPATS-1;	/* set to patterns */
    if ((cur_fillcolor == BLACK || cur_fillcolor == DEFAULT ||
	 cur_fillcolor == WHITE) && cur_fillstyle == NUMSHADEPATS+NUMTINTPATS-1)
	    cur_fillstyle = NUMSHADEPATS-1;	/* no tints */
    show_fill_style(sw);
}

show_fill_style(sw)
    ind_sw_info	   *sw;
{
    /* we must check the validity of the fill style again in case the user changed
       colors.  In that case, there may be an illegal fill value (e.g. for black
       there are no "tints" */
    if ((cur_fillcolor == BLACK || cur_fillcolor == DEFAULT || cur_fillcolor == WHITE) &&
	 (cur_fillstyle >= NUMSHADEPATS && cur_fillstyle < NUMSHADEPATS+NUMTINTPATS))
	    cur_fillstyle = UNFILLED;	/* no tints, set unfilled */
    XSetFillStyle(tool_d, ind_button_gc, FillTiled);
    if (cur_fillstyle == UNFILLED) {
	XSetTile(tool_d, ind_button_gc,
		(cur_fillcolor==BLACK ||
		   (cur_fillcolor==DEFAULT && x_fg_color.pixel==colors[BLACK])?
				fillstyle_choices[0].blackPM:
				fillstyle_choices[0].normalPM));
	XFillRectangle(tool_d, sw->normalPM, ind_button_gc, 32, 0, 32, 32);
	put_msg("NO-FILL MODE");
    } else {
	/* put the pixmap in the widget background */
	XSetTile(tool_d, ind_button_gc,
		(cur_fillcolor==BLACK ||
		   (cur_fillcolor==DEFAULT && x_fg_color.pixel==colors[BLACK])?
				fillstyle_choices[cur_fillstyle+1].blackPM:
				fillstyle_choices[cur_fillstyle+1].normalPM));
	XFillRectangle(tool_d, sw->normalPM, ind_button_gc, 35, 4, 26, 24);
	if (cur_fillstyle < NUMSHADEPATS+NUMTINTPATS)
	   put_msg("FILL MODE (black density/color intensity = %d%%)",
		(cur_fillstyle * 200) / (NUMSHADEPATS+NUMTINTPATS - 1));
	else
	   put_msg("FILL pattern %d",cur_fillstyle-NUMSHADEPATS-NUMTINTPATS);
    }
    XSetFillStyle(tool_d, ind_button_gc, FillSolid);
    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(sw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, sw->normalPM);
    SetValues(sw->button);
}

/* change the colors of the fill style indicators */


recolor_fillstyles()
{
    int 	    i,j;
    double	    savezoom;
    Pixmap	    savepm;

    savezoom = display_zoomscale;
    display_zoomscale = 1.0;
    for (i = 0; i < NUMFILLPATS; i++) {
	j = i-(NUMTINTPATS+NUMSHADEPATS);
	if (j >= 0) {
	    savepm = fill_pm[i];
	    /* use the one create at zoom = 1 */
	    fill_pm[i] = fill_but_pm[j];
	}
	set_fill_gc(i, PAINT, cur_pencolor, cur_fillcolor, 0, 0);
	if ((cur_fillcolor == WHITE || cur_fillcolor == BLACK || cur_fillcolor == DEFAULT) && 
	    (i >= NUMSHADEPATS && i < NUMTINTPATS+NUMSHADEPATS))
		continue;
	XFillRectangle(tool_d, fillstyle_choices[i+1].normalPM, fillgc, 0, 0, 32, 32);
	if (i < NUMSHADEPATS || (i >= NUMTINTPATS+NUMSHADEPATS && 
	    (cur_fillcolor == BLACK || cur_fillcolor == DEFAULT))) {
	    XFillRectangle(tool_d, fillstyle_choices[i+1].blackPM, fillgc, 0, 0, 32, 32);
	}
	if (j >= 0) {
	    fill_pm[i] = savepm;
	}
    }
    display_zoomscale = savezoom;
}

/* TEXT FLAGS */

static
inc_flags(sw)
    ind_sw_info	   *sw;
{
    if (++cur_flagshown > MAX_FLAGS)
	cur_flagshown = 0;
    show_flags(sw);
}

static
dec_flags(sw)
    ind_sw_info	   *sw;
{
    if (--cur_flagshown < 0)
	cur_flagshown = MAX_FLAGS;
    show_flags(sw);
}

static
show_flags(sw)
    ind_sw_info	   *sw;
{
    put_msg("Text flags: Hidden=%s, Special=%s, Rigid=%s (Button 1 to change)",
		(cur_textflags & HIDDEN_TEXT) ? "on" : "off",
		(cur_textflags & SPECIAL_TEXT) ? "on" : "off",
		(cur_textflags & RIGID_TEXT) ? "on" : "off");

    /* write the text/ellipse angle in the background pixmap */
    switch(cur_flagshown) {
	case 0:
	    sprintf(indbuf, "hidden=%s",
			(cur_textflags & HIDDEN_TEXT) ? "on  " : "off ");
	    break;
	case 1:
	    sprintf(indbuf, "special=%s",
			(cur_textflags & SPECIAL_TEXT) ? "on " : "off");
	    break;
	default:
	    sprintf(indbuf, "rigid=%s",
			(cur_textflags & RIGID_TEXT) ? "on   " : "off  ");
    }
    update_string_pixmap(sw, indbuf, 6, 26);
}

/* FONT */

static
inc_font(sw)
    ind_sw_info	   *sw;
{
    if (using_ps)
	cur_ps_font++;
    else
	cur_latex_font++;
    show_font(sw);
}

static
dec_font(sw)
    ind_sw_info	   *sw;
{
    if (using_ps)
	cur_ps_font--;
    else
	cur_latex_font--;
    show_font(sw);
}

static
show_font(sw)
    ind_sw_info	   *sw;
{
    if (using_ps) {
	if (cur_ps_font >= NUM_FONTS)
	    cur_ps_font = DEFAULT;
	else if (cur_ps_font < DEFAULT)
	    cur_ps_font = NUM_FONTS - 1;
    } else {
	if (cur_latex_font >= NUM_LATEX_FONTS)
	    cur_latex_font = 0;
	else if (cur_latex_font < 0)
	    cur_latex_font = NUM_LATEX_FONTS - 1;
    }

    /* erase larger fontpane bits if we switched to smaller (Latex) */
    XFillRectangle(tool_d, sw->normalPM, ind_blank_gc, 0, 0,
	       32 + max2(PS_FONTPANE_WD, LATEX_FONTPANE_WD), DEF_IND_SW_HT);
    /* and redraw info */
    XDrawImageString(tool_d, sw->normalPM, ind_button_gc, 3, 12, sw->line1,
		     strlen(sw->line1));
    XDrawImageString(tool_d, sw->normalPM, ind_button_gc, 3, 25, sw->line2,
		     strlen(sw->line2));

    XCopyArea(tool_d, using_ps ? psfont_menu_bitmaps[cur_ps_font + 1] :
	      latexfont_menu_bitmaps[cur_latex_font],
	      sw->normalPM, ind_button_gc, 0, 0,
	      using_ps ? PS_FONTPANE_WD : LATEX_FONTPANE_WD,
	      using_ps ? PS_FONTPANE_HT : LATEX_FONTPANE_HT,
	  using_ps ? 32 : 32 + (PS_FONTPANE_WD - LATEX_FONTPANE_WD) / 2, 6);

    FirstArg(XtNbackgroundPixmap, 0);
    SetValues(sw->button);
    /* put the pixmap in the widget background */
    FirstArg(XtNbackgroundPixmap, sw->normalPM);
    SetValues(sw->button);
    put_msg("Font: %s", using_ps ? ps_fontinfo[cur_ps_font + 1].name :
	    latex_fontinfo[cur_latex_font].name);
}

/* popup menu of printer fonts */

static int	psflag;
static ind_sw_info *return_sw;

int		show_font_return();

static
popup_fonts(sw)
    ind_sw_info	   *sw;
{
    return_sw = sw;
    psflag = using_ps ? 1 : 0;
    fontpane_popup(&cur_ps_font, &cur_latex_font, &psflag,
		   show_font_return, sw->button);
}

show_font_return(w)
    Widget	    w;
{
    if (psflag)
	cur_textflags = cur_textflags | PSFONT_TEXT;
    else
	cur_textflags = cur_textflags & (~PSFONT_TEXT);
    show_font(return_sw);
}

/* FONT SIZE */

static
inc_fontsize(sw)
    ind_sw_info	   *sw;
{
    if (cur_fontsize >= 100) {
	cur_fontsize = (cur_fontsize / 10) * 10;	/* round first */
	cur_fontsize += 10;
    } else if (cur_fontsize >= 50) {
	cur_fontsize = (cur_fontsize / 5) * 5;
	cur_fontsize += 5;
    } else if (cur_fontsize >= 20) {
	cur_fontsize = (cur_fontsize / 2) * 2;
	cur_fontsize += 2;
    } else
	cur_fontsize++;
    show_fontsize(sw);
}

static
dec_fontsize(sw)
    ind_sw_info	   *sw;
{
    if (cur_fontsize > 100) {
	cur_fontsize = (cur_fontsize / 10) * 10;	/* round first */
	cur_fontsize -= 10;
    } else if (cur_fontsize > 50) {
	cur_fontsize = (cur_fontsize / 5) * 5;
	cur_fontsize -= 5;
    } else if (cur_fontsize > 20) {
	cur_fontsize = (cur_fontsize / 2) * 2;
	cur_fontsize -= 2;
    } else if (cur_fontsize > 4)
	cur_fontsize--;
    show_fontsize(sw);
}

static
show_fontsize(sw)
    ind_sw_info	   *sw;
{
    if (cur_fontsize < 4)
	cur_fontsize = 4;
    else if (cur_fontsize > 1000)
	cur_fontsize = 1000;

    put_msg("Font size %d", cur_fontsize);
    /* write the font size in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%4d", cur_fontsize);
    update_string_pixmap(sw, indbuf, sw->sw_width - 28, 20);
}

/* ELLIPSE/TEXT ANGLE */

static
inc_elltextangle(sw)
    ind_sw_info	   *sw;
{

    if (cur_elltextangle < 0.0)
	cur_elltextangle = ((int) ((cur_elltextangle-14.999)/15.0))*15.0;
    else
	cur_elltextangle = ((int) (cur_elltextangle/15.0))*15.0;
    cur_elltextangle += 15.0;
    show_elltextangle(sw);
}

static
dec_elltextangle(sw)
    ind_sw_info	   *sw;
{
    if (cur_elltextangle < 0.0)
	cur_elltextangle = ((int) (cur_elltextangle/15.0))*15.0;
    else
	cur_elltextangle = ((int) ((cur_elltextangle+14.999)/15.0))*15.0;
    cur_elltextangle -= 15.0;
    show_elltextangle(sw);
}

static
show_elltextangle(sw)
    ind_sw_info	   *sw;
{
    cur_elltextangle = round(cur_elltextangle*10.0)/10.0;
    if (cur_elltextangle <= -360.0 || cur_elltextangle >= 360)
	cur_elltextangle = 0.0;

    put_msg("Text/Ellipse angle %.1f", cur_elltextangle);
    if (cur_elltextangle == old_elltextangle)
	return;

    /* write the text/ellipse angle in the background pixmap */
    indbuf[0]=indbuf[1]=indbuf[2]=indbuf[3]=indbuf[4]=indbuf[5]=' ';
    sprintf(indbuf, "%5.1f", cur_elltextangle);
    update_string_pixmap(sw, indbuf, sw->sw_width - 40, 26);
    old_elltextangle = cur_elltextangle;
}

/* ROTATION ANGLE */

static
inc_rotnangle(sw)
    ind_sw_info	   *sw;
{
    if (cur_rotnangle < 30 || cur_rotnangle >= 120)
	cur_rotnangle = 30;
    else if (cur_rotnangle < 45)
	cur_rotnangle = 45;
    else if (cur_rotnangle < 60)
	cur_rotnangle = 60;
    else if (cur_rotnangle < 90)
	cur_rotnangle = 90;
    else if (cur_rotnangle < 120)
	cur_rotnangle = 120;
    show_rotnangle(sw);
}

static
dec_rotnangle(sw)
    ind_sw_info	   *sw;
{
    if (cur_rotnangle > 120 || cur_rotnangle <= 30)
	cur_rotnangle = 120;
    else if (cur_rotnangle > 90)
	cur_rotnangle = 90;
    else if (cur_rotnangle > 60)
	cur_rotnangle = 60;
    else if (cur_rotnangle > 45)
	cur_rotnangle = 45;
    else if (cur_rotnangle > 30)
	cur_rotnangle = 30;
    show_rotnangle(sw);
}

static
show_rotnangle(sw)
    ind_sw_info	   *sw;
{
    if (cur_rotnangle < 1)
	cur_rotnangle = 1;
    else if (cur_rotnangle > 180)
	cur_rotnangle = 180;

    put_msg("Angle of rotation %d", cur_rotnangle);
    if (cur_rotnangle == old_rotnangle)
	return;

    /* write the rotation angle in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%3d", cur_rotnangle);
    update_string_pixmap(sw, indbuf, sw->sw_width - 22, 20);

    /* change markers if we changed to or from 90 degrees (except at start)  */
    if (old_rotnangle != -1) {
	if (cur_rotnangle == 90)
	    update_markers(M_ALL);
	else if (old_rotnangle == 90)
	    update_markers(M_ROTATE_ANGLE);
    }
    old_rotnangle = cur_rotnangle;
}

/* NUMSIDES */

static
inc_numsides(sw)
    ind_sw_info	   *sw;
{
    cur_numsides++;
    show_numsides(sw);
}

static
dec_numsides(sw)
    ind_sw_info	   *sw;
{
    cur_numsides--;
    show_numsides(sw);
}

static
show_numsides(sw)
    ind_sw_info	   *sw;
{
    if (cur_numsides < 3)
	cur_numsides = 3;
    else if (cur_numsides > 99)
	cur_numsides = 99;

    put_msg("Number of sides %2d", cur_numsides);
    /* write the number of sides in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%2d", cur_numsides);
    update_string_pixmap(sw, indbuf, sw->sw_width - 18, 20);
}

/* NUMCOPIES */

static
inc_numcopies(sw)
    ind_sw_info	   *sw;
{
    cur_numcopies++;
    show_numcopies(sw);
}

static
dec_numcopies(sw)
    ind_sw_info	   *sw;
{
    cur_numcopies--;
    show_numcopies(sw);
}

static
show_numcopies(sw)
    ind_sw_info	   *sw;
{
    if (cur_numcopies < 1)
	cur_numcopies = 1;
    else if (cur_numcopies > 99)
	cur_numcopies = 99;

    put_msg("Number of copies %2d", cur_numcopies);
    /* write the number of copies in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%2d", cur_numcopies);
    update_string_pixmap(sw, indbuf, sw->sw_width - 18, 20);
}

/* NUMXCOPIES */

static
inc_numxcopies(sw)
    ind_sw_info	   *sw;
{
    cur_numxcopies++;
    show_numxcopies(sw);
}

static
dec_numxcopies(sw)
    ind_sw_info	   *sw;
{
    cur_numxcopies--;
    show_numxcopies(sw);
}

static
show_numxcopies(sw)
    ind_sw_info	   *sw;
{
    if (cur_numxcopies < 0)
	cur_numxcopies = 0;
    else if (cur_numxcopies > 999)
	cur_numxcopies = 999;

    if (!cur_numxcopies) 
      put_msg("Number of copies %2d in x-direction", cur_numxcopies);
    /* write the number of x copies in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%2d", cur_numxcopies);
    update_string_pixmap(sw, indbuf, sw->sw_width - 18, 20);
}

/* NUMYCOPIES */

static
inc_numycopies(sw)
    ind_sw_info	   *sw;
{
    cur_numycopies++;
    show_numycopies(sw);
}

static
dec_numycopies(sw)
    ind_sw_info	   *sw;
{
    cur_numycopies--;
    show_numycopies(sw);
}

static
show_numycopies(sw)
    ind_sw_info	   *sw;
{
    if (cur_numycopies < 0)
	cur_numycopies = 0;
    else if (cur_numycopies > 999)
	cur_numycopies = 999;

    if (!cur_numycopies) 
      put_msg("Number of copies %2d in y-direction", cur_numycopies);
    /* write the number of y copies in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%2d", cur_numycopies);
    update_string_pixmap(sw, indbuf, sw->sw_width - 18, 20);
}

/* ZOOM */

inc_zoom(sw)
    ind_sw_info	   *sw;
{
    if (display_zoomscale < 1.0) {
	if (display_zoomscale < 0.2)
	    display_zoomscale = 0.2;
	else
	    display_zoomscale += 0.2; /* always quantized */
	display_zoomscale = (int)(display_zoomscale*5.0);
	display_zoomscale /= 5.0;
    } else
	display_zoomscale = (int)display_zoomscale + 1.0;
    /* use zoom_sw instead of one passed to us because we might have come
       here from an accelerator */
    show_zoom(zoom_sw);
}

dec_zoom(sw)
    ind_sw_info	   *sw;
{
    if (display_zoomscale <= 1.0) {
	display_zoomscale -= 0.2; /* always quantized */
	display_zoomscale = (int)(display_zoomscale*5.0);
	display_zoomscale /= 5.0;
    } else
	{
	if (display_zoomscale != (int)display_zoomscale)
	    display_zoomscale = (int)display_zoomscale;
	else
	    display_zoomscale = (int)display_zoomscale - 1.0;
	if (display_zoomscale < 1.0)
		display_zoomscale = 1.0;
	}
    /* use zoom_sw instead of one passed to us because we might have come
       here from an accelerator */
    show_zoom(zoom_sw);
}

show_zoom(sw)
    ind_sw_info	   *sw;
{
    if (display_zoomscale < 0.1)
	display_zoomscale = 0.1;
    else if (display_zoomscale > 50.0)
	display_zoomscale = 50.0;

    put_msg("Zoom scale %.1f", display_zoomscale);
    if (display_zoomscale == old_display_zoomscale)
	return;

    /* write the zoom factor in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    if (display_zoomscale == (int) display_zoomscale)
	sprintf(indbuf, "%s%.0f ",
		display_zoomscale<10.0?"  ":" ",display_zoomscale);
    else
	sprintf(indbuf, "%s%.1f",
		display_zoomscale<10.0?" ":"",display_zoomscale);
    update_string_pixmap(sw, indbuf, sw->sw_width - 26, 14);

    zoomscale=display_zoomscale/ZOOM_FACTOR;

    /* fix up the rulers and grid */
    reset_rulers();
    redisplay_rulers();
    /* reload text objects' font structures since we need
	to load larger/smaller fonts */
    reload_text_fstructs();
    /* clear fill patterns until they are used if the zoom changed */
    if (old_display_zoomscale != display_zoomscale)
	    clear_patterns();
    setup_grid(cur_gridmode);
    old_display_zoomscale = display_zoomscale;
}

/* DEPTH */

static
inc_depth(sw)
    ind_sw_info	   *sw;
{
    cur_depth++;
    show_depth(sw);
}

static
dec_depth(sw)
    ind_sw_info	   *sw;
{
    cur_depth--;
    show_depth(sw);
}

show_depth(sw)
    ind_sw_info	   *sw;
{
    if (cur_depth < 0)
	cur_depth = 0;
    else if (cur_depth > MAXDEPTH)
	cur_depth = MAXDEPTH;

    put_msg("Depth %3d", cur_depth);

    /* write the depth in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%3d", cur_depth);
    update_string_pixmap(sw, indbuf, sw->sw_width - 22, 20);
}

/* TEXTSTEP */

static
inc_textstep(sw)
    ind_sw_info	   *sw;
{
    if (cur_textstep >= 10.0) {
	cur_textstep = (int) cur_textstep;	/* round first */
	cur_textstep += 1.0;
    } else if (cur_textstep >= 5.0) {
	cur_textstep = ((int)(cur_textstep*2.0+0.01))/2.0;
	cur_textstep += 0.5;
    } else if (cur_textstep >= 2.0) {
	cur_textstep = ((int)(cur_textstep*5.0+0.01))/5.0;
	cur_textstep += 0.2;
    } else
	cur_textstep += 0.1;
    show_textstep(sw);
}

static
dec_textstep(sw)
    ind_sw_info	   *sw;
{
    if (cur_textstep > 10.0) {
	cur_textstep = (int)cur_textstep;	/* round first */
	cur_textstep -= 1.0;
    } else if (cur_textstep > 5.0) {
	cur_textstep = ((int)(cur_textstep*2.0+0.01))/2.0;
	cur_textstep -= 0.5;
    } else if (cur_textstep > 2.0) {
	cur_textstep = ((int)(cur_textstep*5.0+0.01))/5.0;
	cur_textstep -= 0.2;
    } else if (cur_textstep > 0.4)
	cur_textstep -= 0.1;
    show_textstep(sw);
}

/* could make this more generic - but a copy will do for font set JNT */
static
show_textstep(sw)
    ind_sw_info	   *sw;
{
    if (cur_textstep < 0)
	cur_textstep = 0;
    else if (cur_textstep > 99.0)
	cur_textstep = 99.0;

    put_msg("Text step %.1f", cur_textstep);
    /* write the text step in the background pixmap */
    indbuf[0] = indbuf[1] = indbuf[2] = indbuf[3] = indbuf[4] = '\0';
    sprintf(indbuf, "%4.1f", cur_textstep);
    update_string_pixmap(sw, indbuf, sw->sw_width - 28, 20);
}
