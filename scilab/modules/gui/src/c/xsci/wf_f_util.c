/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 *
 * "Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the copyright
 * notice and this permission notice appear in supporting documentation. 
 * No representations are made about the suitability of this software for 
 * any purpose.  It is provided "as is" without express or implied warranty."
 */

#if defined(netbsd) || defined(freebsd)
#include <unistd.h>
#endif

#include "wf_fig.h"
#include "wf_resources.h"

#include "wf_mode.h"
#include "wf_w_util.h"

#include "../machine.h"
#include "All-extern.h"

int
emptyname(name)
    char	    name[];

{
  if (*name == '\0') {
    return (1);
  } else {
    return (0);
  }
}

int emptyname_msg(name, msg)
     char	    name[], msg[];
{
  int		    returnval;
  if ( (returnval = emptyname(name)) == 1)
    put_msg("No file name specified, %s command ignored", msg);
  return (returnval);
}

int
change_directory(path)
    char	   *path;
{
    if (path == NULL) {
	*cur_dir = '\0';
	return (0);
    }
    /**** pour que le changement de directory soit non effectif 
    if (chdir(path) == -1) {
	put_msg("Can't go to directory %s, : %s", path, sys_errlist[errno]);
	return (1); 
    } ****/
    return(0);
    /** 
      if (get_directory())
	return (0);
    else
	return (1);
	**/
}

int 
get_directory()
{
#if defined(SYSV) || defined(SVR4) || defined(netbsd) || defined(freebsd)
    extern char	   *getcwd();

#else
    extern char	   *getwd();

#endif

#if defined(SYSV) || defined(SVR4)
    if (getcwd(cur_dir, 1024) == NULL) {	/* get current working dir */
	put_msg("Can't get current directory");
#else
    if (getwd(cur_dir) == NULL) {/* get current working dir */
	put_msg("%s", cur_dir);	/* err msg is in directory */
#endif
	*cur_dir = '\0';
	return 0;
    }
    return 1;
}
