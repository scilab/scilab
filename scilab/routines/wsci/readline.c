/*******************************************
 * Original source : GNUPLOT - readline.c 
 * modified for Scilab 
 *******************************************
 *
 * Copyright (C) 1986 - 1993   Thomas Williams, Colin Kelley
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 * 
 *
 * AUTHORS
 *
 *   Original Software:
 *     Tom Tkacik
 *   Msdos port and some enhancements:
 *     Gershon Elber and many others.
 *   Scilab port 
 *     Jean-Philippe Chancelier 
 *     Allan CORNET 2004 
*/

/***********************************************************
 * a small portable version of GNU's readline 
 * this is not the BASH or GNU EMACS version of READLINE due to Copyleft restrictions 
 * do not need any terminal capabilities except backspace, 
 * and space overwrites a character 
 * NANO-EMACS line editing facility 
 * printable characters print as themselves (insert not overwrite) 
 * ^A moves to the beginning of the line 
 * ^B moves back a single character 
 * ^E moves to the end of the line 
 * ^F moves forward a single character 
 * ^K kills from current position to the end of line 
 * ^H and DEL delete the previous character 
 * ^D deletes the current character, or EOF if line is empty 
 * ^P moves back through history 
 * ^N moves forward through history 
 * ^L/^R redraw line in case it gets trashed 
 * ^U kills the entire line 
 * ^W kills last word 
 * LF and CR return the entire line regardless of the cursor postition 
 * EOF with an empty line returns (char *)NULL 
 * all other characters are ignored 
 ***********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <conio.h>
#include <string.h>


#ifndef STRICT
	#define STRICT
#endif
	
#include "wcommon.h"
	
#ifdef USE_CONSOLE
	#include "wtextc.h"
#else
	#include "wtext.h"
#endif

#include "../machine.h"


/*-----------------------------------------------------------------------------------*/
#define TEXTUSER 0xf1
#define TEXTGNUPLOT 0xf0
#define special_getc() msdos_getch()
#define MAXBUF	512
#define BACKSPACE 0x08		/* ^H */
#define SPACE	' '
#define isterm(f) ((f==stdin && InteractiveMode()==0)|| f==stdout || f==stderr)
/*-----------------------------------------------------------------------------------*/
struct sci_hist
{
    		char *line;
    		struct sci_hist *prev;
    		struct sci_hist *next;
};
/*-----------------------------------------------------------------------------------*/
extern int MyGetCh (void);
extern LPTW Backuplptw;	
extern TW textwin;

extern struct sci_hist *history;	/* voir history.c */
extern struct sci_hist *cur_entry;

extern BOOL PutLineInBuffer;
extern char copycur_line[MAXBUF];
char cur_line[MAXBUF];	/* current contents of the line */
static int cur_pos = 0;		/* current position of the cursor */
static int max_pos = 0;		/* maximum character position */

static char msdos_getch ();
static char Windows_getch ();
static void fix_line ();
static void redraw_line ();
static void clear_line ();
static void clear_eoline ();
static void copy_line ();
extern int C2F(ismenu) ();
static int sendprompt=1;
#define SV_BUF_SIZE 5000
static char tosearch[SV_BUF_SIZE] = "";/* place to store search string */

extern struct sci_hist * SearchBackwardInHistory(char *line);
extern void GetCurrentPrompt(char *CurrentPrompt);
/*-----------------------------------------------------------------------------------*/
/************************************
 * Send a string to scilab interaction window
 * as if it was typed by the user 
  ************************************/
#ifdef USE_CONSOLE
void Write_Scilab_Console (char *buf)
#else
void Write_Scilab_Window (char *buf)
#endif
{
	#ifdef USE_CONSOLE
	char prompt[10];
	GetCurrentPrompt(prompt);
	clear_line (prompt);
	copy_line (buf);
	#else
	char *d;
	char buffer[1024];
	if ( ( buf[strlen(buf)-1] != '\n' ) && ( buf[strlen(buf)-1] != '\r' ) )
	{
		strcpy(buffer,buf);
		strcat(buffer,"\n");
	}
	else
	{
		strcpy(buffer,buf);
	}
	
	if (buffer[0] != '\0')
    {
      d = buffer;
      while (*d)
		{
			SendMessage (textwin.hWndText, WM_CHAR, *d, 1L);
			d++;
		}
	}
	#endif


}

/*-----------------------------------------------------------------------------------*/
/* user_putc and user_puts should be used in the place of
 * fputc(ch,stdout) and fputs(str,stdout) for all output
 * of user typed characters.  This allows MS-Windows to 
 * display user input in a different color. */
/*-----------------------------------------------------------------------------------*/
static int user_putc (int ch)
{
	int rv;
  
	#ifndef USE_CONSOLE
  		TextAttr (&textwin, TEXTUSER);
	#endif

  	rv = fputc (ch, stdout);
  
	#ifndef USE_CONSOLE
  		TextAttr (&textwin, TEXTGNUPLOT);
	#endif

  	return rv;
}
/*-----------------------------------------------------------------------------------*/
static int user_puts (char *str)
{
	int rv;
	#ifndef USE_CONSOLE
  		TextAttr (&textwin, TEXTUSER);
	#endif
  
  	rv = fputs (str, stdout);
  
	#ifndef USE_CONSOLE
  		TextAttr (&textwin, TEXTGNUPLOT);
  	#endif
  
  	return rv;
}
/*-----------------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
  char *p;
  p = source;
  /* look for end of string */
  while(*p != '\0') p++;
  while(p != source) {
    p--;
    if(*p != ' ') break;
    *p = '\0';
  }
}

/*-----------------------------------------------------------------------------------*/
/* This function provides a centralized non-destructive backspace capability */
/* M. Castro */
static void backspace ()
{
  user_putc (BACKSPACE);
}
/*-----------------------------------------------------------------------------------*/
/***************************************************
 * reads one line when stdin is not a tty 
 * the input characters are not echoed 
 ***************************************************/
static int NotTTyRead (char *prompt, char *buffer, int buf_size, int *eof)
{
  static int firstentry = 0, tty;
  if (firstentry == 0)
    {
      tty = isterm (stdin);
      firstentry++;
    }
  if (!tty)
    {
      /** We are reading a file ==> no prompts : XXXXX to test **/
      fputs ("-->", stdout);
      /* read a line into the buffer, but not too* big */
      *eof = (fgets (buffer, buf_size, stdin) == NULL);
      /* remove newline character if there */
      return (1);
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/***************************************************
 * reads one line 
 * a pointer to an allocated zone (alloc) where
 *   the read characters are stored is returned 
 ***************************************************/
#ifdef USE_CONSOLE
char * readline_nw (char *prompt, int interrupt)
{
  unsigned char cur_char;
  char *new_line;  /* unsigned char *new_line; */
  int eof;
  if (NotTTyRead (prompt, cur_line, MAXBUF, &eof) == 1)
    {
      if (eof == 1)
	{
	/** coded line to return eof **/
	  cur_line[0] = -1;
	  cur_line[1] = '\0';
	}
      new_line = (char *) alloc ((unsigned long)
				 (strlen (cur_line) + 1), "history");
      strcpy (new_line, cur_line);
      return (new_line);
    }

  /* print the prompt */
  if (sendprompt) fputs (prompt, stdout);
  sendprompt=1;

  cur_line[0] = '\0';
  cur_pos = 0;
  max_pos = 0;
  //cur_entry = NULL;

  /* get characters */
  for (;;)
    {
      cur_char = special_getc ();
      	if (interrupt&&(C2F(ismenu) () == 1))
      	{/* abort current line aquisition SS */
		sendprompt=0;
		new_line = (char *) alloc ((unsigned long) 2, "history");
		new_line[0] = -2;
		new_line[1] = '\0';
		return (new_line);
        }
      if ((isprint (cur_char)  || ((unsigned char) cur_char > 0x7f)  ) && max_pos < MAXBUF - 1)
	{
	  int i;
	  for (i = max_pos; i > cur_pos; i--)
	    {
	      cur_line[i] = cur_line[i - 1];
	    }
	  user_putc (cur_char);
	  cur_line[cur_pos] = cur_char;
	  cur_pos += 1;
	  max_pos += 1;
	  if (cur_pos < max_pos)    fix_line ();
	  
	  cur_line[max_pos] = '\0';

	}
      else
	{
	  /* do normal editing commands */
	  /* some of these are also done above */
	  int i;
	  
	  switch (cur_char)
	    {
	    case 255:		/* jpc eof quand on fait un pipe */
	      new_line = (char *) alloc ((unsigned long) 2, "history");
	      new_line[0] = -1;
	      new_line[1] = '\0';
	      return (new_line);
	    case EOF:
	      return ((char *) NULL);
	    case 001:		/* ^A */
	      while (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      break;
	    case 002:		/* ^B */
	      if (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      break;
	    case 005:		/* ^E */
	      while (cur_pos < max_pos)
		{
		  user_putc (cur_line[cur_pos]);
		  cur_pos += 1;
		}
	      break;
	    case 006:		/* ^F */
	      if (cur_pos < max_pos)
		{
		  user_putc (cur_line[cur_pos]);
		  cur_pos += 1;
		}
	      break;
	    case 013:		/* ^K */
	      clear_eoline ();
	      max_pos = cur_pos;
	      break;
	    case 020:		/* ^P */
	      if (history != NULL)
		{
		  if (cur_entry == NULL)
		    {
		      cur_entry = history;
		      clear_line (prompt);
		      copy_line (cur_entry->line);
		    }
		  else if (cur_entry->prev != NULL)
		    {
		      cur_entry = cur_entry->prev;
		      clear_line (prompt);
		      copy_line (cur_entry->line);
		    }
		}
	      break;
	    case 016:		/* ^N */
	      if (cur_entry != NULL)
		{
		  cur_entry = cur_entry->next;
		  clear_line (prompt);
		  if (cur_entry != NULL)
		    copy_line (cur_entry->line);
		  else
		    cur_pos = max_pos = 0;
		}
	      break;
	    case 014:		/* ^L */
	    case 022:		/* ^R */
	      putc ('\n', stdout);	/* go to a fresh line */
	      redraw_line (prompt);
	      break;
	    case 0177:		/* DEL */
	    case 010:		/* ^H */
	      if (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		  for (i = cur_pos; i < max_pos; i++)
		    cur_line[i] = cur_line[i + 1];
		  max_pos -= 1;
		  fix_line ();
		}
	      break;
	    case 004:		/* ^D */
	      if (max_pos == 0)
		{
		  return ((char *) NULL);
		}
	      if (cur_pos < max_pos)
		{
		  for (i = cur_pos; i < max_pos; i++)
		    cur_line[i] = cur_line[i + 1];
		  max_pos -= 1;
		  fix_line ();
		}
	      break;
	    case 025:		/* ^U */
	      clear_line (prompt);
	      break;
	    case 027:		/* ^W */
	      while ((cur_pos > 0) &&
		     (cur_line[cur_pos - 1] == SPACE))
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      while ((cur_pos > 0) &&
		     (cur_line[cur_pos - 1] != SPACE))
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      clear_eoline ();
	      max_pos = cur_pos;
	      break;
	    case '\n':		/* ^J */
	    case '\r':		/* ^M */
			cur_line[max_pos + 1] = '\0';
	      	if (cur_line[0]=='!')
			{ 
				struct sci_hist *P=NULL;
				P=SearchBackwardInHistory(&cur_line[1]);
				clear_line (prompt);
				if (P != NULL) copy_line (P->line);
			}
		    else
			{
				putc ('\n', stdout);
				new_line = (char *) alloc ((unsigned long) (strlen (cur_line) + 1), "history");
				strcpy (new_line, cur_line);
				return (new_line);
			}
		break;
	      
	    
	    
	    default:
	      break;
	    }
	}
    }
}
#else
/*-----------------------------------------------------------------------------------*/
char * readline_win (char *prompt,int interrupt)
{
  unsigned char cur_char;
  char *new_line;  /* unsigned char *new_line; */
  
  /* print the prompt */
  if (sendprompt) fputs (prompt, stdout);
  sendprompt=1;

  cur_line[0] = '\0';
  cur_pos = 0;
  max_pos = 0;
  cur_entry = NULL;
  
  /* get characters */
  for (;;)
    {
	cur_char = Windows_getch() ;      
	if (interrupt&&(C2F(ismenu) () == 1))
      	{/* abort current line aquisition SS */
		sendprompt=0;
		new_line = (char *) alloc ((unsigned long) 2, "history");
		new_line[0] = -2;
		new_line[1] = '\0';

		return (new_line);
        }
	
	if ((isprint (cur_char)  || ((unsigned char) cur_char > 0x7f)  ) && max_pos < MAXBUF - 1)
	{
	  int i;
 
	  for (i = max_pos; i > cur_pos; i--)
	    {
	      cur_line[i] = cur_line[i - 1];
	    }
	  user_putc (cur_char);
	  cur_line[cur_pos] = cur_char;
	  cur_pos += 1;
	  max_pos += 1;
	  if (cur_pos < max_pos)    fix_line ();
	  
	  cur_line[max_pos] = '\0';
          
	}
      else
	{
	  /* do normal editing commands */
	  /* some of these are also done above */
	  int i;
	
	  switch (cur_char)
	    {
	    case 255:		/* jpc eof quand on fait un pipe */
	      new_line = (char *) alloc ((unsigned long) 2, "history");
	      new_line[0] = -1;
	      new_line[1] = '\0';
      
	      return (new_line);
	    case EOF:
	      return ((char *) NULL);
	    case 001:		/* ^A */
	      while (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      break;
	    case 002:		/* ^B */
	      if (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      break;
    
	    case 005:		/* ^E */
	      while (cur_pos < max_pos)
		{
		  user_putc (cur_line[cur_pos]);
		  cur_pos += 1;
		}
	      break;
	    case 006:		/* ^F */
	      if (cur_pos < max_pos)
		{
		  user_putc (cur_line[cur_pos]);
		  cur_pos += 1;
		}
	      break;
	    case 013:		/* ^K */
	      clear_eoline ();
	      max_pos = cur_pos;
	      break;
	     case 020:		/* ^P */
	      if (history != NULL)
		{
		  if (cur_entry == NULL)
		    {
		      cur_entry = history;
		      clear_line (prompt);
		      copy_line (cur_entry->line);
		    }
		  else if (cur_entry->prev != NULL)
		    {
		      cur_entry = cur_entry->prev;
		      clear_line (prompt);
		      copy_line (cur_entry->line);
		    }
		}
	      break;
	    case 016:		/* ^N */
	      if (cur_entry != NULL)
		{
		  cur_entry = cur_entry->next;
		  clear_line (prompt);
		  if (cur_entry != NULL)
		    copy_line (cur_entry->line);
		  else
		    cur_pos = max_pos = 0;
		}
	      break;
	    case 014:		/* ^L */
	    case 022:		/* ^R */
	      putc ('\n', stdout);	/* go to a fresh line */
	      putc ('\n', stdout);
	      redraw_line (prompt);
	      break;
	    case 0177:		/* DEL */
	    case 010:		/* ^H */
	      if (cur_pos > 0)
		{
		  cur_pos -= 1;
		  backspace ();
		  for (i = cur_pos; i < max_pos; i++)
		    cur_line[i] = cur_line[i + 1];
		  max_pos -= 1;
		  fix_line ();
		}
	      break;
	    case 004:		/* ^D */
	      if (max_pos == 0)
		{
		  clear_line (prompt);
		  break;
		}
	      if (cur_pos < max_pos)
		{
		  for (i = cur_pos; i < max_pos; i++)
		    cur_line[i] = cur_line[i + 1];
		  max_pos -= 1;
		  fix_line ();
		}
	      break;
	    case 025:		/* ^U */
	      clear_line (prompt);
	      break;
	    case 027:		/* ^W */
	      while ((cur_pos > 0) &&
		     (cur_line[cur_pos - 1] == SPACE))
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      while ((cur_pos > 0) &&
		     (cur_line[cur_pos - 1] != SPACE))
		{
		  cur_pos -= 1;
		  backspace ();
		}
	      clear_eoline ();
	      max_pos = cur_pos;
	      break;
	    case '\n':		/* ^J */
	    case '\r':		/* ^M */
			cur_line[max_pos + 1] = '\0';
	      	if (cur_line[0]=='!')
			{ 
				struct sci_hist *P=NULL;
				P=SearchBackwardInHistory(&cur_line[1]);
				clear_line (prompt);
				if (P != NULL) copy_line (P->line);
			}
		    else
			{
				putc ('\n', stdout);
				new_line = (char *) alloc ((unsigned long) (strlen (cur_line) + 1), "history");
				strcpy (new_line, cur_line);
				return (new_line);
			}
		break;

	    default:
 
	      break;
	    }
	}
    }
      
}
#endif
/*-----------------------------------------------------------------------------------*/
/* redraw the entire line, putting the cursor where it belongs */
static void redraw_line (prompt)
     char *prompt;
{
  int i;

  fputs (prompt, stdout);
  user_puts (cur_line);

  /* put the cursor where it belongs */
  for (i = max_pos; i > cur_pos; i--)  backspace ();
}
/*-----------------------------------------------------------------------------------*/
/* fix up the line from cur_pos to max_pos */
/* do not need any terminal capabilities except backspace, */
/* and space overwrites a character */
static void fix_line(void)
{
  int i;

  /* write tail of string */
  for (i = cur_pos; i < max_pos; i++)
    user_putc (cur_line[i]);

  /* write a space at the end of the line in case we deleted one */
  user_putc (SPACE);

  /* backup to original position */
  for (i = max_pos + 1; i > cur_pos; i--) backspace ();

}
/*-----------------------------------------------------------------------------------*/
/* clear cur_line and the screen line */
static void clear_line (prompt)
     char *prompt;
{
  int i;
  for (i = 0; i < max_pos; i++)
    cur_line[i] = '\0';

  for (i = cur_pos; i > 0; i--)
    backspace ();

  for (i = 0; i < max_pos; i++)
    putc (SPACE, stdout);

  putc ('\r', stdout);
  fputs (prompt, stdout);

  cur_pos = 0;
  max_pos = 0;
}
/*-----------------------------------------------------------------------------------*/
/* clear to end of line and the screen end of line */
static void clear_eoline (prompt)
     char *prompt;
{
  int i;
  for (i = cur_pos; i < max_pos; i++)
    cur_line[i] = '\0';

  for (i = cur_pos; i < max_pos; i++)
    putc (SPACE, stdout);
  for (i = cur_pos; i < max_pos; i++)
    backspace ();
}
/*-----------------------------------------------------------------------------------*/
/* copy line to cur_line, draw it and set cur_pos and max_pos */
static void copy_line (line)
     char *line;
{
  strcpy (cur_line, line);
  user_puts (cur_line);
  cur_pos = max_pos = strlen (cur_line);
}
/*-----------------------------------------------------------------------------------*/
/* Convert Arrow keystrokes to Control characters: */
static char msdos_getch ()
{
  char c ;
  
  c = getch ();

  Sleep(1);

  if (c == 3)
    {
/** control-C : we return a \n for stopping line processing **/
      SignalCtrC ();
      return ((int) '\n');
    }
   
  if (c == -32) /* 0 avant ? ? ? */
    {
	
  	 c = getch ();		/* Get the extended code. */

      

      switch (c)
	{
	case 75:		/* Left Arrow. */
	  c = 002;
	  break;
	case 77:		/* Right Arrow. */
	  c = 006;
	  break;
	case 72:		/* Up Arrow. */
	
	  c = 020;
	  break;
	case 80:		/* Down Arrow. */
	  c = 016;
	  break;
	case 115:		/* Ctl Left Arrow. */
	case 71:		/* Home */
	  c = 001;
	  break;
	case 116:		/* Ctl Right Arrow. */
	case 79:		/* End */
	  c = 005;
	  break;
	case 83:		/* Delete */
	  c = 004;
	  break;
	default:
	  c = 0;
	  break;
	}
    }
  else if (c == 033)
    {				/* ESC */
      c = 025;
    }
  return c;
}
/*-----------------------------------------------------------------------------------*/
static char Windows_getch ()
{
  char c ;

  c = MyGetCh();
  
  if (c == 3)
    {
	/** control-C : we return a \n for stopping line processing **/
      	SignalCtrC ();
      	return ((int) '\n');
    }
  else
  if (c == 0)
    {
  	 c = MyGetCh();		/* Get the extended code. */
  
      switch (c)
	{
	case 75:		/* Left Arrow. */
	  c = 002;
	  break;
	case 77:		/* Right Arrow. */
	  c = 006;
	  break;
	case 72:		/* Up Arrow. */
	  c = 020;
	  break;
	case 80:		/* Down Arrow. */
	  c = 016;
	  break;
	case 115:		/* Ctl Left Arrow. */
	case 71:		/* Home */
	  c = 001;
	  break;
	case 116:		/* Ctl Right Arrow. */
	case 79:		/* End */
	  c = 005;
	  break;
	case 83:		/* Delete */
	  c = 004;
	  break;
	default:
	  c = 0;
	  break;
	}
    }
  else if (c == 033)
    {				/* ESC */
      c = 025;
    }
  
  return c;
}
/*-----------------------------------------------------------------------------------*/
