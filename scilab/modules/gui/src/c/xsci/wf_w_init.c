/* Copyright (C) 1998 Chancelier Jean-Philippe */
#include "wf_fig.h"
#include "wf_figx.h"
#include "wf_resources.h"

#include "../machine.h"
#include "All-extern.h"

Display	       *tool_d;
Screen	       *tool_s;
int		tool_sn;
Window          tool_win;

extern void clear_text_key();
extern void paste_panel_key();
static XtActionsRec text_panel_actions[] =
{
    {"PastePanelKey", (XtActionProc) paste_panel_key} ,
    {"EmptyTextKey", (XtActionProc) clear_text_key} ,
};


Widget   w_toplevel;

void w_init(w)
     Widget w;
{
  w_toplevel=w;
  XtAppAddActions(app_con, text_panel_actions, XtNumber(text_panel_actions));
  tool_d = XtDisplay(w);
  tool_s = XtScreen(w);
  tool_sn = DefaultScreen(tool_d);
  tool_win = XtWindow(w);
  init_wf_cursor();
  setup_sizes(200,300);
}

