/****************************************************************
 * July 5, 1991
 * Copyright 1991 Lance Norskog And Sundry Contributors
 * This source code is freely redistributable and may be used for
 * any purpose.  This copyright notice must be maintained. 
 * Lance Norskog And Sundry Contributors are not responsible for 
 * the consequences of using this software.
 ****************************************************************/

/**************************************************************
 * file Tools miscellaneous stuff.
 ****************************************************************/
#include <stdio.h>
#include "../../modules/core/includes/machine.h" /* HAVE_STRERROR */
#include "misc.h"
/*-----------------------------------------------------------------------------------*/
#ifndef _MSC_VER
char *sizes[] = {
  "NONSENSE!",
  "bytes",
  "shorts",
  "NONSENSE",
  "longs",
  "32-bit floats",
  "64-bit floats",
  "IEEE floats"
};

char *styles[] = {
  "NONSENSE!",
  "unsigned",
  "signed (2's complement)",
  "u-law",
  "a-law"
};
#endif
char readerr[] = "Premature EOF while reading sample file.";
char writerr[] = "Error writing sample file.  You are probably out of disk space.";
/*-----------------------------------------------------------------------------------*/
/* Utilities */
/* Read short, little-endian: little end first. VAX/386 style. */
unsigned short rlshort(ft_t ft)
{
  unsigned char uc, uc2;
  uc  = getc(ft->fp);
  uc2 = getc(ft->fp);
  return (uc2 << 8) | uc;
}
/*-----------------------------------------------------------------------------------*/
/* Read short, bigendian: big first. 68000/SPARC style. */
unsigned short rbshort(ft_t ft)
{
  unsigned char uc, uc2;
  uc2 = getc(ft->fp);
  uc  = getc(ft->fp);
  return (uc2 << 8) | uc;
}
/*-----------------------------------------------------------------------------------*/
/* Write short, little-endian: little end first. VAX/386 style. */
void wlshort(ft_t ft, short unsigned int us)
{
  putc(us, ft->fp);
  putc(us >> 8, ft->fp);
  if (ferror(ft->fp))
  {
      printf("%s\n",writerr);
      ft->ierr=1;
  }
}
/*-----------------------------------------------------------------------------------*/
/* Write short, big-endian: big end first. 68000/SPARC style. */
void wbshort(ft_t ft, short unsigned int us)
{
  putc(us >> 8, ft->fp);
  putc(us, ft->fp);
  if (ferror(ft->fp))
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Read long, little-endian: little end first. VAX/386 style. */
unsigned long rllong(ft_t ft)
{
  unsigned char uc, uc2, uc3, uc4;
  uc  = getc(ft->fp);
  uc2 = getc(ft->fp);
  uc3 = getc(ft->fp);
  uc4 = getc(ft->fp);
  return ((long)uc4 << 24) | ((long)uc3 << 16) | ((long)uc2 << 8) | (long)uc;
}
/*-----------------------------------------------------------------------------------*/
/* Read long, bigendian: big first. 68000/SPARC style. */
unsigned long rblong(ft_t ft)
{
  unsigned char uc, uc2, uc3, uc4;
  uc  = getc(ft->fp);
  uc2 = getc(ft->fp);
  uc3 = getc(ft->fp);
  uc4 = getc(ft->fp);
  return ((long)uc << 24) | ((long)uc2 << 16) | ((long)uc3 << 8) | (long)uc4;
}
/*-----------------------------------------------------------------------------------*/
/* Write long, little-endian: little end first. VAX/386 style. */
void wllong(ft_t ft, long unsigned int ul)
{
  int datum;
  datum = (ul) & 0xff;
  putc(datum, ft->fp);
  datum = (ul >> 8) & 0xff;
  putc(datum, ft->fp);
  datum = (ul >> 16) & 0xff;
  putc(datum, ft->fp);
  datum = (ul >> 24) & 0xff;
  putc(datum, ft->fp);
  if (ferror(ft->fp))
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Write long, big-endian: big end first. 68000/SPARC style. */
void wblong(ft_t ft, long unsigned int ul)
{
  int datum;

  datum = (ul >> 24) & 0xff;
  putc(datum, ft->fp);
  datum = (ul >> 16) & 0xff;
  putc(datum, ft->fp);
  datum = (ul >> 8) & 0xff;
  putc(datum, ft->fp);
  datum = (ul) & 0xff;
  putc(datum, ft->fp);
  if (ferror(ft->fp))
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Read and write words and longs in "machine format".  Swap if indicated. */
/* Read short. */
unsigned short rshort(ft_t ft)
{
  unsigned short us;
  fread(&us, 2, 1, ft->fp);
  if (ft->swap)
    us = swapw(us);
  return us;
}
/*-----------------------------------------------------------------------------------*/
/* Write short. */
void wshort(ft_t ft, short unsigned int us)
{
  if (ft->swap)
    us = swapw(us);
  if (fwrite(&us, 2, 1, ft->fp) != 1)
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Read a long. : note that long size is machine dependant  */
unsigned long rlong(ft_t ft)
{
  unsigned long ul;
  fread(&ul, sizeof(long), 1, ft->fp);
  if (ft->swap)
    ul = swapl(ul);
  return ul;
}
/*-----------------------------------------------------------------------------------*/
/* Write long. */
void wlong(ft_t ft, long unsigned int ul)
{
  if (ft->swap)
    ul = swapl(ul);
  if (fwrite(&ul,sizeof(long), 1, ft->fp) != 1)
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Read float. */
float rfloat(ft_t ft)
{
  float f;
  fread(&f, sizeof(float), 1, ft->fp);
  if (ft->swap)
    f = swapf(f);
  return f;
}
/*-----------------------------------------------------------------------------------*/
void wfloat(ft_t ft, float f)
{
  float t = f;
  if (ft->swap)
    t = swapf(t);
  if (fwrite(&t, sizeof(float), 1, ft->fp) != 1)
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* Read double. */
double rdouble(ft_t ft)
{
  double d;
  fread(&d, sizeof(double), 1, ft->fp);
  if (ft->swap)
    d = swapd(d);
  return d;
}
/*-----------------------------------------------------------------------------------*/
/* Write double. */
void wdouble(ft_t ft, double d)
{
  if (ft->swap)
    d = swapd(d);
  if (fwrite(&d, sizeof(double), 1, ft->fp) != 1)
    {
      printf("%s\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* generic swap routine */
void swapb(char *l, char *f, int n)
{    
  int i;
  for (i= 0; i< n; i++) f[i]= l[n-i-1];
}
/*-----------------------------------------------------------------------------------*/
/* Byte swappers */
unsigned short swapw(unsigned short us)
{
  return ((us >> 8) | (us << 8)) & 0xffff;
}
/*-----------------------------------------------------------------------------------*/
/** swapl : swap a long : note that a long size is machine dependant **/
unsigned long swapl(long unsigned int ul)
{
  unsigned long  sdf;
  swapb((char *) &ul,(char *) &sdf, sizeof(unsigned long));
  return (sdf);
}
/*-----------------------------------------------------------------------------------*/
/** swap an int assumed to be on 4 bytes **/
unsigned int swapi(unsigned int ul)
{
  return (ul >> 24) | ((ul >> 8) & 0xff00) | ((ul << 8) & 0xff0000) | (ul << 24);
}
/*-----------------------------------------------------------------------------------*/
/**
 * return swapped 32-bit float 
 * @param uf
 * @return 
 */
float swapf(float uf)
{
  if (sizeof(long) == sizeof(float)){
    union {
      unsigned long l;  /** we assume here long is 4 bytes **/
      float f;
    } u;
    u.f= (float)uf;
    u.l= (u.l>>24) | ((u.l>>8)&0xff00) | ((u.l<<8)&0xff0000) | (u.l<<24);
    return u.f;
  }
  else {
    union {
      unsigned int l;  /** we assume here int is 4 bytes **/
      float f;
    } u;
    u.f= (float) uf;
    u.l= (u.l>>24) | ((u.l>>8)&0xff00) | ((u.l<<8)&0xff0000) | (u.l<<24);
    return u.f;
  }
}
/*-----------------------------------------------------------------------------------*/
double swapd(double df)
{
  double sdf;
  swapb((char *) &df,(char *) &sdf, sizeof(double));
  return (sdf);
}
/*-----------------------------------------------------------------------------------*/
#ifndef HAVE_STRERROR
/* strerror function */
char *strerror(int errcode)
{
  static char  nomesg[30];
  extern int sys_nerr;
  extern char *sys_errlist[];
  if (errcode < sys_nerr)
    return (sys_errlist[errcode]);
  else
    {
      sprintf (nomesg, "Undocumented error %d", errcode);
      return (nomesg);
    }
}
#endif
/*-----------------------------------------------------------------------------------*/

