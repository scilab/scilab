#include <stdio.h>
#include <string.h>

/* 
   Gengatsce Xfilename1   Xfilename2   Xfilename3        Xfilename4
             functions     others.o      mex.c .f      startup generic
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char nam1[256]; char nam2[256]; char generic[256]; char nam4[256];
  char fname[256]; char suffix;
  FILE *filesce,*filegat,*filei1,*filei2,*filei3,*filei4;
  int i=0;
  int narg;
  narg=argc-1;
  if ((narg!=4) & (narg!=3)) return 1;
  if (narg==3) {
  if (NULL == (filesce = fopen ("startup.sce", "w"))) return 1;
  if (NULL == (filegat = fopen ("generic_gateway.c", "w"))) return 1;
  } else
    {
  strcpy(fname,argv[4]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/

  if (NULL == (filei4=fopen(fname+1,"r")))  return 1;
  /*  nam2 = the name;  nam1 (tmp) = nam2.sce = name of the Scilab script */
      fscanf(filei4,"%s",nam2); strcpy(nam1,nam2);strcat(nam1,".sce");
      fscanf(filei4,"%s",generic); strcpy(nam4,generic);strcat(nam4,"_gateway.c"); 
      fclose(filei4);
      if (NULL == (filesce = fopen (nam1, "w"))) return 1;
      if (NULL == (filegat = fopen (nam4, "w"))) return 1;
    }
  fprintf(filesce, "scilab_functions=[... \n");
  fprintf(filegat, "#include \"mex.h\" \n"); 
  fprintf(filegat, "      \n");


  strcpy(fname,argv[3]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  if (NULL == (filei3=fopen(fname+1,"r")))    return 1;

  /*  loop  */
  while (1)
    {
      int j;
      if ( fscanf(filei3,"%s",nam1) == EOF ) break;
      j= strlen(nam1);
      suffix=nam1[j-1];
      while ( (j != 0) && (nam1[j] != '.')) j--; 
      nam1[j]= '\0';
      if ((suffix=='c') | (suffix=='C')) fprintf(filegat, "extern Gatefunc %s;\n",nam1);
      if ((suffix=='f') | (suffix=='F')) fprintf(filegat, "extern Gatefunc C2F(%s);\n",nam1);
    }
  fclose(filei3);

  /*   ***********Open Xfilename1*************   */
  strcpy(fname,argv[1]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  /********* scilab_functions *************/
  if (NULL == (filei1=fopen(fname+1,"r"))) return 1;

  /* loop  */
  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      fprintf(filesce, "\"%s\";\n",nam1);
    }
  /* end loop */
  fprintf(filesce,"                 ]; \n");
  fprintf(filesce, "      \n");
  fclose(filei1);

  /* end loop */
  fprintf(filegat, "      \n");
  fprintf(filegat,"static GenericTable Tab[]={ \n"); 

  fprintf(filesce, "      \n");
  fprintf(filesce,"gateway_routines=[...\n"); 

  strcpy(fname,argv[3]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  if (NULL == (filei3=fopen(fname+1,"r")))    return 1;

  /*  loop  */
  while (1)
    {
      int j;
      if ( fscanf(filei3,"%s",nam1) == EOF ) break;
      j= strlen(nam1);
      suffix=nam1[j-1];
      while ( (j != 0) && (nam1[j] != '.')) j--; 
      nam1[j]= '\0';
      if ((suffix=='c') | (suffix=='C')) fprintf(filegat, "{mex_gateway, %s,\"err msg\"},\n",nam1);
      if ((suffix=='f') | (suffix=='F')) fprintf(filegat, "{fortran_mex_gateway, C2F(%s),\"err msg\"},\n",nam1);
      fprintf(filesce, "\"%s.o\";\n",nam1);
    }
  fprintf(filegat, "   }; \n");
  fprintf(filegat, "    \n");

  fprintf(filesce, "]; \n");
  fprintf(filesce, "    \n");
  
  fprintf(filegat, "int C2F(%s_gateway)() \n", generic);
  fprintf(filegat, "{  Rhs = Max(0, Rhs);   \n");
  fprintf(filegat,"(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F); \n");  
  fprintf(filegat,"return 0; \n");
  fprintf(filegat,"}; \n");


  strcpy(fname,argv[2]);
  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/

  /*     ********util_routines*************   */
  if (NULL == (filei2=fopen(fname+1,"r")))
    return 1;
fprintf(filesce,"util_routines =[...   \n");

  /*loop */
  while (1)
    {
      if ( fscanf(filei2,"%s",nam1) == EOF ) break;
      fprintf(filesce, "\"%s\";  \n", nam1);
    }
  /* end loop */
  fprintf(filesce, "             ]; \n");
  fclose(filei2);

  /*     *************G_make****************** */
fprintf(filesce,"object_files=G_make([\"%s_gateway.o\";gateway_routines;util_routines],\"%s.dll\"); \n", generic, generic);

fprintf(filesce,"addinter(object_files,\"%s_gateway\",scilab_functions); \n",generic);

  fclose(filesce);
  fclose(filegat);
  return 0;

}

