#include <stdio.h>
#if !(defined __MSC__)&& !(defined __MINGW32__)
#if defined(netbsd) || defined(freebsd)
#include <sys/types.h>
#endif
#include <dirent.h>
#endif
#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#define MAXNAM 80

int CheckGraphName(name,dir)
char *name;
char *dir;
{
#if (defined __MSC__) || (defined __MINGW32__)
  return(0);
#else 
  DIR *dirp;
  struct dirent *dp;
  char *s;
  char t[MAXNAM];
  int i;

  dirp = opendir(dir);
  for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {
    s = dp->d_name;
    i = 0;
    while ( (t[i++] = *s++) ) {
      if (*s == '.') {
	s++; t[i] = '\0';
	if (strcmp(s,"graph") == 0 && strcmp(t,name) == 0) {
	  closedir(dirp);
	  return 1;
	}
      break;
      }
    }
  }
  closedir(dirp);
  return 0;
#endif /**  __MSC__ **/
}

char *StripGraph(name)
char *name;
{
  char *s;
  int i;
  char *t;

  s = name;
  i = 0;
  t = (char *)MALLOC((unsigned)strlen(name)+1);
  while ( (t[i++] = *s++) ) {
    if (*s == '.') {
      if (strcmp(++s,"graph") == 0) {
	t[i] = '\0'; 
	return t;
      }
      t[i++] = '.';
    }
  }
  return name;
}

char *my_basename (name)
char *name;
{
  char *base;

  base = strrchr (name, '/');
  return base ? base + 1 : name;
}

char* dirname (path)
char *path;
{
  char *newpath;
  char *slash;
  int length;    /* Length of result, not including NUL. */

  slash = strrchr (path, '/');
  if (slash == 0)
    {
      /* File is in the current directory.  */
      path = ".";
      length = 1;
    }
  else
    {
      /* Remove any trailing slashes from result. */
      while (slash > path && *slash == '/')
	--slash;
      length = slash - path + 1;
    }
  newpath = (char *)MALLOC ((unsigned)length + 1);
  if (newpath == 0)
    return 0;
  strncpy (newpath, path, length);
  newpath[length] = 0;
  return newpath;
}
