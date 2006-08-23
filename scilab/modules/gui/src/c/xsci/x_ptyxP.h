/* Private definitions for x_pty widget */

#ifndef _XtermP_h
#define _XtermP_h

#include <X11/IntrinsicP.h>
#include "x_ptyx.h"

typedef struct {int foo;} XtermClassPart;

typedef struct _XtermClassRec {
    CoreClassPart  core_class;
    XtermClassPart xterm_class;
} XtermClassRec;


typedef struct _XtermWidgetRec {
    CorePart	core;
    TKeyboard	keyboard;	/* terminal keyboard		*/
    TScreen	screen;		/* terminal screen		*/
    unsigned	flags;		/* mode flags			*/
    unsigned	initflags;	/* initial mode flags		*/
    Tabs	tabs;		/* tabstops of the terminal	*/
    Misc	misc;		/* miscellaneous parameters	*/
} XtermWidgetRec;


#endif /* _XtermP_h */
