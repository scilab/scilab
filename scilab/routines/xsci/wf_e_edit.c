/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 * Change function implemented by Frank Schmuck (schmuck@svax.cs.cornell.edu)
 * X version by Jon Tombs <jon@uk.ac.oxford.robots>
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
#include "wf_mode.h"
#include "wf_w_drawprim.h"
#include "wf_w_util.h"

#include <string.h> /* in case of dmalloc */ 

#ifdef __STDC__ 
#include <stdlib.h>
#else
#include <malloc.h>
#endif

DeclareArgs(1);

int panel_set_value(widg, val)
     Widget	    widg;
     char	   *val;
{
    FirstArg(XtNstring, val);
    /* I don't know why this doesn't work? */
    /* NextArg(XtNinsertPosition, strlen(val)); */
    SetValues(widg);
    XawTextSetInsertionPoint(widg, (XawTextPosition) strlen(val));
    return(0);
}

char           *
panel_get_value(widg)
    Widget          widg;
{
    char           *val;

    FirstArg(XtNstring, &val);
    GetValues(widg);
    return val;

}

void clear_text_key(w)
Widget w;
{
	panel_set_value(w, "");
}

static void get_clipboard();

void paste_panel_key(w, event)
Widget w;
XKeyEvent *event;
{
	Time event_time;

        event_time = event->time;
        XtGetSelectionValue(w, XA_PRIMARY, XA_STRING, get_clipboard, w, event_time);
}


static void get_clipboard(w, client_data, selection, type, buf, length, format)
Widget w;
XtPointer client_data;
Atom *selection;
Atom *type;
XtPointer buf;
unsigned long *length;
int *format;
{
        char *c, *p;
        int i;
        char s[256];

        strcpy (s, panel_get_value((Widget)client_data));
        p = strchr(s, '\0');
        c = buf;
        for (i=0; i<*length; i++) {
                if (*c=='\0' || *c=='\n' || *c=='\r' || strlen(s)>=sizeof(s)-1)
                        break;
                *p = *c;
                p++;
                *p = '\0';
                c++;
        }
        XtFree(buf);
        panel_set_value((Widget)client_data, s);
}




