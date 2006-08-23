/* $XFree86: xc/lib/misc/snprintf.h,v 3.1 1996/08/26 14:42:33 dawes Exp $ */

#ifndef SNPRINTF_H
#define SNPRINTF_H

#ifdef HAS_SNPRINTF
#ifdef LIBXT
#define _XtSnprintf snprintf
#define _XtVsnprintf vsnprintf
#endif
#ifdef LIBX11
#define _XSnprintf snprintf
#define _XVsnprintf vsnprintf
#endif
#else /* !HAS_SNPRINTF */

#ifdef LIBXT
#define snprintf _XtSnprintf
#define vsnprintf _XtVsnprintf
#endif
#ifdef LIBX11
#define snprintf _XSnprintf
#define vsnprintf _XVsnprintf
#endif

extern int snprintf (char *str, size_t count, const char *fmt, ...);
extern int vsnprintf (char *str, size_t count, const char *fmt, va_list arg);

#endif /* HAS_SNPRINTF */

#endif /* SNPRINTF_H */
