/*
 * $XConsortium: EditresCom.c,v 1.27 91/07/30 15:44:00 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Chris D. Peterson, MIT X Consortium
 */

#include <X11/IntrinsicP.h>	/* To get into the composite and core widget
				   structures. */
#include <X11/ObjectP.h>	/* For XtIs<Classname> macros. */
#include <X11/StringDefs.h>	/* for XtRString. */
#include <X11/ShellP.h>		/* for Application Shell Widget class. */

#include <X11/Xatom.h>
#include <X11/Xos.h>		/* for strcpy declaration */
#include <X11/Xaw/Cardinals.h>
#include <X11/Xmu/EditresP.h>

#include <stdio.h>

#define _XEditResPutBool _XEditResPut8	
#define _XEditResPutResourceType _XEditResPut8

/************************************************************
 *
 * Local structure definitions.
 *
 ************************************************************/

typedef enum { BlockNone, BlockSetValues, BlockAll } EditresBlock;

typedef struct _SetValuesEvent {
    EditresCommand type;	/* first field must be type. */
    WidgetInfo * widgets;
    unsigned short num_entries;		/* number of set values requests. */
    char * name;
    char * res_type;
    XtPointer value;
    unsigned short value_len;
} SetValuesEvent;

typedef struct _SVErrorInfo {
    SetValuesEvent * event;
    ProtocolStream * stream;
    unsigned short * count;
    WidgetInfo * entry;
} SVErrorInfo;

typedef struct _FindChildEvent {
    EditresCommand type;	/* first field must be type. */
    WidgetInfo * widgets;
    short x, y;
} FindChildEvent;

typedef struct _GenericGetEvent {
    EditresCommand type;	/* first field must be type. */
    WidgetInfo * widgets;
    unsigned short num_entries;		/* number of set values requests. */
} GenericGetEvent, GetResEvent, GetGeomEvent;

/*
 * Things that are common to all events.
 */

typedef struct _AnyEvent {
    EditresCommand type;	/* first field must be type. */
    WidgetInfo * widgets;
} AnyEvent;

/*
 * The event union.
 */

typedef union _EditresEvent {
    AnyEvent any_event;
    SetValuesEvent set_values_event;
    GetResEvent get_resources_event;
    GetGeomEvent get_geometry_event;
    FindChildEvent find_child_event;
} EditresEvent;

typedef struct _Globals {
    EditresBlock block;
    SVErrorInfo error_info;
    ProtocolStream stream;
    ProtocolStream * command_stream; /* command stream. */
} Globals;

#define CURRENT_PROTOCOL_VERSION 4L

#define streq(a,b) (strcmp( (a), (b) ) == 0)

static Atom res_editor_command, res_editor_protocol, client_value;

static Globals globals;

static void SendFailure(), SendCommand(), InsertWidget(), ExecuteCommand();
static void FreeEvent(), ExecuteSetValues(), ExecuteGetGeometry();
static void ExecuteGetResources();

static void GetCommand();
static void LoadResources();
static Boolean IsChild();
static void DumpChildren();
static char *DumpWidgets(), *DoSetValues(), *DoFindChild();
static char *DoGetGeometry(), *DoGetResources();

/************************************************************
 *
 * Resource Editor Communication Code
 *
 ************************************************************/

/*	Function Name: _XEditResCheckMessages
 *	Description: This callback routine is set on all shell widgets,
 *                   and checks to see if a client message event
 *                   has come from the resource editor.
 *	Arguments: w - the shell widget.
 *                 data - *** UNUSED ***
 *                 event - The X Event that triggered this handler.
 *                 cont - *** UNUSED ***.
 *	Returns: none.
 */

/* ARGSUSED */
void
_XEditResCheckMessages(w, data, event, cont)
Widget w;
XtPointer data;
XEvent *event;
Boolean *cont;
{
    Time time;
    ResIdent ident;
    static Boolean first_time = FALSE;
    static Atom res_editor, res_comm;
    Display * dpy;

    if (event->type == ClientMessage) {
	XClientMessageEvent * c_event = (XClientMessageEvent *) event;
	dpy = XtDisplay(w);

	if (!first_time) {
	    first_time = TRUE;
	    res_editor = XInternAtom(dpy, EDITRES_NAME, False);
	    res_editor_command = XInternAtom(dpy, EDITRES_COMMAND_ATOM, False);
	    res_editor_protocol = XInternAtom(dpy, EDITRES_PROTOCOL_ATOM,
					      False);

	    /* Used in later procedures. */
	    client_value = XInternAtom(dpy, EDITRES_CLIENT_VALUE, False);
	    LoadResources(w);
	}

	if ((c_event->message_type != res_editor) ||
	    (c_event->format != EDITRES_SEND_EVENT_FORMAT))
	    return;

	time = c_event->data.l[0];
	res_comm = c_event->data.l[1];
	ident = (ResIdent) c_event->data.l[2];
	if (c_event->data.l[3] != CURRENT_PROTOCOL_VERSION) {
	    _XEditResResetStream(&globals.stream);
	    _XEditResPut8(&globals.stream, CURRENT_PROTOCOL_VERSION);
	    SendCommand(w, res_comm, ident, ProtocolMismatch, &globals.stream);
	    return;
	}

	XtGetSelectionValue(w, res_comm, res_editor_command,
			    GetCommand, (XtPointer) ident, time);
    }
}

/*	Function Name: BuildEvent
 *	Description: Takes the info out the protocol stream an constructs
 *                   the proper event structure.
 *	Arguments: w - widget to own selection, in case of error.
 *                 sel - selection to send error message beck in.
 *                 data - the data for the request.
 *                 ident - the id number we are looking for.
 *                 length - length of request.
 *	Returns: the event, or NULL.
 */

#define ERROR_MESSAGE ("Client: Improperly formatted protocol request")

static EditresEvent *
BuildEvent(w, sel, data, ident, length)
Widget w;
Atom sel;
XtPointer data;
ResIdent ident;
unsigned long length;
{
    EditresEvent * event;
    ProtocolStream alloc_stream, *stream;
    unsigned char temp;
    register unsigned int i;

    stream = &alloc_stream;	/* easier to think of it this way... */

    stream->current = stream->top = (unsigned char *) data;
    stream->size = HEADER_SIZE;		/* size of header. */

    /*
     * Retrieve the Header.
     */

    if (length < HEADER_SIZE) {
	SendFailure(w, sel, ident, Failure, ERROR_MESSAGE);
	return(NULL);
    }

    (void) _XEditResGet8(stream, &temp);
    if (temp != ident)		/* Id's don't match, ignore request. */
	return(NULL);		

    event = (EditresEvent *) XtCalloc(sizeof(EditresEvent), 1);

    (void) _XEditResGet8(stream, &temp);
    event->any_event.type = (EditresCommand) temp;
    (void) _XEditResGet32(stream, &(stream->size));
    stream->top = stream->current; /* reset stream to top of value.*/
	
    /*
     * Now retrieve the data segment.
     */
    
    switch(event->any_event.type) {
    case SendWidgetTree:
	break;			/* no additional info */
    case SetValues:
        {
	    SetValuesEvent * sv_event = (SetValuesEvent *) event;
	    
	    if ( !(_XEditResGetString8(stream, &(sv_event->name)) &&
		   _XEditResGetString8(stream, &(sv_event->res_type))))
	    {
		goto done;
	    }

	    /*
	     * Since we need the value length, we have to pull the
	     * value out by hand.
	     */

	    if (!_XEditResGet16(stream, &(sv_event->value_len)))
		goto done;

	    sv_event->value = XtMalloc(sizeof(char) * 
				       (sv_event->value_len + 1));

	    for (i = 0; i < sv_event->value_len; i++) {
		if (!_XEditResGet8(stream, 
				   (unsigned char *) sv_event->value + i)) 
		{
		    goto done;
		}
	    }
	    ((char*)sv_event->value)[i] = '\0'; /* NULL terminate that sucker. */

	    if (!_XEditResGet16(stream, &(sv_event->num_entries)))
		goto done;

	    sv_event->widgets = (WidgetInfo *)
		XtCalloc(sizeof(WidgetInfo), sv_event->num_entries);
	    
	    for (i = 0; i < sv_event->num_entries; i++) {
		if (!_XEditResGetWidgetInfo(stream, sv_event->widgets + i))
		    goto done;
	    }
	}
	break;
    case FindChild:
        {
	    FindChildEvent * find_event = (FindChildEvent *) event;
	    
	    find_event->widgets = (WidgetInfo *) 
		                  XtCalloc(sizeof(WidgetInfo), 1);

	    if (!(_XEditResGetWidgetInfo(stream, find_event->widgets) &&
		  _XEditResGetSigned16(stream, &(find_event->x)) &&
		  _XEditResGetSigned16(stream, &(find_event->y))))
	    {
		goto done;
	    }	    				

	}
	break;
    case GetGeometry:
    case GetResources:
        {
	    GenericGetEvent * get_event = (GenericGetEvent *) event;
	    
	    if (!_XEditResGet16(stream, &(get_event->num_entries)))
		goto done;
		
	    get_event->widgets = (WidgetInfo *)
		XtCalloc(sizeof(WidgetInfo), get_event->num_entries);
	    for (i = 0; i < get_event->num_entries; i++) {
		if (!_XEditResGetWidgetInfo(stream, get_event->widgets + i)) 
		    goto done;
	    }
	}
	break;
    default:
	{
	    char buf[BUFSIZ];
	    
	    sprintf(buf, "Unknown Protocol request %d.",event->any_event.type);
	    SendFailure(w, sel, ident, buf);
	    return(NULL);
	}
    }
    return(event);

 done:

    SendFailure(w, sel, ident, ERROR_MESSAGE);
    FreeEvent(event);
    return(NULL);
}    

/*	Function Name: FreeEvent
 *	Description: Frees the event structure and any other pieces
 *                   in it that need freeing.
 *	Arguments: event - the event to free.
 *	Returns: none.
 */

static void
FreeEvent(event)
EditresEvent * event;
{
    if (event->any_event.widgets != NULL) {
	XtFree((char *)event->any_event.widgets->ids);
	XtFree((char *)event->any_event.widgets);
    }

    if (event->any_event.type == SetValues) {
	XtFree(event->set_values_event.name);     /* XtFree does not free if */
	XtFree(event->set_values_event.res_type); /* value is NULL. */
    }
	
    XtFree((char *)event);
}

/*	Function Name: GetCommand
 *	Description: Gets the Command out of the selection asserted by the
 *                   resource manager.
 *	Arguments: (See Xt XtConvertSelectionProc)
 *                 data - contains the ident number for the command.
 *	Returns: none.
 */

/* ARGSUSED */
static void
GetCommand(w, data, selection, type, value, length, format)
Widget w;
XtPointer data, value;
Atom *selection, *type;
unsigned long *length;
int * format;
{
    ResIdent ident = (ResIdent) data;
    EditresEvent * event;

    if ( (*type != res_editor_protocol) || (*format != EDITRES_FORMAT) )
	return;

    if ((event = BuildEvent(w, *selection, value, ident, *length)) != NULL) {
	ExecuteCommand(w, *selection, ident, event);
	FreeEvent(event);
    }
}

/*	Function Name: ExecuteCommand
 *	Description: Executes a command string received from the 
 *                   resource editor.
 *	Arguments: w       - a widget.
 *                 command - the command to execute.
 *                 value - the associated with the command.
 *	Returns: none.
 *
 * NOTES:  munges str
 */

/* ARGSUSED */    
static void
ExecuteCommand(w, sel, ident, event)
Widget w;
Atom sel;
ResIdent ident;
EditresEvent * event;
{
    char * (*func)();
    char * str;

    if (globals.block == BlockAll) {
	SendFailure(w, sel, ident, 
		    "This client has blocked all Editres commands.");
	return;
    }
    else if ((globals.block == BlockSetValues) && 
	(event->any_event.type == SetValues)) {
	SendFailure(w, sel, ident, 
		    "This client has blocked all SetValues requests.");
	return;
    }

    switch(event->any_event.type) {
    case SendWidgetTree:
	func = DumpWidgets;
	break;
    case SetValues:
	func = DoSetValues;
	break;
    case FindChild:
	func = DoFindChild;
	break;
    case GetGeometry:
	func = DoGetGeometry;
	break;
    case GetResources:
	func = DoGetResources;
	break;
    default: 
        {
	    char buf[BUFSIZ];
	    sprintf(buf,"Unknown Protocol request %d.",event->any_event.type);
	    SendFailure(w, sel, ident, buf);
	    return;
	}
    }

    _XEditResResetStream(&globals.stream);
    if ((str = (*func)(w, event, &globals.stream)) == NULL)
	SendCommand(w, sel, ident, PartialSuccess, &globals.stream);
    else {
	SendFailure(w, sel, ident, str);
	XtFree(str);
    }
}

/*	Function Name: ConvertReturnCommand
 *	Description: Converts a selection.
 *	Arguments: w - the widget that owns the selection.
 *                 selection - selection to convert.
 *                 target - target type for this selection.
 *                 type_ret - type of the selection.
 *                 value_ret - selection value;
 *                 length_ret - lenght of this selection.
 *                 format_ret - the format the selection is in.
 *	Returns: True if conversion was sucessful.
 */
    
/* ARGSUSED */
static Boolean
ConvertReturnCommand(w, selection, target,
		     type_ret, value_ret, length_ret, format_ret)
Widget w;
Atom * selection, * target, * type_ret;
XtPointer *value_ret;
unsigned long * length_ret;
int * format_ret;
{
    /*
     * I assume the intrinsics give me the correct selection back.
     */

    if ((*target != client_value))
	return(FALSE);

    *type_ret = res_editor_protocol;
    *value_ret = (XtPointer) globals.command_stream->real_top;
    *length_ret = globals.command_stream->size + HEADER_SIZE;
    *format_ret = EDITRES_FORMAT;

    return(TRUE);
}

/*	Function Name: CommandDone
 *	Description: done with the selection.
 *	Arguments: *** UNUSED ***
 *	Returns: none.
 */

/* ARGSUSED */
static void
CommandDone(widget, selection, target)
Widget widget;
Atom *selection;
Atom *target;    
{
    /* Keep the toolkit from automaticaly freeing the selection value */
}

/*	Function Name: SendFailure
 *	Description: Sends a failure message.
 *	Arguments: w - the widget to own the selection.
 *                 sel - the selection to assert.
 *	 	   ident - the identifier.
 *                 str - the error message.
 *	Returns: none.
 */

static void
SendFailure(w, sel, ident, str) 
Widget w;
Atom sel;
ResIdent ident;
char * str;
{
    _XEditResResetStream(&globals.stream);
    _XEditResPutString8(&globals.stream, str);
    SendCommand(w, sel, ident, Failure, &globals.stream);
}

/*	Function Name: BuildReturnPacket
 *	Description: Builds a return packet, given the data to send.
 *	Arguments: ident - the identifier.
 *                 command - the command code.
 *                 stream - the protocol stream.
 *	Returns: packet - the packet to send.
 */

static XtPointer
BuildReturnPacket(ident, command, stream)
ResIdent ident;
EditresCommand command;
ProtocolStream * stream;
{
    long old_alloc, old_size;
    unsigned char * old_current;
    
    /*
     * We have cleverly keep enough space at the top of the header
     * for the return protocol stream, so all we have to do is
     * fill in the space.
     */

    /* 
     * Fool the insert routines into putting the header in the right
     * place while being damn sure not to realloc (that would be very bad.
     */
    
    old_current = stream->current;
    old_alloc = stream->alloc;
    old_size = stream->size;

    stream->current = stream->real_top;
    stream->alloc = stream->size + (2 * HEADER_SIZE);	
    
    _XEditResPut8(stream, ident);
    _XEditResPut8(stream, (unsigned char) command);
    _XEditResPut32(stream, old_size);

    stream->alloc = old_alloc;
    stream->current = old_current;
    stream->size = old_size;
    
    return((XtPointer) stream->real_top);
}    

/*	Function Name: SendCommand
 *	Description: Builds a return command line.
 *	Arguments: w - the widget to own the selection.
 *                 sel - the selection to assert.
 *	 	   ident - the identifier.
 *                 command - the command code.
 *                 stream - the protocol stream.
 *	Returns: none.
 */

static void
SendCommand(w, sel, ident, command, stream)
Widget w;
Atom sel;
ResIdent ident;
EditresCommand command;
ProtocolStream * stream;
{
    BuildReturnPacket(ident, command, stream);
    globals.command_stream = stream;	

/*
 * I REALLY want to own the selection.  Since this was not triggered
 * by a user action, and I am the only one using this atom it is safe to
 * use CurrentTime.
 */

    XtOwnSelection(w, sel, CurrentTime,
		   ConvertReturnCommand, NULL, CommandDone);
}

/************************************************************
 *
 * Generic Utility Functions.
 *
 ************************************************************/

/*	Function Name: FindChildren
 *	Description: Retuns all children (popup, normal and otherwise)
 *                   of this widget
 *	Arguments: parent - the parent widget.
 *                 children - the list of children.
 *                 normal - return normal children.
 *                 popup - return popup children.
 *	Returns: the number of children.
 */

static int
FindChildren(parent, children, normal, popup)
Widget parent, **children;
Boolean normal, popup;
{
    CompositeWidget cw = (CompositeWidget) parent;
    int i, num_children, current = 0;
    
    num_children = 0;

    if (XtIsWidget(parent) && popup)
	num_children += parent->core.num_popups;
	
    if (XtIsComposite(parent) && normal) 
	num_children += cw->composite.num_children; 

    if (num_children == 0) {	
	*children = NULL; 
	return(0);
    }

    *children =(Widget*) XtMalloc((Cardinal) sizeof(Widget) * num_children);

    if (XtIsComposite(parent) && normal)
	for (i = 0; i < cw->composite.num_children; i++,current++) 
	    (*children)[current] = cw->composite.children[i]; 

    if (XtIsWidget(parent) && popup)
	for ( i = 0; i < parent->core.num_popups; i++, current++) 
	    (*children)[current] = parent->core.popup_list[i];

    return(num_children);
}
		
/*	Function Name: IsChild
 *	Description: check to see of child is a child of parent.
 *	Arguments: top - the top of the tree.
 *                 parent - the parent widget.
 *                 child - the child.
 *	Returns: none.
 */

static Boolean
IsChild(top, parent, child)
Widget top, parent, child;
{
    int i, num_children;
    Widget * children;

    if (parent == NULL)
	return(top == child);

    num_children = FindChildren(parent, &children, TRUE, TRUE);

    for (i = 0; i < num_children; i++) {
	if (children[i] == child) {
	    XtFree((char *)children);
	    return(TRUE);
	}
    }

    XtFree((char *)children);
    return(FALSE);
}

/*	Function Name: VerifyWidget
 *	Description: Makes sure all the widgets still exist.
 *	Arguments: w - any widget in the tree.
 *                 info - the info about the widget to verify.
 *	Returns: an error message or NULL.
 */

static char * 
VerifyWidget(w, info)
Widget w;
WidgetInfo *info;
{
    Widget top;

    register int count;
    register Widget parent;
    register unsigned long * child;

    for (top = w; XtParent(top) != NULL; top = XtParent(top)) {}

    parent = NULL;
    child = info->ids;
    count = 0;

    while (TRUE) {
	if (!IsChild(top, parent, (Widget) *child)) 
	    return(XtNewString("This widget no longer exists in the client."));

	if (++count == info->num_widgets)
	    break;

	parent = (Widget) *child++;
    }

    info->real_widget = (Widget) *child;
    return(NULL);
}

/************************************************************
 *
 * Code to Perform SetValues operations.
 *
 ************************************************************/


/*	Function Name: 	DoSetValues
 *	Description: performs the setvalues requested.
 *	Arguments: w - a widget in the tree.
 *                 event - the event that caused this action.
 *                 stream - the protocol stream to add.
 *	Returns: NULL.
 */

static char *
DoSetValues(w, event, stream)
Widget w;
EditresEvent * event;
ProtocolStream * stream;
{
    char * str;
    register unsigned i;
    unsigned short count = 0;
    SetValuesEvent * sv_event = (SetValuesEvent *) event;
    
    _XEditResPut16(stream, count); /* insert 0, will be overwritten later. */

    for (i = 0 ; i < sv_event->num_entries; i++) {
	if ((str = VerifyWidget(w, &(sv_event->widgets[i]))) != NULL) {
	    _XEditResPutWidgetInfo(stream, &(sv_event->widgets[i]));
	    _XEditResPutString8(stream, str);
	    XtFree(str);
	    count++;
	}
	else 
	    ExecuteSetValues(sv_event->widgets[i].real_widget, 
			     sv_event, sv_event->widgets + i, stream, &count);
    }

    /*
     * Overwrite the first 2 bytes with the real count.
     */

    *(stream->top) = count >> XER_NBBY;
    *(stream->top + 1) = count;
    return(NULL);
}

/*	Function Name: HandleToolkitErrors
 *	Description: Handles X Toolkit Errors.
 *	Arguments: name - name of the error.
 *                 type - type of the error.
 *                 class - class of the error.
 *                 msg - the default message.
 *                 params, num_params - the extra parameters for this message.
 *	Returns: none.
 */

/* ARGSUSED */
static void
HandleToolkitErrors(name, type, class, msg, params, num_params)
String name, type, class, msg, *params;
Cardinal * num_params;
{
    SVErrorInfo * info = &globals.error_info;	
    char buf[BUFSIZ];

    if ( streq(name, "unknownType") ) 
	sprintf(buf, "The `%s' resource is not used by this widget.", 
		info->event->name); 
    else if ( streq(name, "noColormap") ) 
	sprintf(buf, msg, params[0]);
    else if (streq(name, "conversionFailed") || streq(name, "conversionError"))
    {
	if (streq(info->event->value, XtRString))
	    sprintf(buf, 
		    "Could not convert the string '%s' for the `%s' resource.",
		    info->event->value, info->event->name);   
	else
	    sprintf(buf, "Could not convert the `%s' resource.",
		    info->event->name);
    }
    else 
	sprintf(buf, "Name: %s, Type: %s, Class: %s, Msg: %s",
		name, type, class, msg);

    /*
     * Insert this info into the protocol stream, and update the count.
     */ 

    (*(info->count))++;
    _XEditResPutWidgetInfo(info->stream, info->entry);
    _XEditResPutString8(info->stream, buf);
}

/*	Function Name: ExecuteSetValues
 *	Description: Performs a setvalues for a given command.
 *	Arguments: w - the widget to perform the set_values on.
 *                 sv_event - the set values event.
 *                 sv_info - the set_value info.
 *	Returns: none.
 */

static void
ExecuteSetValues(w, sv_event, entry, stream, count)
Widget w;
SetValuesEvent * sv_event;
WidgetInfo * entry;
ProtocolStream * stream;
unsigned short * count;
{
    XtErrorMsgHandler old;
    
    SVErrorInfo * info = &globals.error_info;	
    info->event = sv_event;	/* No data can be passed to */
    info->stream = stream;	/* an error handler, so we */
    info->count = count;	/* have to use a global, YUCK... */
    info->entry = entry;

    old = XtAppSetWarningMsgHandler(XtWidgetToApplicationContext(w),
				    HandleToolkitErrors);

    XtVaSetValues(w, XtVaTypedArg,
		  sv_event->name, sv_event->res_type,
		  sv_event->value, sv_event->value_len,
		  NULL);

    (void)XtAppSetWarningMsgHandler(XtWidgetToApplicationContext(w), old);
}


/************************************************************
 *
 * Code for Creating and dumping widget tree.
 *
 ************************************************************/

/*	Function Name: 	DumpWidgets
 *	Description: Given a widget it builds a protocol packet
 *                   containing the entire widget heirarchy.
 *	Arguments: w - a widget in the tree.
 *                 event - the event that caused this action.
 *                 stream - the protocol stream to add.
 *	Returns: NULL
 */

/* ARGSUSED */
static char * 
DumpWidgets(w, event, stream)
Widget w;
EditresEvent * event;		/* UNUSED */
ProtocolStream * stream;
{
    unsigned short count = 0;
        
    /* Find Tree's root. */
    for ( ; XtParent(w) != NULL; w = XtParent(w)) {}
    
    /*
     * hold space for count, overwritten later. 
     */

    _XEditResPut16(stream, (unsigned int) 0);

    DumpChildren(w, stream, &count);

    /*
     * Overwrite the first 2 bytes with the real count.
     */

    *(stream->top) = count >> XER_NBBY;
    *(stream->top + 1) = count;
    return(NULL);
}

/*	Function Name: DumpChildren
 *	Description: Adds a child's name to the list.
 *	Arguments: w - the widget to dump.
 *                 stream - the stream to dump to.
 *                 count - number of dumps we have performed.
 *	Returns: none.
 */

/* This is a trick/kludge.  To make shared libraries happier (linking
 * against Xmu but not linking against Xt, and apparently even work
 * as we desire on SVR4, we need to avoid an explicit data reference
 * to applicationShellWidgetClass.  XtIsTopLevelShell is known
 * (implementation dependent assumption!) to use a bit flag.  So we
 * go that far.  Then, we test whether it is an applicationShellWidget
 * class by looking for an explicit class name.  Seems pretty safe.
 */
static Bool isApplicationShell(w)
    Widget w;
{
    register WidgetClass c;

    if (!XtIsTopLevelShell(w))
	return False;
    for (c = XtClass(w); c; c = c->core_class.superclass) {
	if (!strcmp(c->core_class.class_name, "ApplicationShell"))
	    return True;
    }
    return False;
}

static void
DumpChildren(w, stream, count)
Widget w;
ProtocolStream * stream;
unsigned short *count;
{
    int i, num_children;
    Widget *children;
    unsigned long window;
    char * class;

    (*count)++;
	
    InsertWidget(stream, w);       /* Insert the widget into the stream. */

    _XEditResPutString8(stream, XtName(w)); /* Insert name */

    if (isApplicationShell(w))
	class = ((ApplicationShellWidget) w)->application.class;
    else
	class = XtClass(w)->core_class.class_name;

    _XEditResPutString8(stream, class); /* Insert class */

     if (XtIsWidget(w))
	 if (XtIsRealized(w))
	    window = XtWindow(w);
	else
	    window = EDITRES_IS_UNREALIZED;
     else
	 window = EDITRES_IS_OBJECT;

    _XEditResPut32(stream, window); /* Insert window id. */

    /*
     * Find children and recurse.
     */

    num_children = FindChildren(w, &children, TRUE, TRUE);
    for (i = 0; i < num_children; i++) 
	DumpChildren(children[i], stream, count);

    XtFree((char *)children);
}

/************************************************************
 *
 * Code for getting the geometry of widgets.
 *
 ************************************************************/

/*	Function Name: 	DoGetGeometry
 *	Description: retrieves the Geometry of each specified widget.
 *	Arguments: w - a widget in the tree.
 *                 event - the event that caused this action.
 *                 stream - the protocol stream to add.
 *	Returns: NULL
 */

static char *
DoGetGeometry(w, event, stream)
Widget w;
EditresEvent * event;
ProtocolStream * stream;
{
    unsigned i;
    char * str;
    GetGeomEvent * geom_event = (GetGeomEvent *) event;
    
    _XEditResPut16(stream, geom_event->num_entries);

    for (i = 0 ; i < geom_event->num_entries; i++) {

	/* 
	 * Send out the widget id. 
	 */

	_XEditResPutWidgetInfo(stream, &(geom_event->widgets[i]));
	if ((str = VerifyWidget(w, &(geom_event->widgets[i]))) != NULL) {
	    _XEditResPutBool(stream, True); /* an error occured. */
	    _XEditResPutString8(stream, str);	/* set message. */
	    XtFree(str);
	}
	else 
	    ExecuteGetGeometry(geom_event->widgets[i].real_widget, stream);
    }
    return(NULL);
}

/*	Function Name: ExecuteGetGeometry
 *	Description: Gets the geometry for each widget specified.
 *	Arguments: w - the widget to get geom on.
 *                 stream - stream to append to.
 *	Returns: True if no error occured.
 */

static void
ExecuteGetGeometry(w, stream)
Widget w;
ProtocolStream * stream;
{
    int i;
    Boolean mapped_when_man;
    Dimension width, height, border_width;
    Arg args[8];
    Cardinal num_args = 0;
    Position x, y;
    
    if ( !XtIsRectObj(w) || (XtIsWidget(w) && !XtIsRealized(w)) ) {
	_XEditResPutBool(stream, False); /* no error. */
	_XEditResPutBool(stream, False); /* not visable. */
	for (i = 0; i < 5; i++) /* fill in extra space with 0's. */
	    _XEditResPut16(stream, 0);
	return;
    }

    XtSetArg(args[num_args], XtNwidth, &width); num_args++;
    XtSetArg(args[num_args], XtNheight, &height); num_args++;
    XtSetArg(args[num_args], XtNborderWidth, &border_width); num_args++;
    XtSetArg(args[num_args], XtNmappedWhenManaged, &mapped_when_man);
    num_args++;
    XtGetValues(w, args, num_args);

    if (!(XtIsManaged(w) && mapped_when_man) && XtIsWidget(w)) {
	XWindowAttributes attrs;
	
	/* 
	 * The toolkit does not maintain mapping state, we have
	 * to go to the server.
	 */
	
	if (XGetWindowAttributes(XtDisplay(w), XtWindow(w), &attrs) != 0) {
	    if (attrs.map_state != IsViewable) {
		_XEditResPutBool(stream, False); /* no error. */
		_XEditResPutBool(stream, False); /* not visable. */
		for (i = 0; i < 5; i++) /* fill in extra space with 0's. */
		    _XEditResPut16(stream, 0);
		return;
	    }
	}
	else {
	    _XEditResPut8(stream, True); /* Error occured. */
	    _XEditResPutString8(stream, "XGetWindowAttributes failed.");
	    return;
	}
    }

    XtTranslateCoords(w, -((int) border_width), -((int) border_width), &x, &y);

    _XEditResPutBool(stream, False); /* no error. */
    _XEditResPutBool(stream, True); /* Visable. */
    _XEditResPut16(stream, x);
    _XEditResPut16(stream, y);
    _XEditResPut16(stream, width);
    _XEditResPut16(stream, height);
    _XEditResPut16(stream, border_width);
}

/************************************************************
 *
 * Code for executing FindChild.
 *
 ************************************************************/

/*	Function Name: PositionInChild
 *	Description: returns true if this location is in the child.
 *	Arguments: child - the child widget to check.
 *                 x, y - location of point to check in the parent's
 *                        coord space.
 *	Returns: TRUE if the position is in this child.
 */

static Boolean
PositionInChild(child, x, y)
Widget child;
int x, y;
{
    Arg args[6];
    Cardinal num;
    Dimension width, height, border_width;
    Position child_x, child_y;
    Boolean mapped_when_managed;

    if (!XtIsRectObj(child))	/* we must at least be a rect obj. */
	return(FALSE);

    num = 0;
    XtSetArg(args[num], XtNmappedWhenManaged, &mapped_when_managed); num++;
    XtSetArg(args[num], XtNwidth, &width); num++;
    XtSetArg(args[num], XtNheight, &height); num++;
    XtSetArg(args[num], XtNx, &child_x); num++;
    XtSetArg(args[num], XtNy, &child_y); num++;
    XtSetArg(args[num], XtNborderWidth, &border_width); num++;
    XtGetValues(child, args, num);
 
    /*
     * The only way we will know of the widget is mapped is to see if
     * mapped when managed is True and this is a managed child.  Otherwise
     * we will have to ask the server if this window is mapped.
     */

    if (XtIsWidget(child) && !(mapped_when_managed && XtIsManaged(child)) ) {
	XWindowAttributes attrs;

	if (XGetWindowAttributes(XtDisplay(child), 
				 XtWindow(child), &attrs) != 0) {
	    /* oops */
	}
	else if (attrs.map_state != IsViewable)
	    return(FALSE);
    }

    return (x >= child_x) &&
	   (x <= (child_x + (Position)width + 2 * (Position)border_width)) &&
	   (y >= child_y) &&
	   (y <= (child_y + (Position)height + 2 * (Position)border_width));
}

/*	Function Name: _FindChild
 *	Description: Finds the child that actually contatians the point shown.
 *	Arguments: parent - a widget that is known to contain the point
 *                 	    specified.
 *                 x, y - The point in coordinates relative to the 
 *                        widget specified.
 *	Returns: none.
 */

static Widget 
_FindChild(parent, x, y)
Widget parent;
int x, y;
{
    Widget * children;
    int i = FindChildren(parent, &children, TRUE, FALSE);

    while (i > 0) {
	i--;

	if (PositionInChild(children[i], x, y)) {
	    Widget child = children[i];
	    
	    XtFree((char *)children);
	    return(_FindChild(child, x - child->core.x, y - child->core.y));
	}
    }

    XtFree((char *)children);
    return(parent);
}

/*	Function Name: DoFindChild
 *	Description: finds the child that contains the location specified.
 *	Arguments: w - a widget in the tree.
 *                 event - the event that caused this action.
 *                 stream - the protocol stream to add.
 *	Returns: an allocated error message if something went horribly
 *               wrong and no set values were performed, else NULL.
 */

static char *
DoFindChild(w, event, stream)
Widget w;
EditresEvent * event;
ProtocolStream * stream;
{
    char * str;
    Widget parent, child;
    Position parent_x, parent_y;
    FindChildEvent * find_event = (FindChildEvent *) event;
    
    if ((str = VerifyWidget(w, find_event->widgets)) != NULL) 
	return(str);

    parent = find_event->widgets->real_widget;

    XtTranslateCoords(parent, (Position) 0, (Position) 0,
		      &parent_x, &parent_y);
    
    child = _FindChild(parent, find_event->x - (int) parent_x,
		       find_event->y - (int) parent_y);

    InsertWidget(stream, child);
    return(NULL);
}

/************************************************************
 *
 * Procedures for performing GetResources.
 *
 ************************************************************/

/*	Function Name: DoGetResources
 *	Description: Gets the Resources associated with the widgets passed.
 *	Arguments: w - a widget in the tree.
 *                 event - the event that caused this action.
 *                 stream - the protocol stream to add.
 *	Returns: NULL
 */

static char *
DoGetResources(w, event, stream)
Widget w;
EditresEvent * event;
ProtocolStream * stream;
{
    unsigned int i;
    char * str;
    GetResEvent * res_event = (GetResEvent *) event;
    
    _XEditResPut16(stream, res_event->num_entries); /* number of replys */

    for (i = 0 ; i < res_event->num_entries; i++) {
	/* 
	 * Send out the widget id. 
	 */
	_XEditResPutWidgetInfo(stream, &(res_event->widgets[i]));
	if ((str = VerifyWidget(w, &(res_event->widgets[i]))) != NULL) {
	    _XEditResPutBool(stream, True); /* an error occured. */
	    _XEditResPutString8(stream, str);	/* set message. */
	    XtFree(str);
	}
	else {
	    _XEditResPutBool(stream, False); /* no error occured. */
	    ExecuteGetResources(res_event->widgets[i].real_widget,
				stream);
	}
    }
    return(NULL);
}

/*	Function Name: ExecuteGetResources.
 *	Description: Gets the resources for any individual widget.
 *	Arguments: w - the widget to get resources on.
 *                 stream - the protocol stream.
 *	Returns: none.
 */

static void
ExecuteGetResources(w, stream)
Widget w;
ProtocolStream * stream;
{
    XtResourceList norm_list, cons_list;
    Cardinal num_norm, num_cons;
    register int i;

    /* 
     * Get Normal Resources. 
     */

    XtGetResourceList(XtClass(w), &norm_list, &num_norm);

    if (XtParent(w) != NULL) 
	XtGetConstraintResourceList(XtClass(XtParent(w)),&cons_list,&num_cons);
    else
	num_cons = 0;

    _XEditResPut16(stream, num_norm + num_cons); /* how many resources. */
    
    /*
     * Insert all the normal resources.
     */

    for ( i = 0; i < (int) num_norm; i++) {
	_XEditResPutResourceType(stream, NormalResource);
	_XEditResPutString8(stream, norm_list[i].resource_name);
	_XEditResPutString8(stream, norm_list[i].resource_class);
	_XEditResPutString8(stream, norm_list[i].resource_type);
    }
    XtFree((char *) norm_list);

    /*
     * Insert all the constraint resources.
     */

    if (num_cons > 0) {
	for ( i = 0; i < (int) num_cons; i++) {
	    _XEditResPutResourceType(stream, ConstraintResource);
	    _XEditResPutString8(stream, cons_list[i].resource_name);
	    _XEditResPutString8(stream, cons_list[i].resource_class);
	    _XEditResPutString8(stream, cons_list[i].resource_type);
	}
	XtFree((char *) cons_list);
    }
}

/************************************************************
 *
 * Code for inserting values into the protocol stream.
 *
 ************************************************************/

/*	Function Name: InsertWidget
 *	Description: Inserts the full parent heirarchy of this
 *                   widget into the protocol stream as a widget list.
 *	Arguments: stream - the protocol stream.
 *                 w - the widget to insert.
 *	Returns: none
 */

static void
InsertWidget(stream, w)
ProtocolStream * stream;
Widget w;
{
    Widget temp;
    unsigned long * widget_list;
    register int i, num_widgets;

    for (temp = w, i = 0; temp != 0; temp = XtParent(temp), i++) {}

    num_widgets = i;
    widget_list = (unsigned long *) 
	                XtMalloc(sizeof(unsigned long) * num_widgets);

    /*
     * Put the widgets into the list.
     * make sure that they are inserted in the list from parent -> child.
     */

    for (i--, temp = w; temp != NULL; temp = XtParent(temp), i--) 
	widget_list[i] = (unsigned long) temp;
	
    _XEditResPut16(stream, num_widgets);	/* insert number of widgets. */
    for (i = 0; i < num_widgets; i++) /* insert Widgets themselves. */
	_XEditResPut32(stream, widget_list[i]);
    
    XtFree((char *)widget_list);
}

/************************************************************
 *
 * All of the following routines are public.
 *
 ************************************************************/

/*	Function Name: _XEditResPutString8
 *	Description: Inserts a string into the protocol stream.
 *	Arguments: stream - stream to insert string into.
 *                 str - string to insert.
 *	Returns: none.
 */

void
_XEditResPutString8(stream, str)
ProtocolStream * stream;
char * str;
{
    int i, len = strlen(str);

    _XEditResPut16(stream, len);
    for (i = 0 ; i < len ; i++, str++)
	_XEditResPut8(stream, *str);
}

/*	Function Name: _XEditResPut8
 *	Description: Inserts an 8 bit integer into the protocol stream.
 *	Arguments: stream - stream to insert string into.
 *                 value - value to insert.
 *	Returns: none
 */

void
_XEditResPut8(stream, value)
ProtocolStream * stream;
unsigned int value;
{
    unsigned char temp;

    if (stream->size >= stream->alloc) {
	stream->alloc += 100;
	stream->real_top = (unsigned char *) XtRealloc(
						  (char *)stream->real_top,
						  stream->alloc + HEADER_SIZE);
	stream->top = stream->real_top + HEADER_SIZE;
	stream->current = stream->top + stream->size;
    }

    temp = (unsigned char) (value & BYTE_MASK);
    *((stream->current)++) = temp;
    (stream->size)++;
}

/*	Function Name: _XEditResPut16
 *	Description: Inserts a 16 bit integer into the protocol stream.
 *	Arguments: stream - stream to insert string into.
 *                 value - value to insert.
 *	Returns: void
 */

void
_XEditResPut16(stream, value)
ProtocolStream * stream;
unsigned int value;
{
    _XEditResPut8(stream, (value >> XER_NBBY) & BYTE_MASK);
    _XEditResPut8(stream, value & BYTE_MASK);
}

/*	Function Name: _XEditResPut32
 *	Description: Inserts a 32 bit integer into the protocol stream.
 *	Arguments: stream - stream to insert string into.
 *                 value - value to insert.
 *	Returns: void
 */

void
_XEditResPut32(stream, value)
ProtocolStream * stream;
unsigned long value;
{
    int i;

    for (i = 3; i >= 0; i--) 
	_XEditResPut8(stream, (value >> (XER_NBBY*i)) & BYTE_MASK);
}

/*	Function Name: _XEditResPutWidgetInfo
 *	Description: Inserts the widget info into the protocol stream.
 *	Arguments: stream - stream to insert widget info into.
 *                 info - info to insert.
 *	Returns: none
 */

void
_XEditResPutWidgetInfo(stream, info)
ProtocolStream * stream;
WidgetInfo * info;
{
    unsigned int i;

    _XEditResPut16(stream, info->num_widgets);
    for (i = 0; i < info->num_widgets; i++) 
	_XEditResPut32(stream, info->ids[i]);
}

/************************************************************
 *
 * Code for retrieving values from the protocol stream.
 *
 ************************************************************/
    
/*	Function Name: _XEditResResetStream
 *	Description: resets the protocol stream
 *	Arguments: stream - the stream to reset.
 *	Returns: none.
 */

void
_XEditResResetStream(stream)
ProtocolStream * stream;
{
    stream->current = stream->top;
    stream->size = 0;
    if (stream->real_top == NULL) {
	stream->real_top = (unsigned char *) XtRealloc(
						  (char *)stream->real_top,
						  stream->alloc + HEADER_SIZE);
	stream->top = stream->real_top + HEADER_SIZE;
	stream->current = stream->top + stream->size;
    }
}

/*
 * NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE 
 *
 * The only modified field if the "current" field.
 *
 * The only fields that must be set correctly are the "current", "top"
 * and "size" fields.
 */

/*	Function Name: _XEditResGetg8
 *	Description: Retrieves an unsigned 8 bit value
 *                   from the protocol stream.
 *	Arguments: stream.
 *                 val - a pointer to value to return.
 *	Returns: TRUE if sucessful.
 */

Boolean
_XEditResGet8(stream, val)
ProtocolStream * stream;
unsigned char * val;
{
    if (stream->size < (stream->current - stream->top)) 
	return(FALSE);

    *val = *((stream->current)++);
    return(TRUE);
}

/*	Function Name: _XEditResGet16
 *	Description: Retrieves an unsigned 16 bit value
 *                   from the protocol stream.
 *	Arguments: stream.
 *                 val - a pointer to value to return.
 *	Returns: TRUE if sucessful.
 */

Boolean
_XEditResGet16(stream, val)
ProtocolStream * stream;
unsigned short * val;
{
    unsigned char temp1, temp2;

    if ( !(_XEditResGet8(stream, &temp1) && _XEditResGet8(stream, &temp2)) )
	return(FALSE);
    
    *val = (((unsigned short) temp1 << XER_NBBY) + ((unsigned short) temp2));
    return(TRUE);
}

/*	Function Name: _XEditResGetSigned16
 *	Description: Retrieves an signed 16 bit value from the protocol stream.
 *	Arguments: stream.
 *                 val - a pointer to value to return.
 *	Returns: TRUE if sucessful.
 */

Boolean
_XEditResGetSigned16(stream, val)
ProtocolStream * stream;
short * val;
{
    unsigned char temp1, temp2;

    if ( !(_XEditResGet8(stream, &temp1) && _XEditResGet8(stream, &temp2)) )
	return(FALSE);
    
    if (temp1 & (1 << (XER_NBBY - 1))) { /* If the sign bit is active. */
	*val = -1;		 /* store all 1's */
	*val &= (temp1 << XER_NBBY); /* Now and in the MSB */
	*val &= temp2;		 /* and LSB */
    }
    else 
	*val = (((unsigned short) temp1 << XER_NBBY) + ((unsigned short) temp2));

    return(TRUE);
}

/*	Function Name: _XEditResGet32
 *	Description: Retrieves an unsigned 32 bit value
 *                   from the protocol stream.
 *	Arguments: stream.
 *                 val - a pointer to value to return.
 *	Returns: TRUE if sucessful.
 */

Boolean
_XEditResGet32(stream, val)
ProtocolStream * stream;
unsigned long * val;
{
    unsigned short temp1, temp2;

    if ( !(_XEditResGet16(stream, &temp1) && _XEditResGet16(stream, &temp2)) )
	return(FALSE);
    
    *val = (((unsigned short) temp1 << (XER_NBBY * 2)) + 
	    ((unsigned short) temp2));
    return(TRUE);
}

/*	Function Name: _XEditResGetString8
 *	Description: Retrieves an 8 bit string value from the protocol stream.
 *	Arguments: stream - the protocol stream
 *                 str - the string to retrieve.
 *	Returns: True if retrieval was successful.
 */

Boolean
_XEditResGetString8(stream, str)
ProtocolStream * stream;
char ** str;
{
    unsigned short len;
    register unsigned i;

    if (!_XEditResGet16(stream, &len)) {
	return(FALSE);
    }

    *str = XtMalloc(sizeof(char) * (len + 1));

    for (i = 0; i < len; i++) {
	if (!_XEditResGet8(stream, (unsigned char *) *str + i)) {
	    XtFree(*str);
	    *str = NULL;
	    return(FALSE);
	}
    }
    (*str)[i] = '\0';		/* NULL terminate that sucker. */
    return(TRUE);
}

/*	Function Name: _XEditResGetWidgetInfo
 *	Description: Retrieves the list of widgets that follow and stores
 *                   them in the widget info structure provided.
 *	Arguments: stream - the protocol stream
 *                 info - the widget info struct to store into.
 *	Returns: True if retrieval was successful.
 */

Boolean
_XEditResGetWidgetInfo(stream, info)
ProtocolStream * stream;
WidgetInfo * info;
{
    unsigned int i;

    if (!_XEditResGet16(stream, &(info->num_widgets))) 
	return(FALSE);

    info->ids = (unsigned long *) XtMalloc(sizeof(long) * (info->num_widgets));

    for (i = 0; i < info->num_widgets; i++) {
	if (!_XEditResGet32(stream, info->ids + i)) {
	    XtFree((char *)info->ids);
	    info->ids = NULL;
	    return(FALSE);
	}
    }
    return(TRUE);
}
	    
/************************************************************
 *
 * Code for Loading the EditresBlock resource.
 *
 ************************************************************/

/*	Function Name: CvStringToBlock
 *	Description: Converts a string to an editres block value.
 *	Arguments: dpy - the display.
 *                 args, num_args - **UNUSED **
 *                 from_val, to_val - value to convert, and where to put result
 *                 converter_data - ** UNUSED **
 *	Returns: TRUE if conversion was sucessful.
 */

/* ARGSUSED */
static Boolean
CvtStringToBlock(dpy, args, num_args, from_val, to_val, converter_data)
Display * dpy;
XrmValue * args;
Cardinal * num_args;
XrmValue * from_val, * to_val;
XtPointer * converter_data;
{
    char ptr[BUFSIZ];
    static EditresBlock block;

    XmuCopyISOLatin1Lowered(ptr, from_val->addr);

    if (streq(ptr, "none")) 
	block = BlockNone;
    else if (streq(ptr, "setvalues")) 
	block = BlockSetValues;
    else if (streq(ptr, "all")) 
	block = BlockAll;
    else {
	Cardinal num_params = 1;
	String params[1];

	params[0] = from_val->addr;
	XtAppWarningMsg(XtDisplayToApplicationContext(dpy),
			"CvtStringToBlock", "unknownValue", "EditresError",
			"Could not convert string \"%s\" to EditresBlock.",
			params, &num_params);
	return(FALSE);
    }

    if (to_val->addr != NULL) {
	if (to_val->size < sizeof(EditresBlock)) {
	    to_val->size = sizeof(EditresBlock);
	    return(FALSE);
	}
	*(EditresBlock *)(to_val->addr) = block;
    }
    else 
	to_val->addr = (XtPointer) block;

    to_val->size = sizeof(EditresBlock);
    return(TRUE);
}
    
#define XtREditresBlock ("EditresBlock")

/*	Function Name: LoadResources
 *	Description: Loads a global resource the determines of this
 *                   application should allow Editres requests.
 *	Arguments: w - any widget in the tree.
 *	Returns: none.
 */

static void
LoadResources(w)
Widget w;
{
    static XtResource resources[] = {
        {"editresBlock", "EditresBlock", XtREditresBlock, sizeof(EditresBlock),
	 XtOffsetOf(Globals, block), XtRImmediate, (XtPointer) BlockNone}
    };

    for (; XtParent(w) != NULL; w = XtParent(w)) {} 

    XtAppSetTypeConverter(XtWidgetToApplicationContext(w),
			  XtRString, XtREditresBlock, CvtStringToBlock,
			  NULL, (Cardinal) 0, XtCacheAll, NULL);

    XtGetApplicationResources( w, (caddr_t) &globals, resources,
			      XtNumber(resources), NULL, (Cardinal) 0);
}

    
