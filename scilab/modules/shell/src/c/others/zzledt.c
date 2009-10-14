/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU (Completion in nw & nwni modes)
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/* For more information: http://www.gnu.org/software/termutils/manual/termcap-1.3/html_chapter/termcap_4.html */

#include "machine.h"

#include <errno.h>
#include <string.h>
#include <signal.h> /* for SIGINT */
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>

#if defined(HAVE_CURSES_H)
#  include <curses.h>
#elif defined(HAVE_NCURSES_H)
#  include <ncurses.h>
#endif

#ifdef HAVE_TERMCAP_H
#include <termcap.h>
#endif

#ifdef HAVE_TERM_H
#include <term.h>
#endif

#include "MALLOC.h"
#include "completion.h"
#include "getPartLine.h"
#include "localization.h"
#include "scilabmode.h"
#include "sciprint.h"
#include "HistoryManager.h"
#include "ConsoleRead.h"
#include "SetConsolePrompt.h"
#include "GetCharWithEventsLoop.h"
#include "core_math.h"
#include "Scierror.h"
#include "prompt.h"
#include "completion.h"
#include "x_VTPrsTbl.h"
#include "freeArrayOfString.h"
#include "getCommonPart.h"
#include "completeLine.h"
#include "TermReadAndProcess.h"
/*--------------------------------------------------------------------------*/
#ifdef aix
#define ATTUNIX
#endif

#ifdef aix370
#define ATTUNIX
#endif

#ifdef cdcu
#define ATTUNIX
#endif

#ifdef cray
#define B42UNIX
#endif

#ifdef hpux
#define ATTUNIX
#endif

#ifdef sgi
#define ATTUNIX
#endif

#ifdef sun
#define ATTUNIX
#define TERMCAP
#endif

#ifdef ultrix
#define B42UNIX
#define TERMCAP
#endif


#if !defined(linux) && !defined(netbsd) && !defined(freebsd) && !defined(__APPLE__) && !defined(__DragonFly__)
#ifdef  __alpha
#define B42UNIX
#endif
#endif

#if defined(linux) || defined(__APPLE__)
#define ATTUNIX
#define TERMCAP
#endif

#if defined(netbsd) || defined(freebsd) || defined(__DragonFly__)
#define TERMCAP
#endif
/*--------------------------------------------------------------------------*/
//#ifndef HAVE_TERMCAP
//	#undef TERMCAP
//#endif

#ifdef B42UNIX
#define KEYPAD
#include <sys/file.h>
#include <sgtty.h>
static short save_sg_flags;
static struct sgttyb arg;
static struct tchars arg1;
#endif

#ifdef ATTUNIX
#define KEYPAD
#ifdef HAVE_TERMIOS_H
#include <termios.h>
static struct termios save_term;
static struct termios arg;
#else
#include <termio.h>
static struct termio save_term;
static struct termio arg;
#endif
#endif

#define EXCL                  0x0021
#define UP_ARROW              0x0148
#define DOWN_ARROW            0x0150
#define LEFT_ARROW            0x014b
#define RIGHT_ARROW           0x014d
#define SEARCH_BACKWARD       0x015e
#define SEARCH_FORWARD        0x0160
#define CTRL_A                0x0001  /* control-A */
#define CTRL_E                0x0005  /* control-E */
#define CTRL_U                0x0015  /* control-U */
#define INS                   0x0014  /* control-T */
#define DEL                   0x007f
#define BS                    0x0008  /* control-H */
#define CR                    0x000d
#define LF                    0x000a
#define BEL                   0x0007
#define TAB                   0x0009
#define CTRL_B                0x0002  /* back a character */
#define CTRL_C                0x0003  /* redo line */
#define CTRL_D                0x0004  /* delete next char */
#define CTRL_F                0x0006  /* forward a character */
#define CTRL_K                0x000b  /* delete to end of line */
#define CTRL_L                0x000c  /* delete to end of line */
#define CTRL_N                0x000e  /* next line */
#define CTRL_P                0x0010  /* previous line */
#define CTRL_X                0x0018  /* Break scilab */
#define CTRL_Y                0x0019  /* paste */
#define CTRL_Z                0x0020  /* stop */
#define CASE_PRINT            63      /* from x_VTparse.h */

#define NUL '\0'
#define TRUE 1
#define FALSE 0
#define TAB_SKIP 8
#define NO_SAVED_LINES 100
#define WK_BUF_SIZE 520
#define SV_BUF_SIZE 5000

#define N_SEQS       6      /* number of special sequences */
#define MAX_SEQ_LEN  10     /* max chars in special termcap seqs */
#define ESC          0x01b
/*--------------------------------------------------------------------------*/
static char Sci_Prompt[10];
static char *tmpPrompt = NULL;

/* declare and define initial sequences. They
 * may be overwritten by termcap entries */
static char seqs[N_SEQS][MAX_SEQ_LEN] = {
  { 0x1b, 0x5b, 0x41, 0x00 },   /* up arrow */
  { 0x1b, 0x5b, 0x42, 0x00 },   /* down arrow */
  { 0x1b, 0x5b, 0x44, 0x00 },   /* left arrow */
  { 0x1b, 0x5b, 0x43, 0x00 },    /* right arrow */
  { 0x1b, 0x3c, 0x00, 0x00 },   /* search backward*/
  { 0x1b, 0x3e, 0x00, 0x00 }    /* search forward */
};
static int key_map[] = {UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, SEARCH_BACKWARD, SEARCH_FORWARD};

static char yank_buf[WK_BUF_SIZE + 1];/* yank buffer for copy/paste */
static int insert_flag = 1; /*insertion mode */
/* --- locally defined functions ---  */
static void move_right(char *source, int max_chars);
static void move_left(char *source);
static void display_string(char *string);
static void backspace(int n);
static void doCompletion(char *, int *cursor, int *cursor_max);
static void erase_nchar(int n);
static int gchar_no_echo(void);
static int CopyLineAtPrompt(char *wk_buf,char *line,int *cursor,int *cursor_max);
static void strip_blank(char *source);
static int  translate(int ichar);
static char *getLineBeforeCaret(char *wk_buf, int *cursor, int *cursor_max);
static char *getLineAfterCaret(char *wk_buf, int *cursor, int *cursor_max);

/* function for console mode */
static void enable_keypad_mode(void);
static void disable_keypad_mode(void);
static void init_io(void);
static void set_crmod(void);
static void set_cbreak(void);

/* --- extern functions ---  */
extern void C2F(sigbas)(int *n);

/*  ---- interruption handling ---*/
static char wk_buf_save[WK_BUF_SIZE + 1];
static int cursor_save=0;
static int cursor_max_save=0;
static int interrupted=0;
/*  ---- end of interruption handling ---*/

static int sendprompt=1;

/*-------------- Declarations  specific for console mode    -----------------  */
static int init_flag = TRUE;
static int tty;


static int cbreak_crmod = 1;/* crmod on */
static int fd=0;            /* file number for standard in */

#ifdef TERMCAP
/* termcap capability string for arrow up,down, left, and right */
static char *tc_capabilities[] = { "ku", "kd", "kl", "kr", "" };
static char strings[128];   /* place to store output strings from termcap file */
static char *KS=NULL;            /* enable keypad */
static char *KE=NULL;            /* disable keypad */
static char *CE=NULL;            /* clear to end of line */
char *BC=NULL;            /* backspace */
static char *IM=NULL;            /* start insert mode */
static char *IC=NULL;            /* insert character */
static char *EI=NULL;            /* end insert mode */
static char *CL=NULL;            /* clear screen */

#endif


//void C2F(zzledt)(char *buffer,int *buf_size,int *len_line,int * eof,	int *menusflag,int * modex,long int dummy1);

/*-------------- End of Declarations  specific for console mode-----------------  */
static void updateToken(char *linebuffer)
{
  if (linebuffer)
    {
      char *token = NULL;
      token = (char*)MALLOC(sizeof(char)*(strlen(linebuffer)+1));
      if (token)
	{
	  strcpy(token,linebuffer);
	  setSearchedTokenInScilabHistory(token);
	  FREE(token);
	  token = NULL;
	}
    }
}
/***********************************************************************
 * line editor
 **********************************************************************/

char *TermReadAndProcess(void)
{

  int cursor_max = 0;
  int cursor = 0;
  int yank_len,i;

  int keystroke;
  int character_count;
  char wk_buf[WK_BUF_SIZE + 1];

  char *buffer = NULL;

  tmpPrompt = GetTemporaryPrompt(); /* Input function has been used ? */
  GetCurrentPrompt(Sci_Prompt);

  if(init_flag)
    {
      init_io();
      init_flag = FALSE;
    }

  if (interrupted)
    {
      /* restore the state */
      interrupted=0;
      strcpy(wk_buf,wk_buf_save);
      cursor=cursor_save;
      cursor_save=0;
      cursor_max=cursor_max_save;
      cursor_max_save=0;
    }
  else
    {
      wk_buf[0] = NUL; /* initialize empty  buffer */
    }

#ifdef KEYPAD
  set_cbreak();
  enable_keypad_mode();
#endif

  /* Display the Scilab prompt */
  if(sendprompt)
    {
      if(tmpPrompt!=NULL)
        {
          printf("%s",tmpPrompt);
        }
      else
        {
          printf("%s",Sci_Prompt);/* write prompt */
        }
    }

  sendprompt=1;

  setSearchedTokenInScilabHistory(NULL);

  while(1)
    {
      /* main loop to read keyboard input */
      /* get next keystroke (no echo) returns -1 if interrupted */
      keystroke = gchar_no_echo();

      if ( (keystroke ==  CTRL_C) || (keystroke == CTRL_X) ) /* did not exist in old gtk version */
	{
	  int j = SIGINT;
	  C2F(sigbas)(&j);
	  keystroke = '\n';
	};

      /* check for ascii extended characters */
      if( ( iscntrl(keystroke) && groundtable[keystroke] != CASE_PRINT) || keystroke > 0x0100 )
	{
	  /* stroke is line editing command */
	  switch(keystroke)
	    {
	    case CTRL_P: case UP_ARROW: /* move one line up if any in history */
	      {
		char *line = getPreviousLineInScilabHistory();
		if (line)
		  {
		    CopyLineAtPrompt(wk_buf,line,&cursor,&cursor_max);
		    FREE(line);
		  }
	      }
	      break;

	    case CTRL_N: case DOWN_ARROW:  /* move one line down if any in history */
	      {
		char *line = getNextLineInScilabHistory();
		if (line)
		  {
		    CopyLineAtPrompt(wk_buf,line,&cursor,&cursor_max);
		    FREE(line);
		  }
	      }
	      break;

	    case CTRL_B: case LEFT_ARROW:/* move left*/
	      if(cursor > 0)
		{
		  /* is there room to move left */
		  cursor--;
		  backspace(1);
		}
	      else
		{
		  putchar(BEL);
		}
	      break;

	    case CTRL_F: case RIGHT_ARROW: /* move right*/

	      if(cursor < cursor_max) {/* is there room to move right */
		putchar(wk_buf[cursor++]);
	      }
	      else {
		putchar(BEL);
	      }
	      break;

	    case CTRL_A:
	      backspace(cursor); /* move to beginning of the line */
	      cursor = 0;
	      break;

	    case CTRL_E:  /* move to end of line */
	      while(cursor < cursor_max)
		{
		  putchar(wk_buf[cursor++]);
		}
	      break;
	    case TAB:

	      doCompletion(wk_buf, &cursor, &cursor_max);

	      break;
	    case INS: /* toggle insert/overwrite flag */
	      insert_flag = !insert_flag;
	      break;

	    case CTRL_X: case CTRL_C: /** we never get there CTRL_C is explored above **/
	      {
		int j = SIGINT;
		C2F(sigbas)(&j);
	      };
	      break;

	    case CTRL_D: /* delete next character*/
	      if(cursor == cursor_max)
		{
		  /* reminder that backing up over edge */
		  putchar(BEL);
		  break;
		}
	      move_left(&wk_buf[cursor]);
	      cursor_max--;
	      /* and write rest of line to end */
	      display_string(&wk_buf[cursor]);
	      /* erase extra character now at end. */
	      erase_nchar(1);
	      /* backspace to proper cursor position */
	      backspace(cursor_max - cursor);
	      //updateToken(wk_buf);
	      break;

	    case BS: case DEL: /* backspace with delete */
	      if(cursor == 0)
		{
		  /* reminder that backing up over edge */
		  putchar(BEL);
		  break;
		}
	      /* move string in work, one left from cursor */
	      move_left(&wk_buf[cursor - 1]);
	      cursor_max--;
	      cursor--;
	      backspace(1);
	      /* and write rest of line to end */
	      display_string(&wk_buf[cursor]);
	      /* erase extra character now at end. */
	      erase_nchar(1);
	      /* backspace to proper cursor position */
	      backspace(cursor_max - cursor);
	      //updateToken(wk_buf);
	      break;

	    case CTRL_K: /* delete to end of line */
	      if(cursor == cursor_max)
		{
		  /* reminder that backing up over edge */
		  putchar(BEL);
		  break;
		}
	      /* erase  character  at end. */
	      erase_nchar(cursor_max - cursor);
	      /* store cutted part in tyank buffer*/
	      strcpy(yank_buf,&wk_buf[cursor]);
	      /* backspace to proper cursor position */
	      wk_buf[cursor] = NUL;
	      cursor_max = cursor;
	      //updateToken(wk_buf);
	      break;

	    case CTRL_Y: /* Paste at the current point */
	      yank_len=strlen(yank_buf);
	      if(yank_len!=0 )
		{
		  if (cursor==cursor_max)
		    {
		      strcpy(&wk_buf[cursor],yank_buf);
		      display_string(&wk_buf[cursor]);
		      cursor = cursor_max + yank_len;
		      cursor_max = cursor;
		    }
		  else
		    {
		      for(i = 0; i <= cursor_max-cursor; i++) wk_buf[cursor_max+yank_len-i]=wk_buf[cursor_max-i];
		      wk_buf[cursor_max+yank_len]=NUL;
		      strncpy(&wk_buf[cursor],yank_buf,yank_len);
		      erase_nchar(cursor_max - cursor);
		      display_string(&wk_buf[cursor]);
		      backspace(cursor_max-cursor);
		      cursor_max=cursor_max+yank_len;
		      cursor=cursor+yank_len;
		    }
		}
	      //updateToken(wk_buf);
	      break;

	    case CTRL_Z:
	      break;

	    case CTRL_L:
#ifdef TERMCAP
	      fputs(CL,stdout);
	      wk_buf[0]=NUL;
	      goto exit;
#else
	      putchar(BEL);
#endif
	      break;

	    case CTRL_U: /* clear line buffer */
	      backspace(cursor);
	      erase_nchar(cursor_max);
	      wk_buf[0] = NUL;
	      cursor = cursor_max = 0;
	      break;

	    case LF: case CR: /* carrage return indicates line is ok;*/
	      strip_blank(wk_buf);/* first strip any trailing blanks */
	      if (wk_buf[0]==EXCL)
		{
		  char *token = NULL;
		  token = (char*)MALLOC(sizeof(char)*strlen(&wk_buf[1]));
		  if (token)
		    {
		      char *line = NULL;
		      strcpy(token,&wk_buf[1]);
		      setSearchedTokenInScilabHistory(token);
		      line = getNextLineInScilabHistory();
		      if (line)
			{
			  CopyLineAtPrompt(wk_buf,line,&cursor,&cursor_max);
			  FREE(line);
			  line = NULL;
			}
		      FREE(token);
		      token = NULL;
		    }
		  break;
		}
	      else
		{
		  if (strlen(wk_buf)>=0)
		    {
		      appendLineToScilabHistory(wk_buf);
		      setSearchedTokenInScilabHistory(NULL);
		    }
		}
	      goto exit;
	      break;

	    case SEARCH_BACKWARD:
	      break;

	    case SEARCH_FORWARD:
	      break;

	    default:
	      putchar(BEL);
	      break;
	    }
	}
      else
	{
	  /* alpha/numeric keystroke.
	   * substitute blank fill for tab char
	   */
	  if(keystroke == '\t')
	    {
	      keystroke = ' ';
	      character_count = TAB_SKIP - (cursor%TAB_SKIP);
	      if(character_count == 0) character_count = TAB_SKIP;
	    }
	  else
	    {
	      if(keystroke == EOF) ExitWithCodeFromScilab(0);
	      else character_count = 1;
	    }

	  while(character_count--)
	    {
	      if(insert_flag)
		{
		  /* insert mode, move rest of line right and
		   * add character at cursor
		   */
		  move_right(&wk_buf[cursor], WK_BUF_SIZE - cursor);
		  /* bump max cursor but not over buffer
		   * size
		   */
		  cursor_max = (++cursor_max > WK_BUF_SIZE) ? WK_BUF_SIZE : cursor_max;
		  /* if cursor at end of line, backspace so
		   * that new character overwrites last one */
		  if(cursor == WK_BUF_SIZE)
		    {
		      cursor--;
		      backspace(1);
		    }
		  wk_buf[cursor] = keystroke;
		  display_string(&wk_buf[cursor]);
		  cursor++;
		  backspace(cursor_max - cursor);
		}
	      else
		{
		  /* overstrike mode */
		  if(cursor == WK_BUF_SIZE)
		    {
		      cursor--;
		      backspace(1);
		    }
		  wk_buf[cursor] = keystroke;
		  putchar(keystroke);
		  if(cursor < WK_BUF_SIZE - 1)
		    {
		      cursor++;
		      cursor_max = Max(cursor_max, cursor);
		    }
		  else
		    {
		      backspace(1);
		    }
		}
	      updateToken(wk_buf);
	    }
	}
    }

 exit:
  /* copy to return buffer */
  buffer=strdup(wk_buf);
  putchar('\r');
  putchar('\n');
#ifdef KEYPAD
  set_crmod();
  disable_keypad_mode();
#endif

  return buffer;
}

/***********************************************************************
 * move_right - move source string to one address larger (right)
 **********************************************************************/
static void move_right(char *source, int max_chars)
{
  char *p;

  p = source;              /* point to beginning */
  while(max_chars-- && *p++) /* increment until max chars or string
			      * end */
    ;
  *p = NUL;                /* force new string end */
  while(--p > source) {    /* move from rightmost edge to right */
    *p = *(p - 1);
  }
}

/***********************************************************************
 * move_left - move source string to one address less (left)
 **********************************************************************/

static void move_left(char *source)
{
  do {                     /* move from left edge to left */
    *source = *(source + 1);
  } while(*source++ != NUL);
}

/***********************************************************************
 * display_string - display string starting at current cursor position
 **********************************************************************/

static void display_string(char *string)
{
  while(*string != NUL) {
    putchar(*string++);
  }
}

/***********************************************************************
 * doCompletion - manages Scilab Completion
 **********************************************************************/
static void displayPrompt(char *wk_buf)
{
	char msg[WK_BUF_SIZE]="";
	if (tmpPrompt!=NULL)
	{
		sprintf(msg,"%s\r\n%s%s",msg,tmpPrompt,wk_buf);
	}
	else
	{
		sprintf(msg,"%s\r\n%s%s",msg,Sci_Prompt,wk_buf);
	}
	display_string(msg);
}

static void displayCompletionDictionary(char **dictionary,int sizedictionary, char *namedictionary)
{
	#define MAX_LINE_SIZE 79 /* 80 - 1 the leading space */
	if (dictionary)
	{
		int i = 0;
		int lenCurrentLine = 0;

		display_string("\r\n");
		display_string(namedictionary);
		display_string(":");
		display_string("\r\n");

		for(i = 0;i < sizedictionary;i++)
		{
			int newlenLine = lenCurrentLine + (int)strlen(dictionary[i]) + (int)strlen(" ");
			if ( (lenCurrentLine + newlenLine) > MAX_LINE_SIZE )
			{
				display_string("\r\n");
				lenCurrentLine = 0;
			}
			else
			{
				lenCurrentLine = newlenLine;
			}
			display_string(dictionary[i]);
			display_string(" ");
		}
		display_string("\r\n");
	}
}
/*--------------------------------------------------------------------------*/
static char **concatenateStrings(int *sizearrayofstring, char *string1,
				 char *string2, char *string3,
				 char *string4, char *string5)
{
	int newsize = 0;
	char **arrayOfString = NULL;
	*sizearrayofstring = 0;

	if (string1) newsize++;
	if (string2) newsize++;
	if (string3) newsize++;
	if (string4) newsize++;
	if (string5) newsize++;

	if (newsize > 0)
	{
		arrayOfString = (char**)MALLOC(sizeof(char*) *(newsize));
		if (arrayOfString)
		{
			int i = 0;
			if (string1) {arrayOfString[i] = string1; i++;}
			if (string2) {arrayOfString[i] = string2; i++;}
			if (string3) {arrayOfString[i] = string3; i++;}
			if (string4) {arrayOfString[i] = string4; i++;}
			if (string5) {arrayOfString[i] = string5; i++;}
			*sizearrayofstring = i;
		}
		else
		{
			*sizearrayofstring = 0;
		}
	}
	return arrayOfString;
}
/*--------------------------------------------------------------------------*/
static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
				  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern,
				  char *wk_buf, int *cursor, int *cursor_max)
{
	if (dictionaryFiles)
	{
		if (sizedictionaryFiles == 1)
		{
			char *new_line = completeLine(lineBeforeCaret, dictionaryFiles[0],filePattern,defaultPattern,TRUE, lineAfterCaret);
			if (new_line)
			{
				char buflinetmp[WK_BUF_SIZE + 1];
				strcpy(buflinetmp,new_line);
				FREE(new_line);
				
				backspace(*cursor);
				erase_nchar(*cursor_max);
				wk_buf[0] = NUL;
				*cursor = *cursor_max = 0;
			
				displayPrompt(wk_buf);
				
				CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
				return;
			}
		}
		else
		{
			char *common = getCommonPart(dictionaryFiles, sizedictionaryFiles);

			displayCompletionDictionary(dictionaryFiles, 
				sizedictionaryFiles, gettext("File or Directory"));

			display_string("\r\n");
			
			backspace(*cursor);
			erase_nchar(*cursor_max);
			wk_buf[0] = NUL;
			*cursor = *cursor_max = 0;
			
			displayPrompt(wk_buf);

			if (defaultPattern[0] == 0)
			{
				int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);
				char *new_line = (char*)MALLOC(sizeof(char) * (lennewline + 1));
				
				if (new_line)
				{
					strcpy(new_line, lineBeforeCaret);
					strcat(new_line, lineAfterCaret);
					
					CopyLineAtPrompt(wk_buf, new_line, cursor, cursor_max);
					FREE(new_line); new_line = NULL;
				}
			}
			else if (common)
			{
				char *new_line = completeLine(lineBeforeCaret,common,filePattern,defaultPattern,TRUE,lineAfterCaret);
				if (new_line)
				{
					char buflinetmp[WK_BUF_SIZE + 1];
					strcpy(buflinetmp,new_line);
					FREE(new_line);
					
					CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
				}
				else
				{
					int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);
					new_line = (char*)MALLOC(sizeof(char) * (lennewline + 1));
					
					if (new_line)
					{
						strcpy(new_line, lineBeforeCaret);
						strcat(new_line, lineAfterCaret);
						
						CopyLineAtPrompt(wk_buf, new_line, cursor, cursor_max);
						FREE(new_line); new_line = NULL;
					}
				}
				FREE(common);
				common = NULL;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern,
				  char *wk_buf, int *cursor, int *cursor_max)
{
	if ( defaultPattern && strcmp(defaultPattern, "") )
	{
		int numberWordFound = 0;

		char **completionDictionaryFunctions = NULL;
		int sizecompletionDictionaryFunctions = 0;

		char **completionDictionaryCommandWords = NULL;
		int sizecompletionDictionaryCommandWords = 0;

		char **completionDictionaryMacros = NULL;
		int sizecompletionDictionaryMacros = 0;

		char **completionDictionaryVariables = NULL;
		int sizecompletionDictionaryVariables = 0;

		char **completionDictionaryHandleGraphicsProperties = NULL;
		int sizecompletionDictionaryHandleGraphicsProperties = 0;

		completionDictionaryFunctions = completionOnFunctions(defaultPattern, 
			&sizecompletionDictionaryFunctions);

		completionDictionaryCommandWords = completionOnCommandWords(defaultPattern, 
			&sizecompletionDictionaryCommandWords);

		completionDictionaryMacros = completionOnMacros(defaultPattern, 
			&sizecompletionDictionaryMacros);

		completionDictionaryVariables = completionOnVariablesWithoutMacros(defaultPattern, 
			&sizecompletionDictionaryVariables);

		completionDictionaryHandleGraphicsProperties = completionOnHandleGraphicsProperties(defaultPattern, 
			&sizecompletionDictionaryHandleGraphicsProperties);

		numberWordFound = sizecompletionDictionaryFunctions + sizecompletionDictionaryCommandWords +
			sizecompletionDictionaryMacros + sizecompletionDictionaryVariables +
			sizecompletionDictionaryHandleGraphicsProperties;

		if (numberWordFound > 0)
		{
			if (numberWordFound == 1)
			{
				char **completionDictionary = NULL;
				char *new_line = NULL;
				
				if (completionDictionaryFunctions) completionDictionary = completionDictionaryFunctions;
				if (completionDictionaryCommandWords) completionDictionary = completionDictionaryCommandWords;
				if (completionDictionaryMacros) completionDictionary = completionDictionaryMacros;
				if (completionDictionaryVariables) completionDictionary = completionDictionaryVariables;
				if (completionDictionaryHandleGraphicsProperties) completionDictionary = completionDictionaryHandleGraphicsProperties;

				new_line = completeLine(lineBeforeCaret, completionDictionary[0],NULL,defaultPattern,FALSE, lineAfterCaret);
				if (new_line)
				{
					char buflinetmp[WK_BUF_SIZE + 1];
					strcpy(buflinetmp,new_line);
					FREE(new_line);

					backspace(*cursor);
					erase_nchar(*cursor_max);
					wk_buf[0] = NUL;
					*cursor = *cursor_max = 0;
			
					displayPrompt(wk_buf);
					
					CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
				}
			}
			else
			{
				char *commonFunctions = getCommonPart(completionDictionaryFunctions,sizecompletionDictionaryFunctions);
				char *commonCommandWords = getCommonPart(completionDictionaryCommandWords,sizecompletionDictionaryCommandWords);
				char *commonMacros = getCommonPart(completionDictionaryMacros,sizecompletionDictionaryMacros);
				char *commonVariables = getCommonPart(completionDictionaryVariables,sizecompletionDictionaryVariables);
				char *commonHandleGraphicsProperties = getCommonPart(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties);

				char *commonAll = NULL;

				int sizecommonsDictionary = 0;
				char **commonsDictionary = concatenateStrings(&sizecommonsDictionary, commonFunctions,
					commonMacros, commonCommandWords, commonVariables, commonHandleGraphicsProperties);

				if (sizecommonsDictionary > 0)
				{
					if (sizecommonsDictionary == 1)
					{
						commonAll = strdup(commonsDictionary[0]);
					}
					else
					{
						commonAll = getCommonPart(commonsDictionary, sizecommonsDictionary);

					}
					freeArrayOfString(commonsDictionary, sizecommonsDictionary);
				}

				displayCompletionDictionary(completionDictionaryFunctions, sizecompletionDictionaryFunctions,(char *)_("Scilab Function"));
				displayCompletionDictionary(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords,(char *)_("Scilab Command"));
				displayCompletionDictionary(completionDictionaryMacros, sizecompletionDictionaryMacros,(char *)_("Scilab Macro"));
				displayCompletionDictionary(completionDictionaryVariables, sizecompletionDictionaryVariables,(char *)_("Scilab Variable"));
				displayCompletionDictionary(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties,(char *)_("Graphics handle field"));

				display_string("\r\n");
				
				backspace(*cursor);
				erase_nchar(*cursor_max);
				wk_buf[0] = NUL;
				*cursor = *cursor_max = 0;

				displayPrompt(wk_buf);

				if (commonAll)
				{
					char *new_line = NULL;

					new_line = completeLine(lineBeforeCaret, commonAll,NULL,defaultPattern,FALSE, lineAfterCaret);
					if (new_line)
					{
						char buflinetmp[WK_BUF_SIZE + 1];
						strcpy(buflinetmp,new_line);
						FREE(new_line);

						CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
					}

					FREE(commonAll);
					commonAll = NULL;
				}
			}

			freeArrayOfString(completionDictionaryFunctions,sizecompletionDictionaryFunctions);
			freeArrayOfString(completionDictionaryCommandWords,sizecompletionDictionaryCommandWords);
			freeArrayOfString(completionDictionaryMacros,sizecompletionDictionaryMacros);
			freeArrayOfString(completionDictionaryVariables,sizecompletionDictionaryVariables);
			freeArrayOfString(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties);
		}
	}
}
/*--------------------------------------------------------------------------*/
static void doCompletion(char *wk_buf, int *cursor, int *cursor_max)
{
	char *LineBeforeCaret = getLineBeforeCaret(wk_buf, cursor, cursor_max);
	char *LineAfterCaret = getLineAfterCaret(wk_buf, cursor, cursor_max);
	char *fileSearchedPattern = getFilePartLevel(LineBeforeCaret);
	char *SearchedPattern = getPartLevel(LineBeforeCaret);
	char **completionDictionaryFiles = NULL;
	int sizecompletionDictionaryFiles = 0;

	completionDictionaryFiles = completionOnFiles(fileSearchedPattern, &sizecompletionDictionaryFiles);
	if (completionDictionaryFiles)
	{
		TermCompletionOnFiles(completionDictionaryFiles, sizecompletionDictionaryFiles,
					LineBeforeCaret, LineAfterCaret, fileSearchedPattern, SearchedPattern,
					wk_buf, cursor, cursor_max);

		freeArrayOfString(completionDictionaryFiles, sizecompletionDictionaryFiles);
	}
	else
	{
		TermCompletionOnAll(LineBeforeCaret, LineAfterCaret, SearchedPattern,
				    wk_buf, cursor, cursor_max);
	}
	
	if (LineBeforeCaret) {FREE(LineBeforeCaret); LineBeforeCaret = NULL;}
	if (LineAfterCaret) {FREE(LineAfterCaret); LineAfterCaret = NULL;}
	if (fileSearchedPattern) {FREE(fileSearchedPattern); fileSearchedPattern = NULL;}
	if (SearchedPattern) {FREE(SearchedPattern); SearchedPattern = NULL;}
}
/*--------------------------------------------------------------------------*/
static char *getLineBeforeCaret(char *wk_buf, int *cursor, int *cursor_max)
{
	char *line = NULL;
	
	line = strdup(wk_buf);
	line[*cursor] = '\0';
	
	return line;
}
/*--------------------------------------------------------------------------*/
static char *getLineAfterCaret(char *wk_buf, int *cursor, int *cursor_max)
{
	char *line = NULL;
	
	if (*cursor != *cursor_max)
	{
		line = strdup(&wk_buf[*cursor]);
		line[(*cursor_max - *cursor) + 1] = '\0';
	}
	else
	{
		line = strdup("");
	}
	return line;
}
/*--------------------------------------------------------------------------*/

/***********************************************************************
 * backspace - move cursor n char to the left
 **********************************************************************/
static void backspace(int n)
{
  if(n < 1) return;
    while(n--)
#ifdef TERMCAP
      if(BC) {                 /* if control-H won-t work */
		  fputs(BC, stdout);
      }
      else {                   /* otherwise just use a normal control-H */
		  putchar('\010');
      }
#else
    putchar('\010');
#endif

}

/***********************************************************************
 * erase n characters to right and back up cursor
 **********************************************************************/

static void erase_nchar(int n)
{
  int i;                   /* fill field with blanks */
  for(i = 0; i < n; i++) {
    putchar(' ');
  }
  backspace(n);            /* and back up over blanks just written */
}

/***********************************************************************
 * strip_blank(source) - strip trailing blanks by inserting NUL-s
 **********************************************************************/
static void strip_blank(char *source)
{
  char *p;

  p = source;
  /* look for end of string */
  while(*p != NUL) {
    p++;
  }
  while(p != source) {
    p--;
    if(*p != ' ') break;
    *p = NUL;
  }
}

/**
 * get single character with no echo
 * this is the function which retrieves the input from the user in nw
 * and nwni modes
 * if interrupt is true, gchar_no_echo escape if an event arises
 * @return the number of the key (or the combinaison)
 */
static int gchar_no_echo(void)
{
  int i;
  /* get next character, gotten in cbreak mode so no wait for <cr> */
  i = getchar();

  /* if more than one character */
  if(i == ESC) {
    /* translate control code sequences to codes over 100 hex */
    return translate(i);
  }
  return(i);
}
/***********************************************************************
 * translate escape sequences
 **********************************************************************/
static int translate(int ichar)
{
  int i, j, not_done;
  char *pstr[N_SEQS];      /* points to each sequence as it progresses */

  /* initialize pointer array */
  for(i = 0; i < N_SEQS; i++)
    pstr[i] = &seqs[i][0];
  /* examine all pstrings one char at a time */
  for(j=0; j++ < MAX_SEQ_LEN;) {

    not_done = 0;
    for(i = 0; i < N_SEQS; i++) {
      /* if matches next character, this sequence */
      if(ichar == *pstr[i]) {
	/* if next character, this sequence, null */
	if(!*(++pstr[i]))
	  /* return sequence mapped to int */
	  return(key_map[i]);
	else
	  /* flag not done with this sequence yet */
	  not_done = 1;
      }
    }
    /* if any sequence not finished yet */
    if(not_done) {
      ichar = getchar();
    }
    else {
      /* hopefully at first character */
      break;
    }
  }

  return(ichar);
}

/***********************************************************************
 * Copy a line to the screen
 **********************************************************************/

static int CopyLineAtPrompt(char *wk_buf,char *line,int *cursor,int *cursor_max)
{
  if(line)
    {
      //** Copy line to current command buffer, usefull in completion case.
      strcpy (wk_buf,line);
      backspace(*cursor);/* backspace to beginning of line */
      display_string(wk_buf);/* copy to screen */

      *cursor = strlen(wk_buf);/* cursor set at end of line */

      /* erase extra characters left over if any */
      erase_nchar(Max(0, (*cursor_max - *cursor)));
      *cursor_max = *cursor;
      return 1;
    }
  return 0;
}

/************************************************************************
 * set CBREAK mode and switch off echo and disable CR action!
 ***********************************************************************/
static void set_cbreak()
{
  /* switch to CBREAK mode without flushing
   * line buffer */
#ifdef B42UNIX
  arg.sg_flags |= CBREAK;
  arg.sg_flags &= ~ECHO;
  /* arg.sg_flags &= ~CRMOD; */
  ioctl(fd, TIOCSETN, &arg);
#endif

#ifdef ATTUNIX
  arg.c_lflag &= ~ICANON;
  arg.c_lflag &= ~ECHO;
  /* arg.c_iflag &= ~ICRNL; */
  arg.c_oflag &= ~OPOST;
  arg.c_cc [VMIN] = 1;
  arg.c_cc [VTIME] = 0;
#ifdef HAVE_TERMIOS_H
  (void) tcsetattr (fd, TCSANOW, &arg);
#else
  ioctl(fd, TCSETAW, &arg);
#endif
#endif
  cbreak_crmod = 0;
  return;
}

/************************************************************************
 * reset to original mode
 ***********************************************************************/

static void set_crmod()
{
  /* reset to original mode (CRMOD) */
#ifdef B42UNIX
  arg.sg_flags = save_sg_flags;
  ioctl(fd, TIOCSETN, &arg);
  arg1.t_intrc = 3;
  ioctl(fd, TIOCSETC, &arg1);
#endif
#ifdef ATTUNIX
#ifdef HAVE_TERMIOS_H
  (void) tcsetattr (fd, TCSANOW, &save_term);
#else
  ioctl(fd, TCSETAW, &save_term);
#endif
#endif
  cbreak_crmod = 1;
  return;
}

/************************************************************************
 * initialise the io sequences
 ***********************************************************************/

static void init_io()
{
  char tc_buf[1024];       /* holds termcap buffer */
  char *area;
  char erase_char;
  int i;
  /* check standard for interactive */
  fd=fileno(stdin);

#ifdef B42UNIX
  ioctl(fd,TIOCGETP,&arg);
  save_sg_flags = arg.sg_flags;
  ioctl(fd,TIOCGETC,&arg1);
  erase_char = arg.sg_erase;
#endif

/* Get terminal mode */
#ifdef ATTUNIX
#ifdef HAVE_TERMIOS_H
  (void) tcgetattr (fd, &arg);
  (void) tcgetattr (fd, &save_term);
#else
  ioctl(fd, TCGETA, &arg);
  ioctl(fd, TCGETA, &save_term);
#endif
  erase_char = save_term.c_cc [VERASE];
#endif

#ifdef TERMCAP
  /* get termcap translations */
  if(tgetent(tc_buf, getenv("TERM")) == 1) {
    /* loop thru zero terminated list of input
     * capabilities */
    for(i = 0; *tc_capabilities[i]; i++) {
      area = &seqs[i][0];
      tgetstr(tc_capabilities[i], &area);
    }
    area = strings;       /* point to place where strings are to
			   * be stored */
    KS = tgetstr("ks", &area); /* term_keypad_on */
    KE = tgetstr("ke", &area); /* term_keypad_off */
    CE = tgetstr("ce", &area); /* clear end of line */
    BC = tgetstr("bc", &area); /* backspace */
    IM = tgetstr("im", &area); /* Enter insert mode */
    IC = tgetstr("ic", &area); /* Insert char */
    EI = tgetstr("ei", &area); /* Leave insert mode */
    CL = tgetstr("cl", &area); /* Clear scren */
  }
#endif
  setvbuf(stdin, NULL, _IONBF, 0); /* ehrlich juin 2001 */

}
#ifdef TERMCAP
/************************************************************************
 * enable keypad mode if using termcap
 ***********************************************************************/

static void enable_keypad_mode()
{
  /* enable keypad transmit mode */
  if(KS && *KS) fputs(KS, stdout);
}

/************************************************************************
 * disable the keypad mode if using termcap
 ***********************************************************************/

static void disable_keypad_mode()
{
  /* disable keypad transmit mode */
  if(KE && *KE) fputs(KE, stdout);
}
#else
/************************************************************************
 *we need references to thoses function if using KEYPAD but not Having TERMCAP
 ***********************************************************************/

static void enable_keypad_mode(){}
static void disable_keypad_mode(){}
#endif




