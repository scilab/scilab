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
#include "wf_figx.h"
#include "wf_resources.h"

Cursor		cur_wf_cursor,arrow_wf_cursor, wait_wf_cursor;

void init_wf_cursor()
{
    register Display *d = tool_d;
    cur_wf_cursor		= arrow_wf_cursor;  /* current cursor */
    arrow_wf_cursor	= XCreateFontCursor(d, XC_left_ptr);
    wait_wf_cursor		= XCreateFontCursor(d, XC_watch);
}

void reset_wf_cursor()
{
    XDefineCursor(tool_d, tool_win, cur_wf_cursor);
}

void set_temp_wf_cursor(cursor)
    Cursor	    cursor;
{
    XDefineCursor(tool_d, tool_win, cursor);
}

void set_wf_cursor(cursor)
    Cursor	    cursor;
{
    cur_wf_cursor = cursor;
    XDefineCursor(tool_d, tool_win, cursor);
}


