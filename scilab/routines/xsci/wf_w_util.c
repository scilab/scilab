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
#include "wf_w_drawprim.h"
#include "wf_w_util.h"
#include "wf_w_setup.h"

#include "../machine.h"
#include "All-extern.h"

/* manually flush out X events */

void app_flush()
{
    while (XtAppPending(app_con)) {
	XEvent		event;

	/* pass events to ensure we are completely initialised */
	XtAppNextEvent(app_con, &event);
	XtDispatchEvent(&event);
    }
}






