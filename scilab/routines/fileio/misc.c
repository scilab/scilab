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
#include "../machine.h"
/*-----------------------------------------------------------------------------------*/
#define IMPORT  extern
#define EXPORT 

#ifdef __MINGW32__
#define HAVE_STRERROR
#endif 

#define	PRIVSIZE	100
#define NLOOPS		8

#if	defined(__STDC__) || defined(ARM)
	#define	P1(x) x
#else
	#define P1(x)
#endif
/*-----------------------------------------------------------------------------------*/
double 	       swapd(P1(double d));			/* Swap double */
unsigned short swapw(P1(unsigned short us));		/* Swap short */
unsigned long  swapl(P1(unsigned long ul));		/* Swap long */
float  	       swapf(P1(float f));			/* Swap float */
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
struct  signalinfo {
	long		rate;		/* sampling rate */
	int		size;		/* word length of data */
	int		style;		/* format of sample numbers */
	int		channels;	/* number of sound channels */
	};

struct  instrinfo {
	char 		MIDInote;	/* for unity pitch playback */
	char		MIDIlow, MIDIhi;/* MIDI pitch-bend range */
	char		loopmode;	/* semantics of loop data */
	char		nloops;		/* number of active loops */
	unsigned char	smpte[4];	/* SMPTE offset (hour:min:sec:frame) */
	/* this is a film audio thing */
	};

struct  loopinfo {
	int		start;		/* first sample */
	int		length;		/* length */
	int		count;		/* number of repeats, 0=forever */
	int		type;		/* 0=no, 1=forward, 2=forward/back */
	};

struct soundstream {
	struct	signalinfo info;	/* signal specifications */
	struct  instrinfo instr;	/* instrument specification */
	struct  loopinfo loops[NLOOPS];	/* Looping specification */
	char	swap;			/* do byte- or word-swap */
	char	seekable;		/* can seek on this file */
	char	*filename;		/* file name */
	char	*filetype;		/* type of file */
	char	*comment;		/* comment string */
	FILE	*fp;			/* File stream pointer */
	double	priv[PRIVSIZE/8];	/* format's private data area */
	int     ierr;
	};

IMPORT struct soundstream informat, outformat;
typedef struct soundstream *ft_t;
/*-----------------------------------------------------------------------------------*/
EXPORT char *sizes[] = {
			 "NONSENSE!",
			 "bytes",
			 "shorts",
			 "NONSENSE",
			 "longs",
			 "32-bit floats",
			 "64-bit floats",
			 "IEEE floats"
		       };

EXPORT char *styles[] = {
			  "NONSENSE!",
			  "unsigned",
			  "signed (2's complement)",
			  "u-law",
			  "a-law"
			};

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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
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
      sciprint("%s\r\n",writerr);
      ft->ierr=1;
    }
}
/*-----------------------------------------------------------------------------------*/
/* generic swap routine */
static void swapb(char *l, char *f, int n)
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
/* return swapped 32-bit float */
float
#if	defined(__STDC__) || defined(ARM)
swapf(float uf)
#else
     swapf(uf)
     float uf;
#endif
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
/* here for linear interp.  might be useful for other things */
long gcd(long int a, long int b)
{
  if (b == 0)
    return a;
  else
    return gcd(b, a % b);
}
/*-----------------------------------------------------------------------------------*/
long lcm(long int a, long int b)
{
  return (a * b) / gcd(a, b);
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

