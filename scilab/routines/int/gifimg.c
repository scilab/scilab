#include "../gd/gd.h"
#include "../machine.h"

extern void sciprint __PARAMS ((char *fmt,...));

void C2F(deallocategifimg)(im)
     gdImagePtr *im;
{
  gdImageDestroy(*im);
}

void C2F(readgifimg)(string,imgptr,m,n,ncol)
     char * string;
     gdImagePtr *imgptr;
     int *m,*n,*ncol;
{
  FILE *fd;
  gdImagePtr im;

  fd=fopen(string,"r");
    if (fd == 0) 
      {
	sciprint("Can't open file %s\n",string);
	return;
      }
  im = gdImageCreateFromGif(fd);
  if (im == 0) {
    *m = *n = -1;
    return;
      }
  *m = im->sy;
  *n = im->sx;
  *ncol = im->colorsTotal;
  *imgptr =im;
}
void C2F(readxbmimg)(string,imgptr,m,n,ncol)
     char * string;
     gdImagePtr *imgptr;
     int *m,*n,*ncol;
{
  FILE *fd;
  gdImagePtr im;

  fd=fopen(string,"r");
    if (fd == 0) 
      {
	sciprint("Can't open file %s\n",string);
	return;
      }
  im = gdImageCreateFromXbm(fd);
  if (im == 0) {
    *m = *n = -1;
    return;
      }
  *m = im->sy;
  *n = im->sx;
  *ncol = im->colorsTotal;
  *imgptr =im;
}


void C2F(getgifpixels)(imgptr,pixels)
     gdImagePtr *imgptr;
     unsigned char *pixels;
{
  int i,m,n,x,y;
  gdImagePtr im;
  im = *imgptr;
  m = im->sx;
  n = im->sy;
  i =0;
  for (x=0; (x<m); x++) {	
    for (y=0; (y<n); y++) {
      pixels[i] = im->pixels[y][x];
      i++;
    }
  }
}

void C2F(getgifcmap)(imgptr,cmap)
     gdImagePtr *imgptr;
     unsigned char *cmap;
{
  int i,k, ncol;
  gdImagePtr im;
  im = *imgptr;
  
  ncol = im->colorsTotal;
  k = 0;
  for (i=0; (i<ncol); i++) {
    cmap[k]        = im->red[i];
    cmap[ncol+k]   = im->green[i];
    cmap[2*ncol+k] = im->blue[i];
    k++;
  }
}
