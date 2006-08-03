#include <stdio.h>
#include <string.h>

void StringConvrt(str)
     char *str;
{
  char *str1;
  str1 = str;
  while ( *str != 0) 
    {
      if ( *str == '\\' ) 
	{
	  switch ( *(str+1)) 
	    {
	    case 'n' : *str1 = '\n' ; str1++; str += 2;break;
	    case 't' : *str1 = '\t' ; str1++; str += 2;break;
	    case 'r' : *str1 = '\r' ; str1++; str += 2;break;
	    default : *str1 = *str; str1++; str++;break;
	    }
	}
      else 
	{
	  *str1 = *str; str1++; str++;
	}
    }
  *str1 = '\0';
}

/* 
   Util      Xfilename1   Xfilename2    Xfilename3   Xfilename4 Xfilename5
               format       list1         list2         list3     writeto
====>writeto:
list1(1)list2(1)list3(1)
list1(2)list2(2)list3(2)
using format
............
etc

*/

int main(argc,argv)
     int argc;char *argv[];
{
  char fname[256],nam1[256],nam2[256],nam3[256],nam4[256];
  char format[256]; 
  FILE *filesce,*filei1,*filei2,*filei3,*filei4;

  int narg,i;  narg=argc-1;

  /*            filesce=writeto     */
  strcpy(fname,argv[narg]); fname[strlen(fname)]= '\0';
  /** we use fname +1 since msvc makefile temporary filenames 
      begin whith extra @ char **/

  if (NULL == (filei1=fopen(fname+1,"r")))  return 1;
  fscanf(filei1,"%s",nam1); fclose(filei1);
  if (NULL == (filesce = fopen (nam1, "a"))) return 1;

  /*   ***********  Xfilename1 = format*************   */
  strcpy(fname,argv[1]);  fname[strlen(fname)]= '\0';
  if (NULL == (filei1=fopen(fname+1,"r"))) return 1;
  i=0;format[0]='\0';
  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      if (i>0) strcat(format," "); 
      i++;
      strcat(format,nam1);
    }
  /*  printf("FORMAT=%s\n",format); */
  fclose(filei1);
  StringConvrt(format);
  /*                  lists                */
  if (narg==3) {
  strcpy(fname,argv[2]); fname[strlen(fname)]= '\0';
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;

  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      fprintf(filesce,format,nam1);
    }
  fclose(filei1);
  }
  if (narg==4) {
  strcpy(fname,argv[2]); fname[strlen(fname)]= '\0';
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[3]); fname[strlen(fname)]= '\0';
  if (NULL == (filei2=fopen(fname+1,"r")))    return 1;

  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      if ( fscanf(filei2,"%s",nam2) == EOF ) break;
      fprintf(filesce,format,nam1,nam2);
    }

fclose(filei1);
fclose(filei2);
  }
  if (narg==5) {
  strcpy(fname,argv[2]); fname[strlen(fname)]= '\0';
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[3]); fname[strlen(fname)]= '\0';
  if (NULL == (filei2=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[4]); fname[strlen(fname)]= '\0';
  if (NULL == (filei3=fopen(fname+1,"r")))    return 1;

  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      if ( fscanf(filei2,"%s",nam2) == EOF ) break;
      if ( fscanf(filei3,"%s",nam3) == EOF ) break;
      fprintf(filesce,format,nam1,nam2,nam3);
    }

fclose(filei1);
fclose(filei2);
fclose(filei3);
  }
  if (narg==6) {
  strcpy(fname,argv[2]); fname[strlen(fname)]= '\0';
  if (NULL == (filei1=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[3]); fname[strlen(fname)]= '\0';
  if (NULL == (filei2=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[4]); fname[strlen(fname)]= '\0';
  if (NULL == (filei3=fopen(fname+1,"r")))    return 1;
  strcpy(fname,argv[5]); fname[strlen(fname)]= '\0';
  if (NULL == (filei4=fopen(fname+1,"r")))    return 1;

  while (1)
    {
      if ( fscanf(filei1,"%s",nam1) == EOF ) break;
      if ( fscanf(filei2,"%s",nam2) == EOF ) break;
      if ( fscanf(filei3,"%s",nam3) == EOF ) break;
      if ( fscanf(filei4,"%s",nam4) == EOF ) break;
      fprintf(filesce,format,nam1,nam2,nam3,nam4);
    }

fclose(filei1);
fclose(filei2);
fclose(filei3);
fclose(filei4);
  }
  fclose(filesce);
  return 0;
}






