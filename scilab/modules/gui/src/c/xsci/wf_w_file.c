#define FSIZE 1024
char     cur_dir[FSIZE];
char     cur_filename[FSIZE];

/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1991 by Brian V. Smith
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
#include "wf_w_drawprim.h" 	/* for char_height */
#include "wf_w_dir.h"
#include "wf_w_util.h"
#include "wf_w_setup.h"

#ifdef __STDC__ 
#include <stdlib.h> 
#else 
#include <malloc.h>
#endif 

#include "../machine.h"
#include "All-extern.h"
#include "../menusX/men_scilab.h"

extern Boolean	file_msg_is_popped;
extern Widget	file_msg_popup;
Boolean	warnexist=True;

static void file_panel_dismiss  __PARAMS((void));  
static void file_panel_cancel  __PARAMS((Widget w, XButtonEvent *ev));  

/* don't allow newlines in text until we handle multiple line texts */
String		text_translations =
	"<Key>Return: no-op(RingBell)\n\
	Ctrl<Key>J: no-op(RingBell)\n\
	Ctrl<Key>M: no-op(RingBell)\n\
	Ctrl<Key>X: EmptyTextKey()\n\
	Ctrl<Key>U: multiply(4)\n\
	<Key>F18: PastePanelKey()\n";

DeclareStaticArgs(12);

static Widget	cfile_lab, cfile_text;
static Widget	cancel,ok, exec,load,getf,getfc,linkf;
static Widget	file_w;
static Position xposn, yposn;
static String	file_name_translations =
	"<Key>Return: exec()\n";

static void	file_panel_cancel();
void		do_ok(),do_exec(), do_load(),do_getf(),do_getfc(),do_linkf();
static XtActionsRec	file_name_actions[] =
{
    {"exec", (XtActionProc) do_exec},
    {"ok", (XtActionProc) do_ok},
};
static String	file_translations =
	"<Message>WM_PROTOCOLS: DismissFile()\n";
static XtActionsRec	file_actions[] =
{
    {"DismissFile", (XtActionProc) file_panel_cancel},
    {"cancel", (XtActionProc) file_panel_cancel},
    {"ok", (XtActionProc) do_ok},
    {"exec", (XtActionProc) do_exec},
    {"load", (XtActionProc) do_load},
    {"linkf", (XtActionProc) do_linkf},
    {"getf", (XtActionProc) do_getf},
    {"getfc", (XtActionProc) do_getfc}
};

/* Global so w_dir.c can access us */

Widget		file_panel,	/* so w_dir can access the scrollbars */
		file_popup,	/* the popup itself */
		file_selfile,	/* selected file widget */
		file_mask,	/* mask widget */
		file_dir,	/* current directory widget */
		file_flist,	/* file list widget */
		file_dlist,	/* dir list widget */
                labelW;         /* popup label */

static void
file_panel_dismiss()
{
    FirstArg(XtNstring, "\0");
    SetValues(file_selfile);	/* clear Filename string */
    XtPopdown(file_popup);
    XtSetSensitive(file_w, True);
}


#define STRW1 ";exec(\"%s/%s\");\n"

int
exec_file(dir,file)
    char           *file;
     char *dir;
{
    char strsend1[sizeof(STRW1)+FSIZE];
    sprintf(strsend1,STRW1,dir,file);
    write_scilab(strsend1);
    return (0);
}

void
do_exec(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	
	/* check the ascii widget for a filename */
	if (emptyname(fval))
	  fval = cur_filename;	
	/* "Filename" widget empty, use current filename */

	if (emptyname_msg(fval, "EXEC"))
	    return;

	if (change_directory(dval) == 0) {
	    if (exec_file(dval,fval) == 0) {
		FirstArg(XtNlabel, fval);
		SetValues(cfile_text);		/* set the current filename */
		if (fval != cur_filename)
			update_cur_filename(fval);	/* and update cur_filename */
		file_panel_dismiss();
	    }
	}
    } else {
	(void) exec_file("",cur_filename);
    }
}


/***********************************
 * Used with the primitive C2F(xgetfile)
 ***********************************/

void ok_file(dir,file)
     char           *file;
     char *dir;
{
  write_getfile(dir,file);
}

void ok_end()
{
  /* Accelerators to their default values */
  XtOverrideTranslations(file_selfile,
			 XtParseTranslationTable(file_name_translations));
  XtOverrideTranslations(file_flist,
			 XtParseTranslationTable(file_name_translations));
  XtSetSensitive(ok,False);
  XtSetSensitive(exec, True);
  XtSetSensitive(load, True);
  XtSetSensitive(getf, True);
  /**  XtSetSensitive(getfc, True); **/
  XtSetSensitive(linkf, True);
}

static String	file_name_translations1 =
	"<Key>Return: ok()\n";

void ok_prep(filemask,dirname,title,flag,err)
     char *filemask;
     char *dirname;
     char *title;
     int flag,*err;
{
  char newdir[PATH_MAX];
  if (file_popup) {
    /*    FirstArg(XtNtitle, title);
	  SetValues(file_popup);	*/
    FirstArg(XtNlabel, title);
    SetValues(labelW);
  }

  /* Change Accelerators to ok */
  XtOverrideTranslations(file_selfile,
			 XtParseTranslationTable(file_name_translations1));
  XtOverrideTranslations(file_flist,
			 XtParseTranslationTable(file_name_translations1));
  FirstArg(XtNstring, filemask);
  SetValues(file_mask);
  /** callingRescan because of file_mask change **/
  Rescan((Widget) 0, (XEvent*) 0, (String*) 0, (Cardinal*) 0);
  if ( flag == 1 && ((int) strlen(dirname)) >= 1 )
    {
      /* We also want to change dir */
      char longdir[PATH_MAX];
      if (dirname[0]=='~')
	{
	  parseuserpath(dirname,longdir);
	  strcpy(newdir, longdir);
	}
      else if ( strncmp(dirname,"SCI",3) == 0) 
	{
	  parsescipath(dirname,longdir);
	  strcpy(newdir, longdir);
	}
      else 
	{
	  strcpy(newdir,dirname);
	}
      DoChangeDir(newdir); 
    }
  XtSetSensitive(ok,True);
  XtSetSensitive(exec, False);
  XtSetSensitive(load, False);
  XtSetSensitive(getf, False);
  /** XtSetSensitive(getfc, False); **/
  XtSetSensitive(linkf, False);
}

/* make the full path from SCI/partialpath */

void parsescipath(path,longpath)
     char *path,*longpath;
{
  strcpy(longpath,getenv("SCI"));
  if (strlen(path)==3)		/* nothing after the SCI, we have the full path */
    return;
  strcat(longpath,&path[3]);	/* append the rest of the path */
  return;
}

void
do_ok(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	
	/* check the ascii widget for a filename */
	if (emptyname(fval))
	  fval = cur_filename;	
	/* "Filename" widget empty, use current filename */

	if (emptyname_msg(fval, "OK"))
	    return;

	if (change_directory(dval) == 0) 
	  {
	    ok_file(dval,fval);
	    /** if (ok_file(dval,fval) == 0) { **/
	      FirstArg(XtNlabel, fval);
	      SetValues(cfile_text);		/* set the current filename */
	      if (fval != cur_filename)
		update_cur_filename(fval);	/* and update cur_filename */
	      file_panel_dismiss();
	    /* } */
	}
    } else {
	(void) ok_file("",cur_filename);
    }
}


#define STRW3 ";getf(\"%s/%s\",\"c\");\n"

int
getfc_file(dir,file)
     char  *file;
     char *dir;
{
    char strsend1[sizeof(STRW3)+FSIZE];
    sprintf(strsend1,STRW3,dir,file);
    write_scilab(strsend1);
    return (0);
}

void
do_getfc(w)
    Widget	    w;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	/* check the ascii widget for a filename */
	if (emptyname(fval)) {
	    fval = cur_filename;	/* "Filename" widget empty, use current filename */
	    warnexist = False;		/* don't warn if this file exists */
	/* copy the name from the file_name widget to the current filename */
	} else
	    {
	    warnexist = True;			/* warn if this file exists */
	    }

	if (emptyname_msg(fval, "Getfc"))
	    return;

	/* get the directory from the ascii widget */
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);

	if (change_directory(dval) == 0) {
	    XtSetSensitive(getfc, False);
	    if (getfc_file(dval,fval) == 0) {
		FirstArg(XtNlabel, fval);
		SetValues(cfile_text);
		if (strcmp(fval,cur_filename) != 0) {
		    update_cur_filename(fval);	/* update cur_filename */
		}
		file_panel_dismiss();
	    }
	    XtSetSensitive(getfc, True);
	}
    } else {
	/* not using popup filename not changed so ok to write existing file */
	(void) getfc_file("",cur_filename);
	warnexist = False;	

    }
}


#define STRL ";link(\"%s/%s\",\"%s\");\n"

int
linkf_file(dir,file)
    char           *file;
     char *dir;
{
  char *file1;
  char strsend1[sizeof(STRL)+FSIZE];
  file1=malloc((unsigned)  (strlen(file)+1) *sizeof(char));
  if (file1 != (char*) 0)
    {
      strcpy(file1,file);
      file1[strlen(file)-2]='\0';
      sprintf(strsend1,STRL,dir,file,file1);
      free(file1);
    }
  else
    { sprintf(strsend1,STRL,dir,file,file);}
  write_scilab(strsend1);
  return (0);
}

void
do_linkf(w)
    Widget	    w;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	/* check the ascii widget for a filename */
	if (emptyname(fval)) {
	    fval = cur_filename;	/* "Filename" widget empty, use current filename */
	    warnexist = False;		/* don't warn if this file exists */
	/* copy the name from the file_name widget to the current filename */
	} else
	    {
	    warnexist = True;			/* warn if this file exists */
	    }

	if (emptyname_msg(fval, "Link"))
	    return;

	/* get the directory from the ascii widget */
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);

	if (change_directory(dval) == 0) {
	    XtSetSensitive(linkf, False);
	    if (linkf_file(dval,fval) == 0) {
		FirstArg(XtNlabel, fval);
		SetValues(cfile_text);
		if (strcmp(fval,cur_filename) != 0) {
		    update_cur_filename(fval);	/* update cur_filename */
		}
		file_panel_dismiss();
	    }
	    XtSetSensitive(linkf, True);
	}
    } else {
	/* not using popup filename not changed so ok to write existing file */
	(void) linkf_file("",cur_filename);
	warnexist = False;	

    }
}

#define STRW5 ";getf(\"%s/%s\");\n"

int
getf_file(dir,file)
     char  *file;
     char *dir;
{
    char strsend1[sizeof(STRW5)+FSIZE];
    sprintf(strsend1,STRW5,dir,file);
    write_scilab(strsend1);
    return (0);
}

void
do_getf(w)
    Widget	    w;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	/* check the ascii widget for a filename */
	if (emptyname(fval)) {
	    fval = cur_filename;	/* "Filename" widget empty, use current filename */
	    warnexist = False;		/* don't warn if this file exists */
	/* copy the name from the file_name widget to the current filename */
	} else
	    {
	    warnexist = True;			/* warn if this file exists */
	    }

	if (emptyname_msg(fval, "Getf"))
	    return;

	/* get the directory from the ascii widget */
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);

	if (change_directory(dval) == 0) {
	    XtSetSensitive(getf, False);
	    if (getf_file(dval,fval) == 0) {
		FirstArg(XtNlabel, fval);
		SetValues(cfile_text);
		if (strcmp(fval,cur_filename) != 0) {
		    update_cur_filename(fval);	/* update cur_filename */
		}
		file_panel_dismiss();
	    }
	    XtSetSensitive(getf, True);
	}
    } else {
	/* not using popup filename not changed so ok to write existing file */
	(void) getf_file("",cur_filename);
	warnexist = False;	

    }
}

#define STRW6 ";load(\"%s/%s\");\n"

int
load_file(dir,file)
     char  *file;
     char *dir;
{
    char strsend1[sizeof(STRW6)+FSIZE];
    sprintf(strsend1,STRW6,dir,file);
    write_scilab(strsend1);
    return (0);
}

void
do_load(w)
    Widget	    w;
{
    char	   *fval, *dval;

    if (file_popup) {
	FirstArg(XtNstring, &fval);
	GetValues(file_selfile);	/* check the ascii widget for a filename */
	if (emptyname(fval)) {
	    fval = cur_filename;	/* "Filename" widget empty, use current filename */
	    warnexist = False;		/* don't warn if this file exists */
	/* copy the name from the file_name widget to the current filename */
	} else
	    {
	    warnexist = True;			/* warn if this file exists */
	    }

	if (emptyname_msg(fval, "Load"))
	    return;

	/* get the directory from the ascii widget */
	FirstArg(XtNstring, &dval);
	GetValues(file_dir);

	if (change_directory(dval) == 0) {
	    XtSetSensitive(load, False);
	    if (load_file(dval,fval) == 0) {
		FirstArg(XtNlabel, fval);
		SetValues(cfile_text);
		if (strcmp(fval,cur_filename) != 0) {
		    update_cur_filename(fval);	/* update cur_filename */
		}
		file_panel_dismiss();
	    }
	    XtSetSensitive(load, True);
	}
    } else {
	/* not using popup filename not changed so ok to write existing file */
	(void) load_file("",cur_filename);
	warnexist = False;	

    }
}

static void
file_panel_cancel(w, ev)
    Widget	    w;
    XButtonEvent   *ev;
{
  cancel_getfile();/* for getfile.c */
  file_panel_dismiss();
}



void popup_file_panel(w,description)
    Widget	    w;
     char* description;
{
    extern Atom     wm_delete_window;

    w_init(w);
    set_temp_wf_cursor(wait_wf_cursor);
    XtSetSensitive(w, False);

    if (!file_popup)
	create_file_panel(w,description);
    else{
	Rescan((Widget) 0, (XEvent*) 0, (String*) 0, (Cardinal*) 0);
	FirstArg(XtNlabel, description);
	SetValues(labelW);
    }
    XtPopup(file_popup, XtGrabNonexclusive);
    (void) XSetWMProtocols(XtDisplay(file_popup), XtWindow(file_popup),
			   &wm_delete_window, 1);
    if (file_msg_is_popped)
	XtAddGrab(file_msg_popup, False, False);
    reset_wf_cursor();
}

void create_file_panel(w,description)
     Widget		   w;
     char *description;
    {
	int ch;
	Widget		   file, beside, below;
        Widget             hpaned,dialogpanned,viewportW/*,labelW*/;
        XFontStruct     *temp_font;
	static int	   actions_added=0;
	file_w = w;
	XtTranslateCoords(w, (Position) 0, (Position) 0, &xposn, &yposn);
	
	FirstArg(XtNx, xposn);
	NextArg(XtNy, yposn + 50);
	NextArg(XtNtitle, " File menu");
	file_popup = XtCreatePopupShell("xsci_file_menu",
					transientShellWidgetClass,
					w_toplevel, Args, ArgCount);
	XtOverrideTranslations(file_popup,
			       XtParseTranslationTable(file_translations));
	
        hpaned = XtCreateManagedWidget("hpaned",panedWidgetClass,file_popup,
				       (Arg *) NULL,(Cardinal)ZERO);
	
        dialogpanned = XtCreateManagedWidget("paned",panedWidgetClass,hpaned,
					     (Arg *) NULL,(Cardinal)ZERO);

	viewportW = XtCreateManagedWidget("labelviewport",viewportWidgetClass,
					  dialogpanned,(Arg *) NULL,(Cardinal)ZERO);
	FirstArg(XtNlabel, description);
	labelW=XtCreateManagedWidget("label",labelWidgetClass,viewportW,Args, ArgCount);


	file_panel = XtCreateManagedWidget("file_panel", formWidgetClass,
					   dialogpanned, (Arg *) NULL, (Cardinal) 0);

	FirstArg(XtNlabel, " Current Filename:");
	NextArg(XtNvertDistance, 15);
	NextArg(XtNjustify, XtJustifyLeft);
	NextArg(XtNborderWidth, 0);
	cfile_lab = XtCreateManagedWidget("cur_file_label", labelWidgetClass,
					  file_panel, Args, ArgCount);

	FirstArg(XtNlabel, cur_filename);
	NextArg(XtNfromHoriz, cfile_lab);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNjustify, XtJustifyLeft);
	NextArg(XtNborderWidth, 0);
	NextArg(XtNwidth, 250);
	cfile_text = XtCreateManagedWidget("cur_file_name", labelWidgetClass,
					   file_panel, Args, ArgCount);

	FirstArg(XtNlabel, "         Filename:");
	NextArg(XtNvertDistance, 15);
	NextArg(XtNfromVert, cfile_lab);
	NextArg(XtNborderWidth, 0);
	file = XtCreateManagedWidget("file_label", labelWidgetClass,
				     file_panel, Args, ArgCount);
	FirstArg(XtNfont, &temp_font);
	GetValues(file);

	FirstArg(XtNwidth, 350);
	NextArg(XtNheight, ch=char_height(temp_font) * 2 + 4);
	NextArg(XtNeditType, "edit");
	NextArg(XtNstring, cur_filename);
	NextArg(XtNinsertPosition, strlen(cur_filename));
	NextArg(XtNfromHoriz, file);
	NextArg(XtNborderWidth, INTERNAL_BW);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNfromVert, cfile_lab);
	NextArg(XtNscrollHorizontal, XawtextScrollWhenNeeded);
	file_selfile = XtCreateManagedWidget("file_name", asciiTextWidgetClass,
					     file_panel, Args, ArgCount);
	XtOverrideTranslations(file_selfile,
			   XtParseTranslationTable(text_translations));

	if (!actions_added) {
	    XtAppAddActions(app_con, file_actions, XtNumber(file_actions));
	    actions_added = 1;
	    /* add action to exec file */
	    XtAppAddActions(app_con, file_name_actions, XtNumber(file_name_actions));
	}

	/* make <return> in the filename window exec the file */
	XtOverrideTranslations(file_selfile,
			   XtParseTranslationTable(file_name_translations));

	create_dirinfo(file_panel, file_selfile, &beside, &below,
		       &file_mask, &file_dir, &file_flist, &file_dlist);
	/* make <return> in the file list window exec the file */
	XtOverrideTranslations(file_flist,
			   XtParseTranslationTable(file_name_translations));
	FirstArg(XtNlabel, "Cancel");
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNfromHoriz, beside);
	NextArg(XtNfromVert, below);
	NextArg(XtNborderWidth, INTERNAL_BW);
	cancel = XtCreateManagedWidget("cancel", commandWidgetClass,
				       file_panel, Args, ArgCount);
	XtAddEventHandler(cancel, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)file_panel_cancel, (XtPointer) NULL);
        FOpAddInfoHandler(cancel,"Cancel file operation");

	FirstArg(XtNlabel, "Ok");
	NextArg(XtNsensitive, False);
	NextArg(XtNfromHoriz, cancel);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNborderWidth, INTERNAL_BW);
	ok = XtCreateManagedWidget("Ok", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(ok, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_ok, (XtPointer) NULL);
        FOpAddInfoHandler(ok,"send file name to Scilab");

	FirstArg(XtNlabel, "Load");
	NextArg(XtNfromHoriz, ok);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNborderWidth, INTERNAL_BW);
	load = XtCreateManagedWidget("load", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(load, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_load, (XtPointer) NULL);
        FOpAddInfoHandler(load,"Load a scilab file (created by save)");

	FirstArg(XtNlabel, "Getf");
	NextArg(XtNfromHoriz, load);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNborderWidth, INTERNAL_BW);
	getf = XtCreateManagedWidget("getf", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(getf, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_getf, (XtPointer) NULL);
        FOpAddInfoHandler(getf,"Load scilab functions");

	/**
	FirstArg(XtNlabel, "Getfc");
	NextArg(XtNfromHoriz, getf);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNborderWidth, INTERNAL_BW);
	getfc = XtCreateManagedWidget("getfc", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(getfc, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_getfc, (XtPointer) NULL);
        FOpAddInfoHandler(getfc,"Load and compile scilab functions");
        **/
	FirstArg(XtNlabel, "Link");
	NextArg(XtNfromHoriz, getf);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	NextArg(XtNborderWidth, INTERNAL_BW);
	linkf = XtCreateManagedWidget("link", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(linkf, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_linkf, (XtPointer) NULL);
        FOpAddInfoHandler(linkf,"Dynamic link of object file");
	FirstArg(XtNlabel, "Exec");
	NextArg(XtNborderWidth, INTERNAL_BW);
	NextArg(XtNfromHoriz, linkf);
	NextArg(XtNfromVert, below);
	NextArg(XtNvertDistance, 15);
	NextArg(XtNhorizDistance, 25);
	NextArg(XtNheight, 25);
	exec = XtCreateManagedWidget("exec", commandWidgetClass,
				     file_panel, Args, ArgCount);
	XtAddEventHandler(exec, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler)do_exec, (XtPointer) NULL);
        FOpAddInfoHandler(exec,"Execute the content of file (scilab code)");
	init_msg(file_panel,exec,ch," ");
	XtInstallAccelerators(file_panel, cancel);
	XtInstallAccelerators(file_panel, exec);
}

 



