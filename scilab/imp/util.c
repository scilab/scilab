#include "util.h" 

/*-----------------------------------------------
 * read one line of input text un buff 
 * with reallocation if necessary 
 *---------------------------------------------------*/

void read_one_line(char **buff,int *stop,FILE *fd,int *buflen)
{ 
  int i ,c ;
  for ( i = 0 ;  (c =getc(fd)) !=  '\n' && c != EOF ; i++) 
    {
      if ( i == *buflen -1 ) 
	{
	  *buflen += 512;
	  *buff = realloc(*buff,*buflen*sizeof(char));
	  if ( *buff == NULL) 
	    {
	      fprintf(stderr,"Running out of space \n");
	      exit(1);
	    }
	}
      (*buff)[i]= c ;
    }
  if ( i+1 >= *buflen - 1 ) 
    {
      *buflen += 512;
      *buff = realloc(*buff,*buflen*sizeof(char));
      if ( *buff == NULL) 
	{
	  fprintf(stderr,"Running out of space \n");
	  exit(1);
	}
    }
  (*buff)[i]='\n';
  (*buff)[i+1]='\0';
  if ( c == EOF) {*stop = 1;}
}

