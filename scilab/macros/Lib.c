#include <stdio.h>
#include <string.h>


/* 
   Lib file or Lib file 
   Lib File : generates : tmp_Macros tmp_Bin 
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char name[256];
  char fname[256];
  FILE *filei,*fileo2,*fileo3;
  int i=0;
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extrac @ char **/
  if (NULL == (filei=fopen(fname+1,"r")))
    return 1;
  /**********************/
  if (NULL == (fileo2 = fopen ("tmp_Macros", "w")))   return 1;
  if (NULL == (fileo3 = fopen ("tmp_Bin", "w")))   return 1;
  fprintf(fileo2,"mode(0);\n");
  while (1)
    {
      int j;
      if ( fscanf(filei,"%s",name) == EOF ) break;
      j= strlen(name);
      while ( j != 0 && name[j] != '.' ) j--; 
      name[j]= '\0';
      fprintf(fileo2,"getf('%s.sci'),save('%s.bin'),clear ;\n",name,name);
      fprintf(fileo3,"%s.bin\n",name);
      fprintf(stdout,"Processing file %s.sci\n",name);
    }
  fprintf(fileo2,"exit;\n");
  fclose (fileo2) ;
  fclose (fileo3) ;
  return 0;
}




