#include <stdio.h>
#include <string.h>

/* 
 Forwin      Xfilename1   Xfilename2  makeall
             fct/mex.c     fvt/mex.f 
*/

int main(argc,argv)
     int argc;char *argv[];
{
  char nam1[256]; char nam2[256]; char nam4[512];
  char fname[256]; 
  FILE *filesce,*filegat,*filei1;
  int i=0;
  int narg;
  narg=argc-1;
  if (NULL == (filesce = fopen ("makeall", "w"))) return 1;
  if (NULL == (filegat = fopen ("objects", "w"))) return 1;

   strcpy(fname,argv[1]);  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;
  nam4[0]='\0'; strcat(nam4,"MEXOBJECTS = ");
  /*  loop  */
  while (1)
    {
      int j;int n; int k=0;
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      n=strlen(nam1);
      j=n;i=0;
      while ( (j != 0) && (nam1[j] != '/')) j--; 
      while ( (k < n) ) { nam2[k]=nam1[k+j+1]; k++;};
      nam1[j]= '\0';nam2[strlen(nam2)-2]='\0';
      strcat(nam4,nam2);strcat(nam4,".obj ");
      /*     printf("nam1=%s\nnam2=%s\n",nam1,nam2);  */
      fprintf(filesce, "%s.obj:\n\t@$(CC) $(CFLAGS) -DmexFunction=mex_%s -c %s.c -o %s.obj\n", nam2,nam1,nam2,nam2);
    }
  fclose(filei1);
  strcpy(fname,argv[2]);  fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;

  /*  loop  */
  while (1)
    {
      int j;int n; int k=0;
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      n=strlen(nam1);
      j=n;i=0;
      while ( (j != 0) && (nam1[j] != '/')) j--; 
      while ( (k < n) ) { nam2[k]=nam1[k+j+1]; k++;};
      nam1[j]= '\0';nam2[strlen(nam2)-2]='\0';
      strcat(nam4,nam2);strcat(nam4,".obj ");
      /*      printf("nam1=%s\nnam2=%s\n",nam1,nam2); */
      fprintf(filesce, "%s.obj:\n\t@$(FC) $(FFLAGS)  %s.F \n", nam2,nam2);
    }
  fprintf(filegat,"%s\n",nam4);
  fclose(filei1);
  fclose(filesce);
  return 0;

}
 
