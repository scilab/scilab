/* Allan CORNET */
/* INRIA 2005 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <Winuser.h>
#include "..\version.h"

#include "printf.h"
#include "winmain.h"
#include "wcommon.h"

/*-----------------------------------------------------------------------------------*/
extern int getdiary();
void C2F(diary) __PARAMS((char *str,int *n));
void diary_nnl __PARAMS((char *str,int *n));
/*-----------------------------------------------------------------------------------*/
int MyPutCh (int ch)
{
	return TextPutCh (&textwin, (BYTE) ch);
}
/*-----------------------------------------------------------------------------------*/
int MyKBHit (void)
{
	return TextKBHit (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyGetCh (void)
{
	return TextGetCh (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyGetChE (void)
{
	return TextGetChE (&textwin);
}
/*-----------------------------------------------------------------------------------*/
int MyFGetC (FILE * file)
{
	if (isterm (file))
	{
		return MyGetChE ();
	}
	return fgetc (file);
}
/*-----------------------------------------------------------------------------------*/
char *MyGetS (char *str)
{
	TextPutS (&textwin, "\nDANGER: gets() used\n");
	MyFGetS (str, 80, stdin);
	if (strlen (str) > 0
	&& str[strlen (str) - 1] == '\n')
	str[strlen (str) - 1] = '\0';
	return str;
}
/*-----------------------------------------------------------------------------------*/
char * MyFGetS (char *str, unsigned int size, FILE * file)
{
	char FAR *p;
	if (isterm (file))
	{
		p = TextGetS (&textwin, str, size);
		if (p != (char FAR *) NULL)
		return str;
		return (char *) NULL;
	}
	return fgets (str, size, file);
}
/*-----------------------------------------------------------------------------------*/
int MyFPutC (int ch, FILE * file)
{
	if (isterm (file))
	{
		MyPutCh ((BYTE) ch);
		TextMessage ();
		return ch;
	}
	return fputc (ch, file);
}
/*-----------------------------------------------------------------------------------*/
int MyFPutS (char *str, FILE * file)
{
	if (isterm (file))
	{
		TextPutS (&textwin, str);
		TextMessage ();
		return (*str);		/* different from Borland library */
	}
	return fputs (str, file);
}
/*-----------------------------------------------------------------------------------*/
int MyPutS (char *str)
{
	TextPutS (&textwin, str);
	MyPutCh ('\n');
	TextMessage ();
	return 0;			/* different from Borland library */
}
/*-----------------------------------------------------------------------------------*/
/** synonym for scilab but without the \n **/
void Scistring (char *str)
{
	int i;
	C2F (xscion) (&i);
	if (i == 0)
	fprintf (stdout, "%s", str);
	else
	{
		TextPutS (&textwin, str);
		MyPutCh ('\n');
		TextMessage ();
	}
}
/*-----------------------------------------------------------------------------------*/
int MyFPrintF (FILE * file, char *fmt,...)
{
	int count;
	va_list args;
	va_start (args, fmt);
	if (isterm (file))
	{
		char buf[MAXPRINTF];
		count = vsprintf (buf, fmt, args);
		TextPutS (&textwin, buf);
	}
	else
	count = vfprintf (file, fmt, args);
	va_end (args);
	return count;
}
/*-----------------------------------------------------------------------------------*/
int MyPrintF (char *fmt,...)
{
	int count;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);
	count = vsprintf (buf, fmt, args);
	TextPutS (&textwin, buf);
	va_end (args);
	return count;
}
/*-----------------------------------------------------------------------------------*/
/** Synonym for Scilab of MyPrintf **/
void sciprint (char *fmt,...)
{
	int i, count, lstr;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);

	/* next three lines added for diary SS */
	count = vsprintf (buf, fmt, args);
	lstr = strlen (buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		/*count = vfprintf(stdout, fmt, args ); */
		printf ("%s", buf);

	}
	else
	{
		/*count = vsprintf(buf,fmt,args); SS */
		TextPutS (&textwin, buf);
	}
	if (getdiary()) diary_nnl(buf,&lstr);

	va_end (args);
	/** return count; **/
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* the same but no diary record
*---------------------------------------------------*/
void sciprint_nd (char *fmt,...)
{
	int i, count, lstr;
	char buf[MAXPRINTF];
	va_list args;
	va_start (args, fmt);

	/* next three lines added for diary SS */
	count = vsprintf (buf, fmt, args);
	lstr = strlen (buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		/*count = vfprintf(stdout, fmt, args ); */
		printf ("%s", buf);

	}
	else
	{
		/*count = vsprintf(buf,fmt,args); SS */
		TextPutS (&textwin, buf);
	}
	va_end (args);
	/** return count; **/
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------
* as sciprint but with an added first argument
* which is ignored (used in do_printf)
*---------------------------------------------------*/
int sciprint2 (int iv, char *fmt,...)
{
	int i, count,lstr;
	va_list ap;
	char s_buf[1024];
	va_start (ap, fmt);
	/* next three lines added for diary SS */
	count = vsprintf (s_buf, fmt, ap);
	lstr = strlen (s_buf);

	C2F (xscion) (&i);
	if (i == 0)
	{
		count = vfprintf (stdout, fmt, ap);
	}
	else
	{
		/* count = vsprintf (s_buf, fmt, ap); SS */
		TextPutS (&textwin, s_buf);
	}
	if (getdiary()) diary_nnl(s_buf,&lstr);

	va_end (ap);
	return count;
}
/*-----------------------------------------------------------------------------------*/
size_t MyFWrite (const void *ptr, size_t size, size_t n, FILE * file)
{
	if (isterm (file))
	{
		int i;
		for (i = 0; i < (int) n; i++)
		TextPutCh (&textwin, ((BYTE *) ptr)[i]);
		TextMessage ();
		return n;
	}
	return fwrite (ptr, size, n, file);
}
/*-----------------------------------------------------------------------------------*/
size_t MyFRead (void *ptr, size_t size, size_t n, FILE * file)
{
	if (isterm (file))
	{
		int i;
		for (i = 0; i < (int) n; i++)
		((BYTE *) ptr)[i] = TextGetChE (&textwin);
		TextMessage ();
		return n;
	}
	return fread (ptr, size, n, file);
}
/*-----------------------------------------------------------------------------------*/
/* I/O Function for scilab : this function are used when Xscilab is on */

/*-----------------------------------------------------------------------------------*/
void Xputchar (c)
char c;
{
	MyPutCh ((int) c);
}
/*-----------------------------------------------------------------------------------*/
void Xputstring (str, n)
char *str;
int n;
{
	int i;
	for (i = 0; i < n; i++)
	Xputchar (str[i]);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscisncr) (str, n, dummy)
char *str;
integer *n, dummy;
{
	int i;
	for (i = 0; i < *n; i++)
	{
		Xputchar (str[i]);
	}
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscistring) (str, n, dummy)
char *str;
int *n;
long int dummy;
{
	int i;
	for (i = 0; i < *n; i++)
	{
		Xputchar (str[i]);
	}
	Xputstring ("\r\n", 2);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
void C2F (xscimore) (n)
int *n;
{
	int n1, ln;
	*n = 0;
	ln = strlen (MORESTR);
	Xputstring (MORESTR, ln);
	n1 = TextGetCh (&textwin);
	if (n1 == 'n')
	*n = 1;
	Xputstring ("\r\n", 2);
	TextMessage ();
}
/*-----------------------------------------------------------------------------------*/
/* I/O Function for C routines : test for xscion */

void Scisncr (str)
char *str;
{
	int i;
	integer lstr;
	C2F (xscion) (&i);
	if (i == 0)
	fprintf (stdout, "%s", str);
	else
	{
		lstr = strlen (str);
		C2F (xscisncr) (str, &lstr, 0L);
	}
}
/*-----------------------------------------------------------------------------------*/
/********************************************************************
* int ReadKey (void) - reads console input (including function
*			 			keys and national characters).
* Return value:
*		ASCII character (OEM codepage) in low byte
*		Virtual Keycode of function key in high byte if low byte == 0.
* BUGS:
*		This function does not filter keyboard layout switching
*		hot keys.
*
* Original design by Boris Usievich.
* Custom implementation by Sergey Okhapkin
********************************************************************/
int ReadKey (void)
{
	INPUT_RECORD irBuffer;
	DWORD n;
	int ch;
	HANDLE hIn = GetStdHandle (STD_INPUT_HANDLE);
	SetConsoleMode (hIn, ENABLE_PROCESSED_INPUT);
	do
	{
		PeekConsoleInput (hIn, &irBuffer, 1, &n);
		if (n == 0)
		{
			Sleep (100);
			continue;
		}

		if (irBuffer.EventType == KEY_EVENT &&
		irBuffer.Event.KeyEvent.bKeyDown)
		{
			if (irBuffer.Event.KeyEvent.uChar.AsciiChar != '\0')
			{
				ReadConsole (hIn, &ch, 1, &n, NULL);
				return ch & 0xff;
			}
			else
			{
				ReadConsoleInput (hIn, &irBuffer, 1, &n);
				return irBuffer.Event.KeyEvent.wVirtualKeyCode << 8;
			};
		}
		else
		ReadConsoleInput (hIn, &irBuffer, 1, &n);
	}
	while (TRUE);
}
/*-----------------------------------------------------------------------------------*/
int WriteStr (char *s, FILE * file)
{
	#ifdef UseWIN
	BOOL bSuccess;
	DWORD cCharsWritten;
	const PCHAR crlf = "\n";
	BOOL retflag = TRUE;
	HANDLE hOut = GetStdHandle (STD_OUTPUT_HANDLE);
	WriteConsole (hOut, s, strlen (s), &cCharsWritten, NULL);
	#else
	fputs (s, stdout);
	fflush (stdout);
	#endif
	return (strlen (s));
}
/*-----------------------------------------------------------------------------------*/
int WriteKey (char c, FILE * file)
{
	#ifdef UseWIN
	BOOL bSuccess;
	DWORD cCharsWritten;
	const PCHAR crlf = "\n";
	BOOL retflag = TRUE;
	HANDLE hOut = GetStdHandle (STD_OUTPUT_HANDLE);
	SetConsoleMode (hOut, ENABLE_PROCESSED_OUTPUT);
	WriteConsole (hOut, &c, 1, &cCharsWritten, NULL);
	#else
	fputc (c, stdout);
	fflush (stdout);
	#endif
	return (1);
}
/*-----------------------------------------------------------------------------------*/