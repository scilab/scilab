#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include "../stack-c.h"
int C2F(intfilestat)(fname)
char * fname;
{

#ifdef WIN32
   struct _stat buf;
#else
   struct stat buf;
#endif
   int result, m1, n1, l1 , l2,one=1;

   CheckRhs(1,1);
   CheckLhs(1,2);
   GetRhsVar(1, "c", &m1, &n1, &l1); /* get file name */

   /* Get data associated with "given file": */
#ifdef WIN32
   result = _stat(cstk(l1), &buf );
#else
   result = stat(cstk(l1), &buf );
#endif
   /* Check if statistics are valid: */
   if( result != 0 ) {
     n1=0;
     CreateVar(2,"d",&n1,&n1,&l2);}
   else   {
     n1 = 6;
     CreateVar(2,"d",&one,&n1,&l2);
     *stk(l2+0) = (double) buf.st_size;
     *stk(l2+1) = (double) buf.st_mode;
     *stk(l2+2) = (double) buf.st_uid;
     *stk(l2+3) = (double) buf.st_gid;
     *stk(l2+4) = (double) buf.st_dev;
     *stk(l2+5) = (double) buf.st_mtime;
   }
  LhsVar(1) = 2;
  if (Lhs==2) {
    CreateVar(3,"d",&one,&one,&l2);
    *stk(l2) = (double) result;
    LhsVar(2) = 3;
  }
  C2F(putlhsvar)();
  return 0;
}
