/* $XConsortium: CvtStdSel.c,v 1.23 91/04/11 09:04:08 rws Exp $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology
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
 *
 * This file contains routines to handle common selection targets.
 *
 * Public entry points:
 *
 *	XmuConvertStandardSelection()	return a known selection
 */

#ifdef SYSVNET
#include <interlan/il_types.h>
#define __TYPES__		/* prevent #include <sys/types.h> in Xlib.h */
#include <interlan/netdb.h>
#include <interlan/socket.h>
#endif /* SYSVNET */

#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/ShellP.h>
#include <stdio.h>

#ifndef SYSVNET
#include <netdb.h>
#include <sys/socket.h>
#endif

#include "Atoms.h"
#include "StdSel.h"
#include "SysUtil.h"
#include <X11/Xfuncs.h>
#include <X11/Xos.h>

#ifndef OS_NAME
#ifndef X_OS_FILE
#ifdef SYSV			/* keep separate until makedepend fixed */
#define USE_UNAME
#endif
#ifdef SVR4
#define USE_UNAME
#endif
#ifdef ultrix
#define USE_UNAME
#endif
#endif /*X_OS_FILE*/
#ifdef USE_UNAME
#ifdef ultrix
#ifndef __STDC__
#include <limits.h>		/* fixed in Ultrix 3.0 */
#endif
#endif
#include <sys/utsname.h>
#endif
#endif

static char *get_os_name ()
{
#ifdef OS_NAME
	return XtNewString(OS_NAME);
#else
	FILE *f = NULL;

#ifdef USE_UNAME
	struct utsname uts;

	if (uname (&uts) == 0) {
	    char *os_name;
	    int len = strlen(uts.sysname);
#ifndef hpux				/* because of hostname length crock */
	    len += 1 + strlen(uts.release);
#endif
	    os_name = XtMalloc (len);
	    strcpy (os_name, uts.sysname);
#ifndef hpux
	    strcat (os_name, " ");
	    strcat (os_name, uts.release);
#endif
	    return os_name;
	}
#endif

#ifdef X_OS_FILE
	f = fopen(X_OS_FILE, "r");
	if (!f)
#endif
#ifdef MOTD_FILE
	       f = fopen(MOTD_FILE, "r");
#endif
	if (f) {
	    char motd[512];
	    motd[0] = '\0';
	    (void) fgets(motd, 511, f);
	    fclose(f);
	    motd[511] = '\0';
	    if (motd[0] != '\0') {
		int len = strlen(motd);
		if (motd[len - 1] == '\n')
		    motd[len - 1] = '\0';
		return XtNewString(motd);
	    }
	}

#ifdef sun
	return XtNewString("SunOS");
#else
# if !defined(SYSV) && defined(unix)
	return XtNewString("BSD");
# else
	return NULL;
# endif
#endif

#endif /*OS_NAME*/
}

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

Boolean XmuConvertStandardSelection(w, time, selection, target,
				    type, value, length, format)
    Widget w;
    Time time;
    Atom *selection, *target, *type;
    caddr_t *value;
    unsigned long *length;
    int *format;
{
    Display *d = XtDisplay(w);
    if (*target == XA_TIMESTAMP(d)) {
	*value = XtMalloc(4);
	if (sizeof(long) == 4)
	    *(long*)*value = time;
	else {
	    long temp = time;
	    bcopy( ((char*)&temp)+sizeof(long)-4, (char*)*value, 4);
	}
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_HOSTNAME(d)) {
	char hostname[1024];
	hostname[0] = '\0';
	*length = XmuGetHostname (hostname, sizeof hostname);
	*value = XtNewString(hostname);
	*type = XA_STRING;
	*format = 8;
	return True;
    }
#ifdef TCPCONN
    if (*target == XA_IP_ADDRESS(d)) {
	char hostname[1024];

	struct hostent *hostent;
	hostname[0] = '\0';
	(void) XmuGetHostname (hostname, sizeof hostname);
	hostent = gethostbyname (hostname);
	if (hostent->h_addrtype != AF_INET) return False;
	*length = hostent->h_length;
	*value = XtMalloc(*length);
	bcopy (hostent->h_addr, *value, *length);
	*type = XA_NET_ADDRESS(d);
	*format = 8;
	return True;
    }
#endif
#ifdef DNETCONN
    if (*target == XA_DECNET_ADDRESS(d)) {
	return False;		/* XXX niy */
    }
#endif
    if (*target == XA_USER(d)) {
	char *name = (char*)getenv("USER");
	if (name == NULL) return False;
	*value = XtNewString(name);
	*type = XA_STRING;
	*length = strlen(name);
	*format = 8;
	return True;
    }
    if (*target == XA_CLASS(d)) {
	Widget parent = XtParent(w);
	char *class;
	int len;
	while (parent != NULL && !isApplicationShell(w)) {
	    w = parent;
	    parent = XtParent(w);
	}
	if (isApplicationShell(w))
	    class = ((ApplicationShellWidget) w)->application.class;
	else
	    class = XtClass(w)->core_class.class_name;
	*length = (len=strlen(w->core.name)) + strlen(class) + 2;
	*value = XtMalloc(*length);
	strcpy( (char*)*value, w->core.name );
	strcpy( (char*)*value+len+1, class );
	*type = XA_STRING;
	*format = 8;
	return True;
    }
    if (*target == XA_NAME(d)) {
	Widget parent = XtParent(w);

	while (parent != NULL && !XtIsWMShell(w)) {
	    w = parent;
	    parent = XtParent(w);
	}
	if (!XtIsWMShell(w)) return False;
	*value = XtNewString( ((WMShellWidget) w)->wm.title );
	*length = strlen(*value);
	*type = XA_STRING;
	*format = 8;
	return True;
    }
    if (*target == XA_CLIENT_WINDOW(d)) {
	Widget parent = XtParent(w);
	while (parent != NULL) {
	    w = parent;
	    parent = XtParent(w);
	}
	*value = XtMalloc(sizeof(Window));
	*(Window*)*value = w->core.window;
	*type = XA_WINDOW;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_OWNER_OS(d)) {
	*value = get_os_name();
	if (*value == NULL) return False;
	*type = XA_STRING;
	*length = strlen(*value);
	*format = 8;
	return True;
    }
    if (*target == XA_TARGETS(d)) {
#if defined(unix) && defined(DNETCONN)
#  define NUM_TARGETS 9
#else
#  if defined(unix) || defined(DNETCONN)
#    define NUM_TARGETS 8
#  else
#    define NUM_TARGETS 7
#  endif
#endif
	Atom* std_targets = (Atom*)XtMalloc(NUM_TARGETS*sizeof(Atom));
	int i = 0;
	std_targets[i++] = XA_TIMESTAMP(d);
	std_targets[i++] = XA_HOSTNAME(d);
	std_targets[i++] = XA_IP_ADDRESS(d);
	std_targets[i++] = XA_USER(d);
	std_targets[i++] = XA_CLASS(d);
	std_targets[i++] = XA_NAME(d);
	std_targets[i++] = XA_CLIENT_WINDOW(d);
#ifdef unix
	std_targets[i++] = XA_OWNER_OS(d);
#endif
#ifdef DNETCONN
	std_targets[i++] = XA_DECNET_ADDRESS(d);
#endif
	*value = (caddr_t)std_targets;
	*type = XA_ATOM;
	*length = NUM_TARGETS;
	*format = 32;
	return True;
    }
    /* else */
    return False;
}
