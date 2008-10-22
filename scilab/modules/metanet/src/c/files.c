
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "files.h"
#include "PATH_MAX.h"

int CheckGraphName(char *name,char *dir)
{
#if (defined _MSC_VER)
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
#endif /**  _MSC_VER **/
}

char *StripGraph(char *name)
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

#ifndef _MSC_VER
char *my_basename (char *name)
{
  char *base;

  base = strrchr (name, '/');
  return base ? base + 1 : name;
}
#else
char * my_basename(char *name)
{
	char *base = NULL;

	if(name == NULL) return NULL;

	base = strrchr(name, '\\');
	if (base == NULL) strrchr(name, '/');

	if(base) return base + 1;

	if(isalpha(name[0] & 0xFF) && (name[1] == ':')) return (char *)(name + 2);
	return name;
}
#endif


#ifndef _MSC_VER
char* my_dirname (char *path)
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
      length = (int) (slash - path + 1);
    }
  newpath = (char *)MALLOC ((unsigned)length + 1);
  if (newpath == 0)
    return 0;
  strncpy (newpath, path, length);
  newpath[length] = 0;
  return newpath;
}
#else
char *my_dirname(char *path)
{
    static char buf[PATH_MAX + 1];
	char *newpath = NULL;
    size_t pathlen = strlen(path);
    int i;

    if (pathlen >= sizeof(buf)) return NULL;

    strcpy(buf, path);
    for (i = (int)pathlen; i >= 0; --i) {
        if (buf[i] == '/' || buf[i] == '\\') {
            buf[i] = '\0';
            break;
        }
    }
	newpath = (char *)MALLOC ( ((int)strlen(buf) + 1) * sizeof(char) );
	strcpy(newpath,buf);
    return newpath;
}
#endif
