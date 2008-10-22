#ifndef __MISC_H__
#define __MISC_H__

#define	PRIVSIZE	100
#define NLOOPS		8

#define	P1(x) x
#define	P2(x,y) x, y

/*-----------------------------------------------------------------------------------*/
double 	       swapd(P1(double d));			/* Swap double */
unsigned short swapw(P1(unsigned short us));		/* Swap short */
unsigned long  swapl(P1(unsigned long ul));		/* Swap long */
unsigned long long swapll(P1(unsigned long long ul));		/* Swap long long */
float  	       swapf(P1(float f));			/* Swap float */
unsigned int   swapi(P1(unsigned int us));		/* Swap int */
void           swapb(char *l, char *f, int n);          /* Swap bytes */
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

extern struct soundstream informat, outformat;
typedef struct soundstream *ft_t;
/*-----------------------------------------------------------------------------------*/
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

#endif /*#ifndef __MISC_H__ */
/*-----------------------------------------------------------------------------------*/

