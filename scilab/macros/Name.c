#include <stdio.h>
#include <string.h>


/* 
   Name File 
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char name[256],nam1[256],nam2[256];
  char fname[256];
  FILE *file,*filei;
  int i=0;
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  if (NULL == (filei=fopen(fname+1,"r")))
    return 1;
  fscanf(filei,"%s",nam1);
  fscanf(filei,"%s",nam2);
  /**********************/
  if (NULL == (file = fopen ("genlib", "w"))) return 1;
  fprintf(file,"%s=lib('%s/');save('%s/lib',%s);exit\n",nam2,nam1,nam1,nam2);
  fclose (file) ;
  /**********************/
  if (NULL == (file = fopen ("names", "w"))) return 1;
  while (1)
    {
      int j;
      char *c;
      if ( fscanf(filei,"%s",name) == EOF ) break;
      j= strlen(name);
      while ( j != 0 && name[j] != '.' ) j--; 
      name[j]= '\0';
      fprintf(file,"%s\n",name);
    }
  fclose (file) ;
  return 0;
}

