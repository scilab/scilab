#include <stdio.h>
#include <string.h>

/* 
   gensce Xfilename1 Xfilename2
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char nam1[256];  char fname[256]; char nam2[256];
  FILE *file,*filei1,*filei2;

  /*   ***********Open Xfilename2*************   */
  strcpy(fname,argv[2]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/

  if (NULL == (filei2=fopen(fname+1,"r")))  return 1;
  /*  nam2 = the library name;  nam1 (tmp) = nam2.sce = name of script file  */
      fscanf(filei2,"%s",nam2); strcpy(nam1,nam2); strcat(nam1,".sce");
      fclose(filei2);
      if (NULL == (file = fopen (nam1, "w"))) return 1;

  /*   ***********Open Xfilename1*************   */
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  /********* scilab_functions *************/
  if (NULL == (filei1=fopen(fname+1,"r"))) return 1;

fprintf(file, "scilab_functions=[... \n");
  /* loop  */
  while (1)
    {
      int j;
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      j= strlen(nam1);
      while ( j != 0 && nam1[j] != '/' ) j--; 
      nam1[j]= '\0';
      fprintf(file, "\"%s\";\n",nam1); 
    }
  /* end loop */
  fprintf(file,"                 ]; \n");
  fprintf(file, "      \n");
  fclose(filei1);
  /*  ends with addinter stuff   
      fprintf(file, "addinter(\"./%s.so\",\"%s_gateway\",scilab_functions); \n",nam2,nam2); */
  fclose (file) ;  return 0;
}

