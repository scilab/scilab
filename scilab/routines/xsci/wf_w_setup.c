/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 *
 * "Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the copyright
 * notice and this permission notice appear in supporting documentation. 
 * No representations are made about the suitability of this software for 
 * any purpose.  It is provided "as is" without express or implied warranty."
 */

#include "wf_fig.h"
#include "wf_resources.h"
#include "wf_w_setup.h"
#include "wf_w_util.h"

#include "../machine.h"
#include "All-extern.h"

int		CMDPANEL_WD;
int		MODEPANEL_WD;
int		MODEPANEL_SPACE;
int		MSGFORM_WD;
int		MSGPANEL_WD = 600;
int		MOUSEFUN_WD;
int		INDPANEL_WD;
int		CANVAS_WD, CANVAS_HT;
int		INTERNAL_BW=1;
int		TOPRULER_WD, TOPRULER_HT;
int		SIDERULER_WD, SIDERULER_HT;
int		SW_PER_ROW, SW_PER_COL;

void setup_sizes(new_canv_wd, new_canv_ht)
    int		    new_canv_wd, new_canv_ht;
{
    int		    NUM_CMD_SW;

    /*
     * make the width of the mousefun panel about 1/3 of the size of the
     * canvas width and the cmdpanel the remaining width. Be sure to set it
     * up so that cmdpanel buttons can be allocated a size which divides
     * evenly into the remaining space.
     */
    CANVAS_WD = new_canv_wd;
    if (CANVAS_WD < 10)
	CANVAS_WD = 10;
    CANVAS_HT = new_canv_ht;
    if (CANVAS_HT < 10)
	CANVAS_HT = 10;

    SIDERULER_WD = RULER_WD + 8;	/* allow for 100's numbers */
    TOPRULER_HT = RULER_WD;
    TOPRULER_WD = CANVAS_WD;
    SIDERULER_HT = CANVAS_HT;
    if (TOPRULER_WD > MAX_TOPRULER_WD)
	TOPRULER_WD = MAX_TOPRULER_WD;
    if (SIDERULER_HT > MAX_SIDERULER_HT)
	SIDERULER_HT = MAX_SIDERULER_HT;

    MODEPANEL_WD = (MODE_SW_WD + INTERNAL_BW) * SW_PER_ROW + INTERNAL_BW;
    NUM_CMD_SW = 10;
    CMDPANEL_WD = (((2 * CANVAS_WD) / 3 + MODEPANEL_WD +
		    SIDERULER_WD) / NUM_CMD_SW) * NUM_CMD_SW;
    MOUSEFUN_WD = (MODEPANEL_WD + CANVAS_WD + SIDERULER_WD - CMDPANEL_WD);
    while (MOUSEFUN_WD < MIN_MOUSEFUN_WD) {
	MOUSEFUN_WD += NUM_CMD_SW;
	CMDPANEL_WD -= NUM_CMD_SW;
    }
    if (CMDPANEL_WD < 5 * NUM_CMD_SW)
	CMDPANEL_WD = 5 * NUM_CMD_SW;
    MSGFORM_WD = CMDPANEL_WD;

    MODEPANEL_SPACE = CANVAS_HT + RULER_WD -
	SW_PER_COL * (MODE_SW_HT + INTERNAL_BW);
    if (MODEPANEL_SPACE < 2)
	MODEPANEL_SPACE = 2;
    INDPANEL_WD = MODEPANEL_WD + CANVAS_WD + SIDERULER_WD + INTERNAL_BW*2;
}
