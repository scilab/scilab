/*
 * $XConsortium: StrToLong.c,v 1.3 90/10/26 16:43:40 dave Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
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
 */


#include <X11/Intrinsic.h>
#include "Converters.h"

#define done(address, type) \
        { (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }

void XmuCvtStringToLong (args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
{
    static long l;

    if (*num_args != 0)
        XtWarningMsg("wrongParameters","cvtStringToLong","XtToolkitError",
                  "String to Long conversion needs no extra arguments",
                  (String *) NULL, (Cardinal *)NULL);
    if (sscanf((char *)fromVal->addr, "%ld", &l) == 1) {
        done(&l, long);
    } else {
        XtStringConversionWarning((char *) fromVal->addr, XtRLong);
    }
}
