/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 *
 * "Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty."
 *
 */

#include <X11/Xos.h>

#include <sys/stat.h>
#ifndef SYSV
#ifndef SVR4
#include <fcntl.h>
#endif
#endif
#include <pwd.h>
#include <signal.h>

#include <stdio.h>
#include <ctype.h>
#include <errno.h>


#ifndef linux 
#if !defined(__bsdi__) && !defined(__NetBSD__) && !defined(__APPLE__)
extern int	errno;
extern int	sys_nerr;
#if (! (defined(BSD) && (BSD >= 199306))) && !defined(freebsd)  && !defined(__APPLE__)
extern char    *sys_errlist[];
#endif
#endif
#endif 

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xatom.h>

#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Dialog.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/Text.h>
#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/MenuButton.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/Sme.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Viewport.h>
#include <X11/Xaw3d/List.h>
