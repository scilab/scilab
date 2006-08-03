#include <stdio.h>
#include <string.h>

/* 
   gensomex Xfilename1 Xfilename2
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char nam1[256];  char fname[256]; char nam2[256];
  char suffix;
  FILE *file,*filei1,*filei2;

  /*   ***********Open Xfilename2*************   */
  strcpy(fname,argv[2]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/

  if (NULL == (filei2=fopen(fname+1,"r")))  return 1;
  /*  nam2 = the library name;  nam1 (tmp) = nam2_gateway.c = name of C file */
      fscanf(filei2,"%s",nam2); strcpy(nam1,nam2); 
      strcat(nam1,"_gateway.c");
      fclose(filei2);
      if (NULL == (file = fopen (nam1, "w"))) return 1;

  /*   ***********Open Xfilename1*************   */
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  /********* scilab_functions *************/
  if (NULL == (filei1=fopen(fname+1,"r"))) return 1;

  fprintf(file,"#include \"mex.h\"\n");
  fprintf(file, "      \n");
  /* loop  */
  while (1)
    {
      int j;
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      j= strlen(nam1);
      suffix=nam1[j-1];
      while ( j != 0 && nam1[j] != '/' ) j--; 
      nam1[j]= '\0';
      if ((suffix=='c') | (suffix=='C')) fprintf(file, "extern Gatefunc mex_%s;\n",nam1);
      if ((suffix=='f') | (suffix=='F')) fprintf(file, "extern Gatefunc C2F(mex%s);\n",nam1);
    }
  /* end loop */
  fprintf(file, "      \n");
  fclose(filei1);
  /*   ***********Re-Open Xfilename1*************   */
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  /********* scilab_functions *************/
  if (NULL == (filei1=fopen(fname+1,"r"))) return 1;

  fprintf(file, "static GenericTable Tab[]={ \n");
  /* loop  */
  while (1)
    {
      int j;
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      j= strlen(nam1);
      suffix=nam1[j-1];
      while ( j != 0 && nam1[j] != '/' ) j--; 
      nam1[j]= '\0';
      if ((suffix=='c') | (suffix=='C')) fprintf(file, "{mex_gateway, mex_%s,\"err msg\"},\n",nam1);
      if ((suffix=='f') | (suffix=='F')) fprintf(file, "{(Myinterfun)fortran_mex_gateway, C2F(mex%s),\"err msg\"},\n",nam1);
     }
  /* end loop */
  fprintf(file, "                          };      \n");
  fprintf(file, "      \n");
  /*  fprintf(file, "int C2F(libmylib_gateway)()     \n");
  fprintf(file, "{  Rhs = Max(0, Rhs); \n");
  fprintf(file,"(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F); \n");
  fprintf(file,"  return 0; \n");
  fprintf(file,"}\n");
  fprintf(file, "      \n"); */
  fclose(filei1);





  /*  ends with Table of functions   */
/* fprintf(file, "int C2F(\"./%s.so\",\"%s_gateway.o\",scilab_functions); \n",nam2,nam2); */
  fclose (file) ;  return 0;
}

