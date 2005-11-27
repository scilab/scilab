#ifndef SCI_ST 
#define SCI_ST 

#include "wav.h" 

/*
 * July 5, 1991
 * Copyright 1991 Lance Norskog And Sundry Contributors
 * This source code is freely redistributable and may be used for
 * any purpose.  This copyright notice must be maintained. 
 * Lance Norskog And Sundry Contributors are not responsible for 
 * the consequences of using this software.
 */
#if (defined(netbsd) || defined(freebsd) || defined(__APPLE__)) && !defined(unix) 
#define unix
#endif

#define IMPORT  extern
#define EXPORT 

/*
 * Sound Tools sources header file.
 */

#include <stdio.h>

/*
 * Handler structure for each format.
 */


/* Signal parameters */

struct  signalinfo {
	long		rate;		/* sampling rate */
	int		size;		/* word length of data */
	int		style;		/* format of sample numbers */
	int		channels;	/* number of sound channels */
};

/* Loop parameters */

struct  loopinfo {
	int		start;		/* first sample */
	int		length;		/* length */
	int		count;		/* number of repeats, 0=forever */
	int		type;		/* 0=no, 1=forward, 2=forward/back */
};

/* Instrument parameters */

/* vague attempt at generic information for sampler-specific info */

struct  instrinfo {
	char 		MIDInote;	/* for unity pitch playback */
	char		MIDIlow, MIDIhi;/* MIDI pitch-bend range */
	char		loopmode;	/* semantics of loop data */
	char		nloops;		/* number of active loops */
	unsigned char	smpte[4];	/* SMPTE offset (hour:min:sec:frame) */
					/* this is a film audio thing */
};


#define MIDI_UNITY 60		/* MIDI note number to play sample at unity */

/* Loop modes */
#define LOOP_NONE          0	
#define LOOP_8             1	/* 8 loops: don't know ?? */
#define LOOP_SUSTAIN_DECAY 2	/* AIFF style: one sustain & one decay loop */

/* Pipe parameters */

struct	pipeinfo {
	FILE	*pout;			/* Output file */
	FILE	*pin;			/* Input file */
};

/*
 *  Format information for input and output files.
 */

#define	PRIVSIZE	100

#define NLOOPS		8

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

/* flags field */
#define FILE_STEREO	1	/* does file format support stereo? */
#define FILE_LOOPS	2	/* does file format support loops? */
#define FILE_INSTR	4	/* does file format support instrument specificications? */


/* Size field */
#define	BYTESCI	1
#define	WORDSCI	2
#define	LONGSCI	4
#define	FLOATSCI	5
#define DOUBLESCI	6
#define IEEE	7		/* IEEE 80-bit floats.  Is it necessary? */


/* Style field */
#define UNSIGNED	1	/* unsigned linear: Sound Blaster */
#define SIGN2		2	/* signed linear 2's comp: Mac */
#define	ULAW		3	/* U-law signed logs: US telephony, SPARC */
#define ALAW		4	/* A-law signed logs: non-US telephony */

IMPORT char *sizes[], *styles[];

#if	defined(__STDC__) || defined(ARM)
#define	P1(x) x
#define	P2(x,y) x, y
#define	P3(x,y,z) x, y, z
#define	P4(x,y,z,w) x, y, z, w
#else
#define P1(x)
#define P2(x,y)
#define P3(x,y,z)
#define P4(x,y,z,w)
#endif

/* Utilities to read and write shorts and longs little-endian and big-endian */
unsigned short rlshort(P1(ft_t ft));			/* short little-end */
unsigned short rbshort(P1(ft_t ft));			/* short big-end    */
void wlshort(P2(ft_t ft, unsigned short us));	/* short little-end */
void wbshort(P2(ft_t ft, unsigned short us));	/* short big-end    */
unsigned long  rllong(P1(ft_t ft));			/* long little-end  */
unsigned long  rblong(P1(ft_t ft));			/* long big-end     */
void wllong(P2(ft_t ft, unsigned long ul));	/* long little-end  */
void wblong(P2(ft_t ft, unsigned long ul));	/* long big-end     */
/* Read and write words and longs in "machine format".  Swap if indicated.  */
unsigned short rshort(P1(ft_t ft));			
void wshort(P2(ft_t ft, unsigned short us));
unsigned long  rlong(P1(ft_t ft));		
void  wlong(P2(ft_t ft, unsigned long ul));
float          rfloat(P1(ft_t ft));
void wfloat(ft_t ft, float f);
double         rdouble(P1(ft_t ft));
void           wdouble(P2(ft_t ft, double d));

/* Utilities to byte-swap values */
unsigned int   swapi(P1(unsigned int us));		/* Swap int */
unsigned short swapw(P1(unsigned short us));		/* Swap short */
unsigned long  swapl(P1(unsigned long ul));		/* Swap long */
float  	       swapf(P1(float f));			/* Swap float */
double 	       swapd(P1(double d));			/* Swap double */

#ifdef ARM
IMPORT double sfloor(P1(double x));   /* Hack our way around the flawed */
IMPORT double sceil(P1(double x));    /* UnixLib floor ceil functions */
#endif


IMPORT void report(P2(char *, ...)),  warn(P2(char *, ...)),
	 fail(P2(char *, ...));

typedef	unsigned int u_i;
typedef	unsigned long u_l;
typedef	unsigned short u_s;

IMPORT float volume;	/* expansion coefficient */
IMPORT int dovolume;

IMPORT float amplitude;	/* Largest sample so far */

/* export flags */
IMPORT int summary;	/* just print summary of information */

IMPORT char *myname;

IMPORT int soxpreview;	/* Preview mode: be fast and ugly */

#define	MAXRATE	50L * 1024			/* maximum sample rate */

#if  defined(unix) || defined (__OS2__) || defined(aix)
/* Some wacky processors don't have arithmetic down shift, so do divs */
/* Most compilers will turn this into a shift if they can, don't worry */
#define RIGHT(datum, bits)	((datum) / (1L << bits)) 
#define LEFT(datum, bits)	((datum) << bits) 
#else
/* x86 & 68k PC's have arith shift ops and dumb compilers */
#define RIGHT(datum, bits)	((datum) >> bits)
#define LEFT(datum, bits)	((datum) << bits)
#endif

#ifndef	M_PI
#define M_PI	3.14159265358979323846
#endif

#if	defined(unix) || defined(AMIGA) || defined (__OS2__) \
	|| defined(OS9) || defined(ARM) || defined(aix)
#define READBINARY	"r"
#define WRITEBINARY	"w"
#endif
#ifdef	VMS
#define READBINARY      "r", "mbf=16", "ctx=stm" 
#define WRITEBINARY     "w", "ctx=stm"
#endif
#if defined(DOS) || defined(WIN32)
#define READBINARY	"rb"
#define WRITEBINARY	"wb"
#endif

/* default values */ 
#ifndef READBINARY 
#define READBINARY "r"
#endif

#ifndef WRITEBINARY
#define WRITEBINARY "w"
#endif

 


/* Error code reporting */
#ifdef	QNX
#include <errno.h>
#endif

#if defined(unix) || defined(__OS2__) || defined(aix)
#include <errno.h>
extern int errno;
#endif

#ifdef	__OS2__
#define REMOVE remove
#else
#define REMOVE unlink
#endif

char *version();			/* return version number */
/* ummmm??? */

#include "../machine.h" 

#if defined(__alpha)|defined(__ia64__)
int wavread __PARAMS((ft_t ft, int *buf, long int len));
#else
int wavread __PARAMS((ft_t ft, long int *buf, long int len));
#endif


void  wavstartread __PARAMS((ft_t ft,WavInfo *,int flag));
void wavwrite __PARAMS((ft_t ft, long int *buf, long int len));
void wavstartwrite __PARAMS((ft_t ft));
void wavstopwrite  __PARAMS((ft_t ft));
void wavwritehdr  __PARAMS((ft_t ft));

#if defined(__alpha)|defined(__ia64__)
int rawread __PARAMS((ft_t ft,int * buf,long nsamp));
#else
int rawread __PARAMS((ft_t ft,long * buf,long nsamp));
#endif
void rawwrite __PARAMS((ft_t ft,long *buf, long nsamp) );

#endif 
