/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
