#include "../gd/gd.h"
#include "../machine.h"
void C2F(deallocategifimg)(im);
gdImagePtr **im;
{
  gdImageDestroy(*im);
}

void C2F(readgifimg)(buf,imgptr,m,n,ncol)
char * buf;
gdImagePtr **imgptr;
int *m,*n,*ncol;
{
FILE *fd;
gdImagePtr *im;

fd=fopen(string1,"w")
  if (fd == 0) 
    {
      sciprint("Can't open file %s\n");
      return;
    }
 im = gdImageCreateFromGif(fd);
 *m = im->sx;
 *n = im->sy;
 *ncol = im->colorsTotal;
}

void C2F(readxbmimg)(buf,imgptr,m,n,ncol)
char * buf;
gdImagePtr **imgptr;
int *m,*n,*ncol;
{
FILE *fd;
gdImagePtr *im;

fd=fopen(string1,"w")
  if (fd == 0) 
    {
      sciprint("Can't open file %s\n");
      return;
    }
 im = gdImageCreateFromXbm(fd);
 *m = im->sx;
 *n = im->sy;
 *ncol = im->colorsTotal;
}

void C2F(getgifpixels)(imgptr,pixels)
gdImagePtr **imgptr;
unsigned char *pixels;
{
  int i,m,n,x,y;
  gdImagePtr *im;
  im = *imgptr;
  m = im->sx;
  n = im->sy;
  i =0;
  for (y=0; (y<n); y++) {
    for (x=0; (x<m); x++) {	
      pixels[i] = im->pixels[y][x];
      i++;
    }
  }
}

void C2F(getgifcmap)(imgptr,cmap)
gdImagePtr **imgptr;
unsigned char *cmap;
{
  int i,k, ncol;
  unsigned char *c1;
  gdImagePtr *im;
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
