/* Copyright INRIA/ENPC */

#include "../machine.h"
#include <stdio.h>

#if defined(__STDC__) || defined(__MSC__) || defined(__CYGWIN__) ||  (defined __ABSC__)
#include <stdlib.h>
#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif
#else 
extern  char  *getenv();
#endif

#if (defined __MSC__) || (defined __ABSC__) || defined(__MINGW32__) 
#ifdef __ABSC__
#define putenv(x) abs_putenv(x)
#define getpid() getpid_()
#else
#define putenv(x) _putenv(x)
#endif
#endif

extern void C2F(setprlev) __PARAMS((int*));

static char tmp_dir[256],buf[256];


/****************************
 * creates a tmp dir for a scilab session 
 * and fixes the TMPDIR env variable
 ****************************/

void C2F(settmpdir)(void)
{
  static int first =0;
  if ( first == 0 ) 
    {
      first++;
#ifdef WIN32 
  if (!getenv("TEMP")) {
    sprintf(tmp_dir,"C:/tmp/SD_%d_",(int) getpid());
  } else {
    sprintf(tmp_dir,"%s\\SD_%d_",getenv("TEMP"),(int) getpid());
  }
  SciCreateDirectory(tmp_dir);
#else 
  sprintf(tmp_dir,"/tmp/SD_%d_",(int) getpid());
  sprintf(buf,"umask 000;if test ! -d %s; then mkdir %s; fi ",tmp_dir,tmp_dir);
  system(buf);
#endif 
  sprintf(buf,"TMPDIR=%s",tmp_dir);
  putenv(buf);
    }
}

/****************************
 * get a reference to tmp_dir 
 ****************************/

char *get_sci_tmp_dir(void)
{
  /* just in case */
  C2F(settmpdir)();
  return tmp_dir;
}

/*************************************************
 * remove TMPDIR and dynamic link temporary files 
 *************************************************/

#if (defined(hppa))
  extern void hppa_sci_unlink_shared();
#endif

void C2F(tmpdirc)(void)
{
  char *tmp_dir = get_sci_tmp_dir(); 
#ifdef WIN32 
  SciRemoveDirectory(tmp_dir);
#else 
#if (defined(hppa))
  hppa_sci_unlink_shared();
#endif
  sprintf(buf,"rm -f -r %s >/dev/null  2>/dev/null",tmp_dir);
  system(buf);
  sprintf(buf,"rm -f -r /tmp/%d.metanet.* > /dev/null  2>/dev/null",
	  (int) getpid());
  system(buf);
#endif 
}
