/*
 *	$XConsortium: error.h,v 1.8 91/07/17 17:43:54 rws Exp $
 */


/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* @(#)error.h	X10/6.6	11/6/86 */
/* main.c */
#define	ERROR_KMALLOC	10	/* main: malloc() failed for keyboardtype */
#define	ERROR_FIONBIO	11	/* main: ioctl() failed on FIONBIO */


#define	ERROR_OPDEVTTY	14	/* spawn: open() failed on /dev/tty */
#define	ERROR_TIOCGETP	15	/* spawn: ioctl() failed on TIOCGETP */
#define	ERROR_TIOCGETC	16	/* spawn: ioctl() failed on TIOCGETC */
#define	ERROR_TIOCGETD	17	/* spawn: ioctl() failed on TIOCGETD */
#define	ERROR_TIOCGLTC	18	/* spawn: ioctl() failed on TIOCGLTC */
#define	ERROR_TIOCLGET	19	/* spawn: ioctl() failed on TIOCLGET */
#define	ERROR_TIOCCONS	20	/* spawn: ioctl() failed on TIOCCONS */
#define	ERROR_OPDEVTTY2	21	/* spawn: second open() failed on /dev/tty */
#define	ERROR_NOTTY	22	/* spawn: ioctl() failed on TIOCNOTTY */
#define	ERROR_TIOCSETP	23	/* spawn: ioctl() failed on TIOCSETP */
#define	ERROR_TIOCSETC	24	/* spawn: ioctl() failed on TIOCSETC */
#define	ERROR_TIOCSETD	25	/* spawn: ioctl() failed on TIOCSETD */
#define	ERROR_TIOCSLTC	26	/* spawn: ioctl() failed on TIOCSLTC */
#define	ERROR_TIOCLSET	27	/* spawn: ioctl() failed on TIOCLSET */

#define	ERROR_FORK	29	/* spawn: fork() failed */
#define	ERROR_EXEC	30	/* spawn: exec() failed */
#define	ERROR_OPDEVTTY3	31	/* spawn: third open() failed on /dev/tty */
#define	ERROR_PTYS	32	/* get_pty: not enough ptys */
#define	ERROR_NOX	33	/* get_terminal: can't connect to server */
#define ERROR_PTY_EXEC	34	/* waiting for initial map */

#define	ERROR_INIT	36	/* spawn: can't initialize window */
#define	ERROR_NOCO	37	/* resize: no `co' in termcap */
#define	ERROR_NOLI	38	/* resize: no `li' in termcap */
#define	ERROR_BORDER	39	/* get_terminal: can't make border tile */
#define	ERROR_BACK	40	/* get_terminal: can't make background tile */
#define ERROR_NOX3      43      /* get_terminal: bad pty from display server */
#define	ERROR_TIOCKGET	44	/* spawn: ioctl() failed on TIOCKGET */
#define	ERROR_TIOCKGETC	45	/* spawn: ioctl() failed on TIOCKGETC */
#define	ERROR_TIOCKSET	46	/* spawn: ioctl() failed on TIOCKSET */
#define	ERROR_TIOCKSETC	47	/* spawn: ioctl() failed on TIOCKSETC */
/* charproc.c */
#define	ERROR_SELECT	50	/* in_put: select() failed */
#define	ERROR_VINIT	54	/* VTInit: can't initialize window */
#define	ERROR_CNMALLOC1	55	/* Changename: malloc failed */
#define	ERROR_CNMALLOC2	56	/* Changename: malloc failed */
/* button.c */
#define	ERROR_BMALLOC2	71	/* SaltTextAway: malloc() failed */

/* misc.c */
#define	ERROR_LOGEXEC	80	/* StartLog: exec() failed */
#define	ERROR_XERROR	83	/* xerror: XError event */
#define	ERROR_XIOERROR	84	/* xioerror: X I/O error */
#define ERROR_WINNAME   85      /* get_terminal: malloc failed */
/* screen.c */
#define	ERROR_SCALLOC	90	/* Alloc: calloc() failed on base */
#define	ERROR_SCALLOC2	91	/* Alloc: calloc() failed on rows */
#define	ERROR_SREALLOC	92	/* ScreenResize: realloc() failed on alt base */
#define	ERROR_SREALLOC2	93	/* ScreenResize: realloc() failed on alt rows */
#define	ERROR_SREALLOC3	94	/* ScreenResize: realloc() failed on rows */
#define	ERROR_SREALLOC4	95	/* ScreenResize: realloc() failed on rows */
#define	ERROR_RESIZE	96	/* ScreenResize: malloc() or realloc() failed */
#define	ERROR_RESIZE2	97	/* ScreenResize: malloc() or realloc() failed */
#define	ERROR_RESIZROW	98	/* ScreenResize: realloc() failed on alt char */
#define	ERROR_RESIZROW2	99	/* ScreenResize: realloc() failed on alt attr */
#define	ERROR_RESIZROW3	100	/* ScreenResize: realloc() failed on attr */
#define	ERROR_RESIZROW4	101	/* ScreenResize: realloc() failed on attr */
/* scrollbar.c */
#define	ERROR_SBRALLOC	110	/* ScrollBarOn: realloc() failed on base */
#define	ERROR_SBRALLOC2	111	/* ScrollBarOn: realloc() failed on rows */
/* util.c */
#define	ERROR_UBACK	120	/* ReverseVideo: can't make background */
