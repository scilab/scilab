/* LineRead procedure reads lines in ascii files coming from Unix Dos or Mac 
 * End of line may be \Cr\lf (Dos) \Lf (Unix) \Cr (Mac)
 */
#include <stdio.h>
#include "../machine.h"

#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif 

#define CR 13
#define LF 10
FILE *GetFile();
int LineRead(fd,buf,n,cnt,nr)
     FILE * fd;
     int n;
     char buf[];
     int *cnt; /* number of charaters returned in buf */
     int *nr; /* number of characters read */
{
  int c,count,info;
  long int offset;

  count=0;
  *nr=0;
  
  while (1) {
    c=fgetc(fd);
    *nr=*nr+1;
    if (c==LF) { /* LF reached first: unix file */
      buf[count++]=(char)0;
      info=1; /* EOL reached */
      break;
    }
    else if (c==CR) { /* CR reached first: Dos or Mac file */
      c=fgetc(fd);
      *nr=*nr+1;
      if (c==EOF) {
	buf[count++]=(char)0;
	info=0;/* EOF reached after an EOL */
	break;
      }
    else if (c==LF) { /* LF after CR : Dos file */
	buf[count++]=(char)0;
	info=1; /* EOL reached */
	break;
      }
      else if (c!=LF) { /* Mac file */
	offset=-1;
	fseek(fd,offset,SEEK_CUR);
	*nr=*nr-1;
	buf[count++]=(char)0;
	info=1;  /* EOL reached */
	break;
      }
    }
    else if (c==EOF) { /* EOF reached before any EOL*/
      buf[count++]=(char)0;
      if (count==1) 
	info=-1; /* EOF reached */
      else
	info=3;
      break;
    }
    else {
      buf[count++]=(char)c;
      if (count==n-1) {
	buf[count++]=(char)0;
	info=2; /* buffer full */
	break;
      }
    }
  }
  *cnt=count;
  return(info);
}
 
void C2F(readnextline)(fd,buf,n,count,nr,ierr)
     integer *fd;
     integer *n,*nr, *ierr;
     char buf[];
     integer *count;
{

FILE *fa= GetFile(fd);

*ierr=LineRead(fa,buf,*n,count,nr);

}

