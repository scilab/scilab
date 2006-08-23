/* $XConsortium: TextPop.c,v 1.31 94/04/17 20:13:10 kaleb Exp $ */

/*

Copyright (c) 1989, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

/************************************************************
 *
 * This file is broken up into three sections one dealing with
 * each of the three popups created here:
 *
 * FileInsert, Search, and Replace.
 *
 * There is also a section at the end for utility functions 
 * used by all more than one of these dialogs.
 *
 * The following functions are the only non-static ones defined
 * in this module.  They are located at the begining of the
 * section that contains this dialog box that uses them.
 * 
 * void _XawTextInsertFileAction(w, event, params, num_params);
 * void _XawTextDoSearchAction(w, event, params, num_params);
 * void _XawTextDoReplaceAction(w, event, params, num_params);
 * void _XawTextInsertFile(w, event, params, num_params);
 *
 *************************************************************/

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h> 

#include <X11/Xaw3d/TextP.h>
#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xmu/CharSet.h>
#include "XawI18n.h"
#include <stdio.h>
#include <X11/Xos.h>		/* for O_RDONLY */
#include <errno.h>

#ifdef X_NOT_STDC_ENV
extern int errno;
#endif

#define INSERT_FILE ("Enter Filename:")

#define SEARCH_LABEL_1  ("Use <Tab> to change fields.")
#define SEARCH_LABEL_2  ("Use ^q<Tab> for <Tab>.")
#define DISMISS_NAME  ("cancel")
#define DISMISS_NAME_LEN 6
#define FORM_NAME     ("form")
#define LABEL_NAME    ("label")
#define TEXT_NAME     ("text")

#define R_OFFSET      1

static void CenterWidgetOnPoint(), PopdownSearch(), DoInsert(), _SetField();
static void InitializeSearchWidget(), SetResource(), SetSearchLabels();
static void DoReplaceOne(), DoReplaceAll();
static Widget CreateDialog(), GetShell();
static void SetWMProtocolTranslations();
static Boolean DoSearch(), SetResourceByName(), Replace();
static String GetString();

static String GetStringRaw();

static void AddInsertFileChildren();
static Boolean InsertFileNamed();
static void AddSearchChildren();

static char radio_trans_string[] =
    "<Btn1Down>,<Btn1Up>:   set() notify()";

static char search_text_trans[] = 
  "~Shift<Key>Return:      DoSearchAction(Popdown) \n\
   Shift<Key>Return:       DoSearchAction() SetField(Replace) \n\
   Ctrl<Key>q,<Key>Tab:    insert-char()    \n\
   Ctrl<Key>c:             PopdownSearchAction() \n\
   <Btn1Down>:             select-start() SetField(Search) \n\
   <Key>Tab:               DoSearchAction() SetField(Replace)";

static char rep_text_trans[] = 
  "~Shift<Key>Return:      DoReplaceAction(Popdown) \n\
   Shift<Key>Return:       SetField(Search) \n\
   Ctrl<Key>q,<Key>Tab:    insert-char()     \n\
   Ctrl<Key>c:             PopdownSearchAction() \n\
   <Btn1Down>:             select-start() DoSearchAction() SetField(Replace)\n\
   <Key>Tab:               SetField(Search)";

/************************************************************
 * 
 * This section of the file contains all the functions that 
 * the file insert dialog box uses.
 *
 ************************************************************/

/*	Function Name: _XawTextInsertFileAction
 *	Description: Action routine that can be bound to dialog box's 
 *                   Text Widget that will insert a file into the main
 *                   Text Widget.
 *	Arguments:   (Standard Action Routine args) 
 *	Returns:     none.
 */

/* ARGSUSED */
void 
_XawTextInsertFileAction(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  DoInsert(w, (XtPointer) XtParent(XtParent(XtParent(w))), (XtPointer)NULL);
}

/*	Function Name: _XawTextInsertFile
 *	Description: Action routine that can be bound to the text widget
 *                   it will popup the insert file dialog box.
 *	Arguments:   w - the text widget.
 *                   event - X Event (used to get x and y location).
 *                   params, num_params - the parameter list.
 *	Returns:     none.
 *
 * NOTE:
 *
 * The parameter list may contain one entry.
 *
 *  Entry:  This entry is optional and contains the value of the default
 *          file to insert.
 */

void 
_XawTextInsertFile(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget)w;
  char * ptr;
  XawTextEditType edit_mode;
  Arg args[1];

  XtSetArg(args[0], XtNeditType,&edit_mode);
  XtGetValues(ctx->text.source, args, ONE);
  
  if (edit_mode != XawtextEdit) {
    XBell(XtDisplay(w), 0);
    return;
  }

  if (*num_params == 0) 
    ptr = "";
  else 
    ptr = params[0];
    
  if (!ctx->text.file_insert) {
    ctx->text.file_insert = CreateDialog(w, ptr, "insertFile",
					 AddInsertFileChildren);
    XtRealizeWidget(ctx->text.file_insert);
    SetWMProtocolTranslations(ctx->text.file_insert);
  }

  CenterWidgetOnPoint(ctx->text.file_insert, event);
  XtPopup(ctx->text.file_insert, XtGrabNone);
}

/*	Function Name: PopdownFileInsert
 *	Description: Pops down the file insert button.
 *	Arguments: w - the widget that caused this action.
 *                 closure - a pointer to the main text widget that
 *                           popped up this dialog.
 *                 call_data - *** NOT USED ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
PopdownFileInsert(w, closure, call_data)
Widget w;			/* The Dialog Button Pressed. */
XtPointer closure;		/* Text Widget. */
XtPointer call_data;		/* unused */
{
  TextWidget ctx = (TextWidget) closure;

  XtPopdown( ctx->text.file_insert );
  (void) SetResourceByName( ctx->text.file_insert, LABEL_NAME, 
			   XtNlabel, (XtArgVal) INSERT_FILE);
}

/*	Function Name: DoInsert
 *	Description: Actually insert the file named in the text widget
 *                   of the file dialog.
 *	Arguments:   w - the widget that activated this callback.
 *                   closure - a pointer to the text widget to insert the
 *                             file into.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
DoInsert(w, closure, call_data)
Widget w;			/* The Dialog Button Pressed. */
XtPointer closure;		/* Text Widget */
XtPointer call_data;		/* unused */
{
  TextWidget ctx = (TextWidget) closure;
  char buf[BUFSIZ], msg[BUFSIZ];
  Widget temp_widget;

  (void) sprintf(buf, "%s.%s", FORM_NAME, TEXT_NAME);
  if ( (temp_widget = XtNameToWidget(ctx->text.file_insert, buf)) == NULL ) {
    (void) strcpy(msg, 
	   "*** Error: Could not get text widget from file insert popup");
  }
  else 
    if (InsertFileNamed( (Widget) ctx, GetString( temp_widget ))) {
      PopdownFileInsert(w, closure, call_data);
      return;
    }
    else
      (void) sprintf( msg, "*** Error: %s ***", strerror(errno));

  (void)SetResourceByName(ctx->text.file_insert, 
			  LABEL_NAME, XtNlabel, (XtArgVal) msg);
  XBell(XtDisplay(w), 0);
}

/*	Function Name: InsertFileNamed
 *	Description: Inserts a file into the text widget.
 *	Arguments: tw - The text widget to insert this file into.
 *                 str - name of the file to insert.
 *	Returns: TRUE if the insert was sucessful, FALSE otherwise.
 */


static Boolean
InsertFileNamed(tw, str)
Widget tw;
char *str;
{
  FILE *file;
  XawTextBlock text;
  XawTextPosition pos;

  if ( (str == NULL) || (strlen(str) == 0) || 
       ((file = fopen(str, "r")) == NULL))
    return(FALSE);

  pos = XawTextGetInsertionPoint(tw);

  fseek(file, 0L, 2);


  text.firstPos = 0;
  text.length = (ftell(file))/sizeof(unsigned char);
  text.ptr = XtMalloc((text.length + 1) * sizeof(unsigned char));
  text.format = XawFmt8Bit;

  fseek(file, 0L, 0);
  if (fread(text.ptr, sizeof(unsigned char), text.length, file) != text.length)
      XtErrorMsg("readError", "insertFileNamed", "XawError",
                 "fread returned error.", NULL, NULL);

 /* DELETE if (text.format == XawFmtWide) {
     wchar_t* _XawTextMBToWC();
     wchar_t* wstr;
     wstr = _XawTextMBToWC(XtDisplay(tw), text.ptr, &(text.length));
     wstr[text.length] = NULL;
     XtFree(text.ptr);
     text.ptr = (char *)wstr;
  } else {
     (text.ptr)[text.length] = '\0';
  }*/

  if (XawTextReplace(tw, pos, pos, &text) != XawEditDone) {
     XtFree(text.ptr);
     fclose(file);
     return(FALSE);
  }
  pos += text.length;
  XtFree(text.ptr);
  fclose(file);
  XawTextSetInsertionPoint(tw, pos);
  return(TRUE);
}


/*	Function Name: AddInsertFileChildren
 *	Description: Adds all children to the InsertFile dialog widget.
 *	Arguments: form - the form widget for the insert dialog widget.
 *                 ptr - a pointer to the initial string for the Text Widget.
 *                 tw - the main text widget.
 *	Returns: none
 */

static void
AddInsertFileChildren(form, ptr, tw)
Widget form, tw;
char * ptr;
{
  Arg args[10];
  Cardinal num_args;
  Widget label, text, cancel, insert;
  XtTranslations trans;

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, INSERT_FILE);num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE ); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, 0 ); num_args++;
  label = XtCreateManagedWidget (LABEL_NAME, labelWidgetClass, form, 
				 args, num_args);
  
  num_args = 0;
  XtSetArg(args[num_args], XtNfromVert, label); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainRight); num_args++;
  XtSetArg(args[num_args], XtNeditType, XawtextEdit); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE); num_args++;
  XtSetArg(args[num_args], XtNresize, XawtextResizeWidth); num_args++;
  XtSetArg(args[num_args], XtNstring, ptr); num_args++;
  text = XtCreateManagedWidget(TEXT_NAME, asciiTextWidgetClass, form,
				args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Insert File"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, text); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  insert = XtCreateManagedWidget("insert", commandWidgetClass, form,
				 args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Cancel"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, text); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, insert); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  cancel = XtCreateManagedWidget(DISMISS_NAME, commandWidgetClass, form,
				 args, num_args);

  XtAddCallback(cancel, XtNcallback, PopdownFileInsert, (XtPointer) tw);
  XtAddCallback(insert, XtNcallback, DoInsert, (XtPointer) tw);

  XtSetKeyboardFocus(form, text);

/*
 * Bind <CR> to insert file.
 */

  trans = XtParseTranslationTable("<Key>Return: InsertFileAction()");
  XtOverrideTranslations(text, trans);

}

/************************************************************
 * 
 * This section of the file contains all the functions that 
 * the search dialog box uses.
 *
 ************************************************************/

/*	Function Name: _XawTextDoSearchAction
 *	Description: Action routine that can be bound to dialog box's 
 *                   Text Widget that will search for a string in the main 
 *                   Text Widget.
 *	Arguments:   (Standard Action Routine args) 
 *	Returns:     none.
 *
 * Note:
 * 
 * If the search was sucessful and the argument popdown is passed to
 * this action routine then the widget will automatically popdown the 
 * search widget.
 */

/* ARGSUSED */
void 
_XawTextDoSearchAction(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget tw = (TextWidget) XtParent(XtParent(XtParent(w)));
  Boolean popdown = FALSE;

  if ( (*num_params == 1) &&
       ((params[0][0] == 'p') || (params[0][0] == 'P')) )
      popdown = TRUE;
    
  if (DoSearch(tw->text.search) && popdown)
    PopdownSearch(w, (XtPointer) tw->text.search, (XtPointer)NULL);
}

/*	Function Name: _XawTextPopdownSearchAction
 *	Description: Action routine that can be bound to dialog box's 
 *                   Text Widget that will popdown the search widget.
 *	Arguments:   (Standard Action Routine args) 
 *	Returns:     none.
 */

/* ARGSUSED */
void 
_XawTextPopdownSearchAction(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget tw = (TextWidget) XtParent(XtParent(XtParent(w)));

  PopdownSearch(w, (XtPointer) tw->text.search, (XtPointer)NULL);
}

/*	Function Name: PopdownSeach
 *	Description: Pops down the search widget and resets it.
 *	Arguments: w - *** NOT USED ***.
 *                 closure - a pointer to the search structure.
 *                 call_data - *** NOT USED ***.
 *	Returns: none
 */

/* ARGSUSED */
static void 
PopdownSearch(w, closure, call_data)
Widget w;			
XtPointer closure;		
XtPointer call_data;		
{
  struct SearchAndReplace * search = (struct SearchAndReplace *) closure;

  XtPopdown( search->search_popup );
  SetSearchLabels(search, SEARCH_LABEL_1, SEARCH_LABEL_2, FALSE);
}

/*	Function Name: SearchButton
 *	Description: Performs a search when the button is clicked.
 *	Arguments: w - *** NOT USED **.
 *                 closure - a pointer to the search info.
 *                 call_data - *** NOT USED ***.
 *	Returns: 
 */

/* ARGSUSED */
static void 
SearchButton(w, closure, call_data)
Widget w;			
XtPointer closure;		
XtPointer call_data;
{
  (void) DoSearch( (struct SearchAndReplace *) closure );
}

/*	Function Name: _XawTextSearch
 *	Description: Action routine that can be bound to the text widget
 *                   it will popup the search dialog box.
 *	Arguments:   w - the text widget.
 *                   event - X Event (used to get x and y location).
 *                   params, num_params - the parameter list.
 *	Returns:     none.
 *
 * NOTE:
 *
 * The parameter list contains one or two entries that may be the following.
 *
 * First Entry:   The first entry is the direction to search by default.
 *                This arguement must be specified and may have a value of
 *                "left" or "right".
 *
 * Second Entry:  This entry is optional and contains the value of the default
 *                string to search for.
 */

#define SEARCH_HEADER ("Text Widget - Search():")

void 
_XawTextSearch(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget)w;
  XawTextScanDirection dir;
  char * ptr, buf[BUFSIZ];
  XawTextEditType edit_mode;
  Arg args[1];

#ifdef notdef
  if (ctx->text.source->Search == NULL) {
      XBell(XtDisplay(w), 0);
      return;
  }
#endif

  if ( (*num_params < 1) || (*num_params > 2) ) {
    (void) sprintf(buf, "%s %s\n%s", SEARCH_HEADER, 
	    "This action must have only", 
	    "one or two parameters");
    XtAppWarning(XtWidgetToApplicationContext(w), buf);
    return;
  }

  if (*num_params == 2 )
      ptr = params[1];
  else
      if (_XawTextFormat(ctx) == XawFmtWide) {
          /*This just does the equivalent of ptr = ""L, a waste because params[1] isnt W aligned.*/
          ptr = (char *)XtMalloc(sizeof(wchar_t));
          *((wchar_t*)ptr) = (wchar_t)0;
      } else
          ptr = "";

  switch(params[0][0]) {
  case 'b':			/* Left. */
  case 'B':
    dir = XawsdLeft;
    break;
  case 'f':			/* Right. */
  case 'F':
    dir = XawsdRight;
    break;
  default:
    (void) sprintf(buf, "%s %s\n%s", SEARCH_HEADER, 
	    "The first parameter must be",
	    "Either 'backward' or 'forward'");
    XtAppWarning(XtWidgetToApplicationContext(w), buf);
    return;
  }

  if (ctx->text.search== NULL) {
    ctx->text.search = XtNew(struct SearchAndReplace);
    ctx->text.search->search_popup = CreateDialog(w, ptr, "search",
						  AddSearchChildren);
    XtRealizeWidget(ctx->text.search->search_popup);
    SetWMProtocolTranslations(ctx->text.search->search_popup);
  }
  else if (*num_params > 1) {
    XtVaSetValues(ctx->text.search->search_text, XtNstring, ptr, NULL);
  }

  XtSetArg(args[0], XtNeditType,&edit_mode);
  XtGetValues(ctx->text.source, args, ONE);

  InitializeSearchWidget(ctx->text.search, dir, (edit_mode == XawtextEdit));

  CenterWidgetOnPoint(ctx->text.search->search_popup, event);
  XtPopup(ctx->text.search->search_popup, XtGrabNone);
}

/*	Function Name: InitializeSearchWidget
 *	Description: This function initializes the search widget and
 *                   is called each time the search widget is poped up.
 *	Arguments: search - the search widget structure.
 *                 dir - direction to search. 
 *                 replace_active - state of the sensitivity for the 
 *                                  replace button.
 *	Returns: none.
 */

static void
InitializeSearchWidget(search, dir, replace_active)
struct SearchAndReplace * search;
XawTextScanDirection dir;
Boolean replace_active;
{
  SetResource(search->rep_one, XtNsensitive, (XtArgVal) replace_active);
  SetResource(search->rep_all, XtNsensitive, (XtArgVal) replace_active);
  SetResource(search->rep_label, XtNsensitive, (XtArgVal) replace_active);
  SetResource(search->rep_text, XtNsensitive, (XtArgVal) replace_active);

  switch (dir) {
  case XawsdLeft:
    SetResource(search->left_toggle, XtNstate, (XtArgVal) TRUE);
    break;
  case XawsdRight:
    SetResource(search->right_toggle, XtNstate, (XtArgVal) TRUE);
    break;
  default:
    break;
  }
}  

/*	Function Name: AddSearchChildren
 *	Description: Adds all children to the Search Dialog Widget.
 *	Arguments: form - the form widget for the search widget.
 *                 ptr - a pointer to the initial string for the Text Widget.
 *                 tw - the main text widget.
 *	Returns: none.
 */

static void
AddSearchChildren(form, ptr, tw)
Widget form, tw;
char * ptr;
{
  Arg args[10];
  Cardinal num_args;
  Widget cancel, search_button, s_label, s_text, r_text;
  XtTranslations trans;
  struct SearchAndReplace * search = ((TextWidget) tw)->text.search;

  num_args = 0;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE ); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, 0 ); num_args++;
  search->label1 = XtCreateManagedWidget("label1", labelWidgetClass, form,
					 args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNfromVert, search->label1); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE ); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, 0 ); num_args++;
  search->label2 = XtCreateManagedWidget("label2", labelWidgetClass, form,
					 args, num_args);
  
/* 
 * We need to add R_OFFSET to the radio_data, because the value zero (0)
 * has special meaning.
 */

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Backward"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, search->label2); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNradioData, (XPointer) XawsdLeft + R_OFFSET);
  num_args++;
  search->left_toggle = XtCreateManagedWidget("backwards", toggleWidgetClass,
					      form, args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Forward"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, search->label2); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, search->left_toggle); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNradioGroup, search->left_toggle); num_args++;
  XtSetArg(args[num_args], XtNradioData, (XPointer) XawsdRight + R_OFFSET);
  num_args++;
  search->right_toggle = XtCreateManagedWidget("forwards", toggleWidgetClass,
					       form, args, num_args);

  {
    XtTranslations radio_translations;

    radio_translations = XtParseTranslationTable(radio_trans_string);
    XtOverrideTranslations(search->left_toggle, radio_translations);
    XtOverrideTranslations(search->right_toggle, radio_translations);
  }

  num_args = 0;
  XtSetArg(args[num_args], XtNfromVert, search->left_toggle); num_args++;
  XtSetArg(args[num_args], XtNlabel, "Search for:  ");num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, 0 ); num_args++;
  s_label = XtCreateManagedWidget("searchLabel", labelWidgetClass, form,
				  args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNfromVert, search->left_toggle); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, s_label); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainRight); num_args++;
  XtSetArg(args[num_args], XtNeditType, XawtextEdit); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE); num_args++;
  XtSetArg(args[num_args], XtNresize, XawtextResizeWidth); num_args++;
  XtSetArg(args[num_args], XtNstring, ptr); num_args++;
  s_text = XtCreateManagedWidget("searchText", asciiTextWidgetClass, form,
				 args, num_args);
  search->search_text = s_text;

  num_args = 0;
  XtSetArg(args[num_args], XtNfromVert, s_text); num_args++;
  XtSetArg(args[num_args], XtNlabel, "Replace with:");num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, 0 ); num_args++;
  search->rep_label = XtCreateManagedWidget("replaceLabel", labelWidgetClass,
					    form, args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNfromHoriz, s_label); num_args++;
  XtSetArg(args[num_args], XtNfromVert, s_text); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainRight); num_args++;
  XtSetArg(args[num_args], XtNeditType, XawtextEdit); num_args++;
  XtSetArg(args[num_args], XtNresizable, TRUE); num_args++;
  XtSetArg(args[num_args], XtNresize, XawtextResizeWidth); num_args++;
  XtSetArg(args[num_args], XtNstring, ""); num_args++;
  r_text = XtCreateManagedWidget("replaceText", asciiTextWidgetClass,
				 form, args, num_args);
  search->rep_text = r_text;
  
  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Search"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, r_text); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  search_button = XtCreateManagedWidget("search", commandWidgetClass, form,
					args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Replace"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, r_text); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, search_button); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  search->rep_one = XtCreateManagedWidget("replaceOne", commandWidgetClass,
					  form, args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Replace All"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, r_text); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, search->rep_one); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  search->rep_all = XtCreateManagedWidget("replaceAll", commandWidgetClass,
					  form, args, num_args);

  num_args = 0;
  XtSetArg(args[num_args], XtNlabel, "Cancel"); num_args++;
  XtSetArg(args[num_args], XtNfromVert, r_text); num_args++;
  XtSetArg(args[num_args], XtNfromHoriz, search->rep_all); num_args++;
  XtSetArg(args[num_args], XtNleft, XtChainLeft); num_args++;
  XtSetArg(args[num_args], XtNright, XtChainLeft); num_args++;
  cancel = XtCreateManagedWidget(DISMISS_NAME, commandWidgetClass, form,
				 args, num_args);

  XtAddCallback(search_button, XtNcallback, SearchButton, (XtPointer) search);
  XtAddCallback(search->rep_one, XtNcallback, DoReplaceOne, (XtPointer) search);
  XtAddCallback(search->rep_all, XtNcallback, DoReplaceAll, (XtPointer) search);
  XtAddCallback(cancel, XtNcallback, PopdownSearch, (XtPointer) search);

/*
 * Initialize the text entry fields.
 */

  {
    Pixel color;
    num_args = 0;
    XtSetArg(args[num_args], XtNbackground, &color); num_args++;
    XtGetValues(search->rep_text, args, num_args);
    num_args = 0;
    XtSetArg(args[num_args], XtNborderColor, color); num_args++;
    XtSetValues(search->rep_text, args, num_args);
    XtSetKeyboardFocus(form, search->search_text);
  }

  SetSearchLabels(search, SEARCH_LABEL_1, SEARCH_LABEL_2, FALSE);

/*
 * Bind Extra translations.
 */

  trans = XtParseTranslationTable(search_text_trans);
  XtOverrideTranslations(search->search_text, trans);

  trans = XtParseTranslationTable(rep_text_trans);
  XtOverrideTranslations(search->rep_text, trans);
}

/*	Function Name: DoSearch
 *	Description: Performs a search.
 *	Arguments: search - the serach structure.
 *	Returns: TRUE if sucessful.
 */

/* ARGSUSED */
static Boolean
DoSearch(search)
struct SearchAndReplace * search;
{
  char msg[BUFSIZ];
  Widget tw = XtParent(search->search_popup);
  XawTextPosition pos;
  XawTextScanDirection dir;
  XawTextBlock text;

  TextWidget ctx = (TextWidget)tw;

  text.ptr = GetStringRaw(search->search_text);
  if ((text.format = _XawTextFormat(ctx)) == XawFmtWide)
      text.length = wcslen((wchar_t*)text.ptr);
  else
      text.length = strlen(text.ptr);
  text.firstPos = 0;
  
  dir = (XawTextScanDirection)(int) ((XPointer)XawToggleGetCurrent(search->left_toggle) -
				R_OFFSET);
  
  pos = XawTextSearch( tw, dir, &text);


   /* The Raw string in find.ptr may be WC I can't use here, so I re - call 
   GetString to get a tame version. */

  if (pos == XawTextSearchError) 
    (void) sprintf( msg, "Could not find string ``%s''.", GetString( search->search_text ) );
  else {
    if (dir == XawsdRight)
      XawTextSetInsertionPoint( tw, pos + text.length);
    else
      XawTextSetInsertionPoint( tw, pos);
    
    XawTextSetSelection( tw, pos, pos + text.length);
    search->selection_changed = FALSE; /* selection is good. */
    return(TRUE);
  }
  
  XawTextUnsetSelection(tw);
  SetSearchLabels(search, msg, "", TRUE);
  return(FALSE);
}

/************************************************************
 * 
 * This section of the file contains all the functions that 
 * the replace dialog box uses.
 *
 ************************************************************/

/*	Function Name: _XawTextDoReplaceAction
 *	Description: Action routine that can be bound to dialog box's 
 *                   Text Widget that will replace a string in the main 
 *                   Text Widget.
 *	Arguments:   (Standard Action Routine args) 
 *	Returns:     none.
 */

/* ARGSUSED */
void 
_XawTextDoReplaceAction(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget) XtParent(XtParent(XtParent(w)));
  Boolean popdown = FALSE;

  if ( (*num_params == 1) &&
       ((params[0][0] == 'p') || (params[0][0] == 'P')) )
    popdown = TRUE;

  if (Replace( ctx->text.search, TRUE, popdown) && popdown)
    PopdownSearch(w, (XtPointer) ctx->text.search, (XtPointer)NULL);
}

/*	Function Name: DoReplaceOne
 *	Description:  Replaces the first instance of the string
 *                     in the search dialog's text widget
 *                    with the one in the replace dialog's text widget.
 *	Arguments: w - *** Not Used ***.
 *                 closure - a pointer to the search structure.
 *                 call_data - *** Not Used ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
DoReplaceOne(w, closure, call_data)
Widget w;			/* The Button Pressed. */
XtPointer closure;		/* Text Widget. */
XtPointer call_data;		/* unused */
{
  Replace( (struct SearchAndReplace *) closure, TRUE, FALSE);
}

/*	Function Name: DoReplaceOne
 *	Description:  Replaces every instance of the string
 *                    in the search dialog's text widget
 *                    with the one in the replace dialog's text widget.
 *	Arguments: w - *** Not Used ***.
 *                 closure - a pointer to the search structure.
 *                 call_data - *** Not Used ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
DoReplaceAll(w, closure, call_data)
Widget w;			/* The Button Pressed. */
XtPointer closure;		/* Text Widget. */
XtPointer call_data;		/* unused */
{
  Replace( (struct SearchAndReplace *) closure, FALSE, FALSE);
}

/*	Function Name: Replace
 *	Description: This is the function that does the real work of
 *                   replacing strings in the main text widget.
 *	Arguments: tw - the Text Widget to replce the string in.
 *                 once_only - If TRUE then only replace the first one found.
 *                             other replace all of them.
 *                 show_current - If true then leave the selection on the 
 *                                string that was just replaced, otherwise
 *                                move it onto the next one.
 *	Returns: none.
 */

static Boolean
Replace(search, once_only, show_current)
struct SearchAndReplace * search;
Boolean once_only, show_current;
{
  XawTextPosition pos, new_pos, end_pos;
  XawTextScanDirection dir;
  XawTextBlock find, replace;
  Widget tw = XtParent(search->search_popup);
  int count = 0;

  TextWidget ctx = (TextWidget)tw;

  find.ptr = GetStringRaw( search->search_text);
  if ((find.format = _XawTextFormat(ctx)) == XawFmtWide)
      find.length = wcslen((wchar_t*)find.ptr);
  else
      find.length = strlen(find.ptr);
  find.firstPos = 0;

  replace.ptr = GetStringRaw(search->rep_text);
  replace.firstPos = 0;
  if ((replace.format = _XawTextFormat(ctx)) == XawFmtWide)
      replace.length = wcslen((wchar_t*)replace.ptr);
  else
      replace.length = strlen(replace.ptr);
    
  dir = (XawTextScanDirection)(int) ((XPointer)XawToggleGetCurrent(search->left_toggle) -
				R_OFFSET);
  /* CONSTCOND */
  while (TRUE) {
    if (count != 0) {
      new_pos = XawTextSearch( tw, dir, &find);
      
      if ( (new_pos == XawTextSearchError) ) {
	if (count == 0) {
	  char msg[BUFSIZ];

             /* The Raw string in find.ptr may be WC I can't use here, 
		so I call GetString to get a tame version.*/

	  (void) sprintf( msg, "%s %s %s", "*** Error: Could not find string ``",
		  GetString( search->search_text ), "''. ***");
	  SetSearchLabels(search, msg, "", TRUE);
	  return(FALSE);
	}
	else
	  break;
      }
      pos = new_pos;
      end_pos = pos + find.length;
    }
    else {
      XawTextGetSelectionPos(tw, &pos, &end_pos);

      if (search->selection_changed) {
	SetSearchLabels(search, "Selection has been modified, aborting.",
			"", TRUE);
	return(FALSE);
      }
      if (pos == end_pos) 
	  return(FALSE);
    }

    if (XawTextReplace(tw, pos, end_pos, &replace) != XawEditDone) {
      char msg[BUFSIZ];
      
      (void) sprintf( msg, "'%s' with '%s'. ***", find.ptr, replace.ptr);
      SetSearchLabels(search, "*** Error while replacing", msg, TRUE);
      return(FALSE);
    }      

    if (dir == XawsdRight)
      XawTextSetInsertionPoint( tw, pos + replace.length);
    else
      XawTextSetInsertionPoint( tw, pos);

    if (once_only) 
      if (show_current)
	break;
      else {
	DoSearch(search);
	return(TRUE);
      }
    count++;
  }

  if (replace.length == 0)
    XawTextUnsetSelection(tw);
  else
    XawTextSetSelection( tw, pos, pos + replace.length);

  return(TRUE);
}

/*	Function Name: SetSearchLabels
 *	Description: Sets both the search labels, and also rings the bell
 *	Arguments: search - the search structure.
 *                 msg1, msg2 - message to put in each search label.
 *                 bell - if TRUE then ring bell.
 *	Returns: none.
 */

static void
SetSearchLabels(search, msg1, msg2, bell)
struct SearchAndReplace * search;
String msg1, msg2;
Boolean bell;
{
  (void) SetResource( search->label1, XtNlabel, (XtArgVal) msg1);
  (void) SetResource( search->label2, XtNlabel, (XtArgVal) msg2);
  if (bell) 
    XBell(XtDisplay(search->search_popup), 0);
}

/************************************************************
 * 
 * This section of the file contains utility routines used by
 * other functions in this file.
 *
 ************************************************************/


/*	Function Name: _XawTextSetField
 *	Description: Action routine that can be bound to dialog box's 
 *                   Text Widget that will send input to the field specified.
 *	Arguments:   (Standard Action Routine args) 
 *	Returns:     none.
 */

/* ARGSUSED */
void 
_XawTextSetField(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  struct SearchAndReplace * search;
  Widget new, old;

  search = ((TextWidget) XtParent(XtParent(XtParent(w))))->text.search;

  if (*num_params != 1) {
    SetSearchLabels(search, "*** Error: SetField Action must have",
		    "exactly one argument. ***", TRUE);
    return;
  }
  switch (params[0][0]) {
  case 's':
  case 'S':
    new = search->search_text;
    old = search->rep_text;
    break;
  case 'r':
  case 'R':
    old = search->search_text;
    new = search->rep_text;
    break;
  default:
    SetSearchLabels(search, "*** Error: SetField Action's first Argument must",
		    "be either 'Search' or 'Replace'. ***", TRUE);
    return;
  }
  _SetField(new, old);
}

/*	Function Name: SetField
 *	Description: Sets the current text field.
 *	Arguments: new, old - new and old text fields.
 *	Returns: none
 */

static void
_SetField(new, old)
Widget new, old;
{
  Arg args[2];
  Pixel new_border, old_border, old_bg;

  if (!XtIsSensitive(new)) {
    XBell(XtDisplay(old), 0);	/* Don't set field to an inactive Widget. */
    return;
  }

  XtSetKeyboardFocus(XtParent(new), new); 
						
  XtSetArg(args[0], XtNborderColor, &old_border);
  XtSetArg(args[1], XtNbackground, &old_bg);
  XtGetValues(new, args, TWO);

  XtSetArg(args[0], XtNborderColor, &new_border);
  XtGetValues(old, args, ONE);

  if (old_border != old_bg)	/* Colors are already correct, return. */
      return;

  SetResource(old, XtNborderColor, (XtArgVal) old_border);
  SetResource(new, XtNborderColor, (XtArgVal) new_border);
}

/*	Function Name: SetResourceByName
 *	Description: Sets a resource in any of the dialog children given
 *                   name of the child and the shell widget of the dialog.
 *	Arguments: shell - shell widget of the popup.
 *                 name - name of the child.
 *                 res_name - name of the resource.
 *                 value - the value of the resource.
 *	Returns: TRUE if sucessful.
 */

static Boolean
SetResourceByName(shell, name, res_name, value)
Widget shell;
char * name, * res_name;
XtArgVal value;
{
  Widget temp_widget;
  char buf[BUFSIZ];

  (void) sprintf(buf, "%s.%s", FORM_NAME, name);

  if ( (temp_widget = XtNameToWidget(shell, buf)) != NULL) {
    SetResource(temp_widget, res_name, value);
    return(TRUE);
  }
  return(FALSE);
}

/*	Function Name: SetResource
 *	Description: Sets a resource in a widget
 *	Arguments: w - the widget.
 *                 res_name - name of the resource.
 *                 value - the value of the resource.
 *	Returns: none.
 */

static void
SetResource(w, res_name, value)
Widget w;
char * res_name;
XtArgVal value;
{
  Arg args[1];
  
  XtSetArg(args[0], res_name, value);
  XtSetValues( w, args, ONE );
}

/*	Function Name: GetString{Raw}
 *	Description:   Gets the value for the string in the popup.
 *	Arguments:     text - the text widget whose string we will get.
 * 
 *	GetString returns the string as a MB.
 *	GetStringRaw returns the exact buffer contents suitable for a search.
 *
 */

static String
GetString(text)
Widget text;
{
  String string;
  Arg args[1];

  XtSetArg( args[0], XtNstring, &string );
  XtGetValues( text, args, ONE );
  return(string);
}

static String
GetStringRaw(tw)
Widget tw;
{
  TextWidget ctx = (TextWidget)tw;
  XawTextPosition last;
  char *_XawTextGetText();

  last = XawTextSourceScan(ctx->text.source, 0, XawstAll, XawsdRight,
			     ctx->text.mult, TRUE);
  return (_XawTextGetText(ctx, 0, last));
}

/*	Function Name: CenterWidgetOnPoint.
 *	Description: Centers a shell widget on a point relative to
 *                   the root window.
 *	Arguments: w - the shell widget.
 *                 event - event containing the location of the point
 *	Returns: none.
 *
 * NOTE: The widget is not allowed to go off the screen.
 */

static void
CenterWidgetOnPoint(w, event)
Widget w;
XEvent *event;
{
  Arg args[3];
  Cardinal num_args;
  Dimension width, height, b_width;
  Position x = 0, y = 0, max_x, max_y;
  
  if (event != NULL) {
    switch (event->type) {
    case ButtonPress:
    case ButtonRelease:
      x = event->xbutton.x_root;
      y = event->xbutton.y_root;
      break;
    case KeyPress:
    case KeyRelease:
      x = event->xkey.x_root;
      y = event->xkey.y_root;
      break;
    default:
      return;
    }
  }
  
  num_args = 0;
  XtSetArg(args[num_args], XtNwidth, &width); num_args++;
  XtSetArg(args[num_args], XtNheight, &height); num_args++;
  XtSetArg(args[num_args], XtNborderWidth, &b_width); num_args++;
  XtGetValues(w, args, num_args);

  width += 2 * b_width;
  height += 2 * b_width;

  x -= ( (Position) width/2 );
  if (x < 0) x = 0;
  if ( x > (max_x = (Position) (XtScreen(w)->width - width)) ) x = max_x;

  y -= ( (Position) height/2 );
  if (y < 0) y = 0;
  if ( y > (max_y = (Position) (XtScreen(w)->height - height)) ) y = max_y;
  
  num_args = 0;
  XtSetArg(args[num_args], XtNx, x); num_args++;
  XtSetArg(args[num_args], XtNy, y); num_args++;
  XtSetValues(w, args, num_args);
}

/*	Function Name: CreateDialog
 *	Description: Actually creates a dialog.
 *	Arguments: parent - the parent of the dialog - the main text widget.
 *                 ptr - initial_string for the dialog.
 *                 name - name of the dialog.
 *                 func - function to create the children of the dialog.
 *	Returns: the popup shell of the dialog.
 * 
 * NOTE:
 *
 * The function argument is passed the following arguements.
 *
 * form - the from widget that is the dialog.
 * ptr - the initial string for the dialog's text widget.
 * parent - the parent of the dialog - the main text widget.
 */

static Widget
CreateDialog(parent, ptr, name, func)
Widget parent;
String ptr, name;
void (*func)();
{
  Widget popup, form;
  Arg args[5];
  Cardinal num_args;

  num_args = 0;
  XtSetArg(args[num_args], XtNiconName, name); num_args++;
  XtSetArg(args[num_args], XtNgeometry, NULL); num_args++;
  XtSetArg(args[num_args], XtNallowShellResize, TRUE); num_args++;
  XtSetArg(args[num_args], XtNtransientFor, GetShell(parent)); num_args++;
  popup = XtCreatePopupShell(name, transientShellWidgetClass, 
			     parent, args, num_args);
  
  form = XtCreateManagedWidget(FORM_NAME, formWidgetClass, popup,
			       (ArgList)NULL, ZERO);
  XtManageChild (form);

  (*func) (form, ptr, parent);
  return(popup);
}

 /*	Function Name: GetShell
  * 	Description: Walks up the widget hierarchy to find the
  * 		nearest shell widget.
  * 	Arguments: w - the widget whose parent shell should be returned.
  * 	Returns: The shell widget among the ancestors of w that is the
  * 		fewest levels up in the widget hierarchy.
  */
 
static Widget
GetShell(w)
Widget w;
{
    while ((w != NULL) && !XtIsShell(w))
	w = XtParent(w);
    
    return (w);
}

static Boolean InParams(str, p, n)
    String str;
    String *p;
    Cardinal n;
{
    int i;
    for (i=0; i < n; p++, i++)
	if (! XmuCompareISOLatin1(*p, str)) return True;
    return False;
}

static char *WM_DELETE_WINDOW = "WM_DELETE_WINDOW";

static void WMProtocols(w, event, params, num_params)
    Widget w;		/* popup shell */
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    Atom wm_delete_window;
    Atom wm_protocols;

    wm_delete_window = XInternAtom(XtDisplay(w), WM_DELETE_WINDOW, True);
    wm_protocols = XInternAtom(XtDisplay(w), "WM_PROTOCOLS", True);

    /* Respond to a recognized WM protocol request iff 
     * event type is ClientMessage and no parameters are passed, or
     * event type is ClientMessage and event data is matched to parameters, or
     * event type isn't ClientMessage and parameters make a request.
     */
#define DO_DELETE_WINDOW InParams(WM_DELETE_WINDOW, params, *num_params)

    if ((event->type == ClientMessage &&
	 event->xclient.message_type == wm_protocols &&
	 event->xclient.data.l[0] == wm_delete_window &&
	 (*num_params == 0 || DO_DELETE_WINDOW))
	|| 
	(event->type != ClientMessage && DO_DELETE_WINDOW)) {

#undef DO_DELETE_WINDOW

	Widget cancel;
	char descendant[DISMISS_NAME_LEN + 2];
	(void) sprintf(descendant, "*%s", DISMISS_NAME);
	cancel = XtNameToWidget(w, descendant);
	if (cancel) XtCallCallbacks(cancel, XtNcallback, (XtPointer)NULL);
    }
}

static void SetWMProtocolTranslations(w)
    Widget	w;	/* realized popup shell */
{
    int i;
    XtAppContext app_context;
    Atom wm_delete_window;
    static XtTranslations compiled_table;	/* initially 0 */
    static XtAppContext *app_context_list;	/* initially 0 */
    static Cardinal list_size;			/* initially 0 */

    app_context = XtWidgetToApplicationContext(w);

    /* parse translation table once */
    if (! compiled_table) compiled_table = XtParseTranslationTable
	("<Message>WM_PROTOCOLS: XawWMProtocols()\n");

    /* add actions once per application context */
    for (i=0; i < list_size && app_context_list[i] != app_context; i++) ;
    if (i == list_size) {
	XtActionsRec actions[1];
	actions[0].string = "XawWMProtocols";
	actions[0].proc = WMProtocols;
	list_size++;
	app_context_list = (XtAppContext *) XtRealloc
	    ((char *)app_context_list, list_size * sizeof(XtAppContext));
	XtAppAddActions(app_context, actions, 1);
	app_context_list[i] = app_context;
    }

    /* establish communication between the window manager and each shell */
    XtAugmentTranslations(w, compiled_table);
    wm_delete_window = XInternAtom(XtDisplay(w), WM_DELETE_WINDOW, False);
    (void) XSetWMProtocols(XtDisplay(w), XtWindow(w), &wm_delete_window, 1);
}
