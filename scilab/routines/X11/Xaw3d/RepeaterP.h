/*
 * $XConsortium: RepeaterP.h,v 1.4 94/04/17 20:12:39 jim Exp $
 *
Copyright (c) 1989  X Consortium

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
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#ifndef _XawRepeaterP_h
#define _XawRepeaterP_h

#include <X11/Xaw3d/CommandP.h>
#include <X11/Xaw3d/Repeater.h>

typedef struct {			/* new fields in widget class */
    int dummy;
} RepeaterClassPart;

typedef struct _RepeaterClassRec {	/* Repeater widget class */
    CoreClassPart core_class;
    SimpleClassPart simple_class;
    ThreeDClassPart threeD_class;
    LabelClassPart label_class;
    CommandClassPart command_class;
    RepeaterClassPart repeater_class;
} RepeaterClassRec;

typedef struct {			/* new fields in widget */
    /* resources... */
    int initial_delay;			/* initialDelay/Delay */
    int repeat_delay;			/* repeatDelay/Delay */
    int minimum_delay;			/* minimumDelay/MinimumDelay */
    int decay;				/* decay to minimum delay */
    Boolean flash;			/* flash/Boolean */
    XtCallbackList start_callbacks;	/* startCallback/StartCallback */
    XtCallbackList stop_callbacks;	/* stopCallback/StopCallback */
    /* private data... */
    int next_delay;			/* next amount for timer */
    XtIntervalId timer;			/* timer that will fire */
} RepeaterPart;

typedef struct _RepeaterRec {
    CorePart core;
    SimplePart simple;
    ThreeDPart threeD;
    LabelPart label;
    CommandPart command;
    RepeaterPart repeater;
} RepeaterRec;

#define REP_DEF_DECAY 5			/* milliseconds */
#define REP_DEF_INITIAL_DELAY 200	/* milliseconds */
#define REP_DEF_MINIMUM_DELAY 10	/* milliseconds */
#define REP_DEF_REPEAT_DELAY 50		/* milliseconds */

/*
 * external declarations
 */
extern RepeaterClassRec repeaterClassRec;

#endif /* _XawRepeaterP_h */
