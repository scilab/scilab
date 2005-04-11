#define MAXCHARS 256
#define MAXLINE 80
#include "gd.h"
#include <string.h>
#include <stdlib.h>

#define BITN(x,n) ( (x>>n) & 0x1 ) 
#define Max(x,y)	(((x)>(y))?(x):(y))

/* extern gdFontPtr FontPtr;*/

int GetTag(s,tag)
     char *s, *tag;
{
  int i;
  i=0;
  while (*s) {
    tag[i++] = *s++;
    if (*s == ' ' || *s == '\t' || *s == '\n') {
      tag[i] = '\0';
      return i++;
    }
  }
  return i; /* Correction warning missing return statement at end of non-void function */
}

void dispchar(C,height,width)
     char *C;
     int height,width;
{
  int i,j,k;
  k=0;
  for (i=0;i<height;i++) {
    for (j=0;j<width;j++) {
      printf("%d",C[k]);
      k++;
    }
    printf("\n");
  }
  printf("\n");
}

int ReadbdfFont(f,FontPtr,FontName)
     FILE *f;
     gdFontPtr FontPtr;
     char *FontName;

{
  char s[MAXLINE],hex[3],Spacing[4];
  char tag[MAXLINE];
  int curptr, curptrs, curcharptr;
  int  bottom, width, currentchar, globalleft, globaltop, left;
  int height,hei,wid, gobitmap, bot;
  char *Data, *data;
  int i, j, nrow, nbytes,k ;
  int c;

  FontPtr->fixed =0;
  gobitmap =0;
  hex[2]=0;

  while (fgets(s,MAXLINE,f)) 
    {
      GetTag(s,tag); 
      if (!strcmp(tag,"ENCODING")){
	sscanf(s,"%s %d",tag,&currentchar);
      }
      else if (!strcmp(tag,"FONT")){
	sscanf(s,"%s %s",tag,FontName);
      }
      else if (!strcmp(tag,"SPACING")){
	char buff[256];
	int nbchar = sprintf(buff,"%%%ds %%3s",MAXLINE-4);
	buff[nbchar+1]='\0';

	sscanf(s,buff,tag,Spacing);
	if (Spacing[1]=='M') FontPtr->fixed=1;
      }
      else if (!strcmp(tag,"FONTBOUNDINGBOX")){
	sscanf(s,"%s %d %d %d %d",tag,&width,&height,&globalleft,&globaltop);
	Data = (char *)calloc(256*width*height,sizeof(char));
	if (Data == 0) return 1;
	data = (char *)calloc(width*height+8,sizeof(char));
	if (Data == 0) {
	  free(Data);
	  return 1;
	}
	FontPtr->nchars = 256;
	FontPtr->offset = 0; /* ? */
	FontPtr->w = width;
	FontPtr->h = height;
	FontPtr->data = Data;
	for (i=0;i<256*width*height;i++) Data[i]=0;
      }
      else if (!strcmp(tag,"BBX")){
	sscanf(s,"%s %d %d %d %d",tag,&wid,&hei,&left,&bottom);
      }
      else if (!strcmp(tag,"BITMAP")){
	if (currentchar>=0) {
	  gobitmap = 1;
	  nrow = 0;
	  curcharptr = width*height*currentchar;
	  for (i=0; i<width*height; i++) data[i]=0;
	}
	curptrs = 0;
      }

      else if (!strcmp(tag,"ENDCHAR")){
	gobitmap = 0;
	if (currentchar>=0) {
	  bot = globaltop - bottom;
	  if (bot <= 0) {
	    k=curcharptr+(height-nrow+bot)*width;
	    for (i=0; i<nrow*width; i++)
	      Data[k + i]=data[i];
	  }
	}
      }
      else if (gobitmap) {
	curptr = Max(curptrs+left,0); /** Max added : jpc Jul 99 **/
	
	nbytes=strlen(s)/2;
	for (j=0 ; j<nbytes ; j++) {
	  hex[0]=s[2*j];
	  hex[1]=s[2*j+1];
	  sscanf(hex,"%x",&c);
	  for (i=7 ; i>=0 ; i--) {
	    data[curptr++] = (char)(BITN(c,i) != 0);
	  }
	}

	/* ?????? */
        if (curptrs >0)
	  data[curptrs-1]=0;
	
        curptrs = curptrs + width;
	nrow++;
      }
    }
  free(data);
  return 0;
}

