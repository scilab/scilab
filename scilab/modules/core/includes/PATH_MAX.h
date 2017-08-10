/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#ifndef __PATH_MAX_H__
#define __PATH_MAX_H__


/* make sure we have PATH_MAX */
#if HAVE_LIMITS_H
# include <limits.h>
#endif
#if HAVE_SYS_PARAM
# include <sys/param.h>
#endif
#ifndef PATH_MAX
# ifndef _POSIX_PATH_MAX
#  ifndef MAXPATHLEN
#   define PATH_MAX 1024
#  else
#   define PATH_MAX MAXPATHLEN
#  endif
# else
#  define PATH_MAX _POSIX_PATH_MAX
# endif
#endif

#endif /* __PATH_MAX_H__ */
/*--------------------------------------------------------------------------*/
