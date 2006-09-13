#include "gifimg.h"
#include "machine.h"
#include "sciprint.h"

void C2F(deallocategifimg)( gdImagePtr * im )
{
  gdImageDestroy(*im);
}

void deallocateGifImg( gdImagePtr * im ) { C2F(deallocategifimg)( im ) ; }

void C2F(readgifimg)( char * string, gdImagePtr * imgptr, int * m,int * n, int * ncol )
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

void readGifImg( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol) { C2F(readgifimg)( string, imgptr, m, n, ncol ) ; }

void C2F(readxbmimg)( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol )
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

void readXbmImg( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol) { C2F(readxbmimg)( string, imgptr, m, n, ncol ) ; }

void C2F(getgifpixels)( gdImagePtr * imgptr, unsigned char * pixels )
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

void getGifPixels( gdImagePtr * imgptr, unsigned char * pixels ) { C2F(getgifpixels)( imgptr, pixels ) ; }

void C2F(getgifcmap)( gdImagePtr * imgptr, unsigned char * cmap )
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

void getGifCmap( gdImagePtr * imgptr, unsigned char * cmap ) { C2F(getgifcmap)( imgptr, cmap ) ; }
