/***********************************************************
* a small portable version of readline 
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
* LF and CR return the entire line regardless of the cursor position 
* EOF with an empty line returns (char *)NULL 
* all other characters are ignored 
***********************************************************/
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>	
#include "machine.h"
#include "readline_nw.h"
#include "prompt.h"
#include "MALLOC.h"
#include "dynamic_menus.h"
#include "HistoryManager.h"
#include "scilabmode.h"
#include "sigbas.h"
#include "freeArrayOfString.h"
#include "scilines.h"
#include "strdup_windows.h"
#include "charEncoding.h"
#include "completion.h"
#include "preparsecompletion_nw.h"
/*--------------------------------------------------------------------------*/
#define MAXBUF	512
#define BACKSPACE 0x08		/* ^H */
#define SPACE	' '
#define isterm(f) ((f==stdin)|| f==stdout || f==stderr)

#ifdef TAB_KEY
#undef TAB_KEY
#endif
#define TAB_KEY 9

#ifdef PIPE
#undef PIPE
#endif 
#define PIPE 255

#ifdef CTRL_A
#undef CTRL_A
#endif 
#define CTRL_A 001

#ifdef CTRL_B
#undef CTRL_B
#endif 
#define CTRL_B 002

#ifdef CTRL_C
#undef CTRL_C
#endif 
#define CTRL_C 003

#ifdef CTRL_D
#undef CTRL_D
#endif 
#define CTRL_D 004

#ifdef CTRL_E
#undef CTRL_E
#endif 
#define CTRL_E 005

#ifdef CTRL_F
#undef CTRL_F
#endif 
#define CTRL_F 006

#ifdef CTRL_H
#undef CTRL_H
#endif 
#define CTRL_H 010

#ifdef CTRL_K
#undef CTRL_K
#endif 
#define CTRL_K 013

#ifdef CTRL_L
#undef CTRL_L
#endif 
#define CTRL_L 014

#ifdef CTRL_N
#undef CTRL_N
#endif 
#define CTRL_N 016

#ifdef CTRL_P
#undef CTRL_P
#endif 
#define CTRL_P 020

#ifdef CTRL_R
#undef CTRL_R
#endif 
#define CTRL_R 022

#ifdef CTRL_U
#undef CTRL_U
#endif 
#define CTRL_U 025

#ifdef CTRL_W
#undef CTRL_W
#endif 
#define CTRL_W 027

#ifdef CTRL_X
#undef CTRL_X
#endif 
#define CTRL_X 030

#ifdef ESC_KEY
#undef ESC_KEY
#endif 
#define ESC_KEY 033

#ifdef DEL
#undef DEL
#endif 
#define DEL 0177

#ifdef CR_1
#undef CR_1
#endif 
#define CR_1 '\n'

#ifdef CR_2
#undef CR_2
#endif 
#define CR_2 '\r'

#ifdef LEFT_ARROW
#undef LEFT_ARROW
#endif 
#define LEFT_ARROW 75

#ifdef RIGHT_ARROW
#undef RIGHT_ARROW
#endif 
#define RIGHT_ARROW 77

#ifdef UP_ARROW
#undef UP_ARROW
#endif 
#define UP_ARROW 72

#ifdef DOWN_ARROW
#undef DOWN_ARROW
#endif 
#define DOWN_ARROW 80

#ifdef HOME_KEY
#undef HOME_KEY
#endif 
#define HOME_KEY 71

#ifdef END_KEY
#undef END_KEY
#endif 
#define END_KEY 79

#ifdef DELETE_KEY
#undef DELETE_KEY
#endif 
#define DELETE_KEY 83

#ifdef CTRL_LEFT_ARROW
#undef CTRL_LEFT_ARROW
#endif 
#define CTRL_LEFT_ARROW 115

#ifdef CTRL_RIGHT_ARROW
#undef CTRL_RIGHT_ARROW
#endif 
#define CTRL_RIGHT_ARROW 116
/*--------------------------------------------------------------------------*/
char cur_line[MAXBUF];	/* current contents of the line */
/*--------------------------------------------------------------------------*/
extern int IsFromC(void);
/*--------------------------------------------------------------------------*/
static int cur_pos = 0;		/* current position of the cursor */
static int max_pos = 0;		/* maximum character position */
static int sendprompt = 1;  /* default */
static int lenCurrentLine = 0;
/*--------------------------------------------------------------------------*/
static char msdos_getch (void);
static void fix_line (void);
static void redraw_line(char *prompt);
static void clear_line (char *prompt);
static void clear_eoline (char *prompt);
static void copy_line (char *line);
static void doCompletion(const char *current_line,const char *prompt);
/*--------------------------------------------------------------------------*/
/* user_putc and user_puts should be used in the place of
* fputc(ch,stdout) and fputs(str,stdout) for all output
* of user typed characters.  This allows MS-Windows to 
* display user input in a different color. */
/*--------------------------------------------------------------------------*/
static int user_putc (int ch)
{
	return fputc(ch,stdout);
}
/*--------------------------------------------------------------------------*/
static int user_puts (char *str)
{
	return fputs (str, stdout);
}
/*--------------------------------------------------------------------------*/
static void doNewLine(BOOL displayPrompt)
{
	fputs ("\n", stdout);
	if (displayPrompt) fputs (SCIPROMPT, stdout);
	cur_line[0] = '\0';
	cur_pos = 0;
	max_pos = 0;
}
/*--------------------------------------------------------------------------*/
static void strip_blank(char *source)
{
	char *p;
	p = source;
	/* look for end of string */
	while(*p != '\0') p++;
	while(p != source)
	{
		p--;
		if(*p != ' ') break;
		*p = '\0';
	}
}
/*--------------------------------------------------------------------------*/
static void backspace ()
{
	user_putc (BACKSPACE);
}
/*--------------------------------------------------------------------------*/
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
		fputs (SCIPROMPT, stdout);
		/* read a line into the buffer, but not too* big */
		*eof = (fgets (buffer, buf_size, stdin) == NULL);
		/* remove newline character if there */
		return (1);
	}
	return (0);
}
/*--------------------------------------------------------------------------*/
/***************************************************
* reads one line 
* a pointer to an allocated zone (alloc) where
* the read characters are stored is returned 
***************************************************/
char * readline_nw (char *prompt)
{
	unsigned char cur_char;
	char *new_line = NULL;
	int eof;

	if (NotTTyRead (prompt, cur_line, MAXBUF, &eof) == 1)
	{
		if (eof == 1)
		{
			/** coded line to return eof **/
			cur_line[0] = -1;
			cur_line[1] = '\0';
		}
		return strdup(cur_line);
	}

	/* print the prompt */
	if (sendprompt) 
          {
            fputs ("\n", stdout);
            fputs (prompt, stdout);
          }
	sendprompt=1;

	cur_line[0] = '\0';
	cur_pos = 0;
	max_pos = 0;

	setSearchedTokenInScilabHistory(NULL);

	for (;;)
	{
		cur_char = msdos_getch();

		if (cur_char == TAB_KEY)
		{
			doCompletion(cur_line,prompt);
			cur_char = 0;
		}

		if (ismenu () == 1)
		{
			/* abort current line aquisition SS */
			sendprompt=0;
			if (new_line) {FREE(new_line);new_line=NULL;}
			new_line = (char*)MALLOC(sizeof(char)*2);
			new_line[0] = -2;
			new_line[1] = '\0';
			return (new_line);
		}

		if ((isprint (cur_char)  || ((unsigned char) cur_char > 0x7f)  ) && max_pos < MAXBUF - 1)
		{
			int i;
			for (i = max_pos; i > cur_pos; i--) cur_line[i] = cur_line[i - 1];
			user_putc (cur_char);
			cur_line[cur_pos] = cur_char;
			cur_pos += 1;
			max_pos += 1;
			if (cur_pos < max_pos) fix_line ();

			cur_line[max_pos] = '\0';
			setSearchedTokenInScilabHistory(cur_line);
		}
		else
		{
			/* do normal editing commands */
			/* some of these are also done above */
			int i;

			switch (cur_char)
			{
			case PIPE: 
				if (new_line) {FREE(new_line);new_line=NULL;}
				new_line = (char *)MALLOC(sizeof(char)*2);
				if (new_line)
				{
					new_line[0] = -1;
					new_line[1] = '\0';
				}
				return (new_line);

			case EOF:
				return ((char *) NULL);
			case CTRL_A: 
				while (cur_pos > 0)
				{
					cur_pos -= 1;
					backspace ();
				}
				break;
			case CTRL_B: 
				if (cur_pos > 0)
				{
					cur_pos -= 1;
					backspace ();
				}
				break;
			case CTRL_E: 
				while (cur_pos < max_pos)
				{
					user_putc (cur_line[cur_pos]);
					cur_pos += 1;
				}
				break;
			case CTRL_F: 
				if (cur_pos < max_pos)
				{
					user_putc (cur_line[cur_pos]);
					cur_pos += 1;
				}
				break;
			case CTRL_K: 
				clear_eoline (prompt);
				max_pos = cur_pos;
				break;
			case CTRL_P: 
				{
					/* move in history */
					cur_line[max_pos + 1] = '\0';
					if (cur_line[0]== '\0')
					{
						resetSearchedTokenInScilabHistory();
						setSearchedTokenInScilabHistory(NULL);
					}

					{
						char *line = getPreviousLineInScilabHistory();
						if (line)
						{
							clear_line(prompt);
							copy_line(line);
							FREE(line);
						}
					}
				}
				break;
			case CTRL_N: 
				{
					/* move in history */
					cur_line[max_pos + 1] = '\0';
					if (cur_line[0]== '\0')
					{
						resetSearchedTokenInScilabHistory();
						setSearchedTokenInScilabHistory(NULL);
					}

					{
						char *line = getNextLineInScilabHistory();
						if (line)
						{
							clear_line(prompt);
							copy_line(line);
							FREE(line);
						}
					}
				}
				break;
			case CTRL_L: case CTRL_R:
				putc ('\n', stdout);	/* go to a fresh line */
				redraw_line (prompt);
				break;
			case DEL: case CTRL_H:
				if (cur_pos > 0)
				{
					cur_pos -= 1;
					backspace ();
					for (i = cur_pos; i < max_pos; i++) cur_line[i] = cur_line[i + 1];
					max_pos -= 1;
					fix_line ();
				}
				break;
			case CTRL_D:		/* ^D */
				if (max_pos == 0) return ((char *) NULL);
				if (cur_pos < max_pos)
				{
					for (i = cur_pos; i < max_pos; i++)
						cur_line[i] = cur_line[i + 1];
					max_pos -= 1;
					fix_line ();
				}
				break;
			case CTRL_U:		/* ^U */
				clear_line (prompt);
				break;
			case CTRL_W:		/* ^W */
				while ((cur_pos > 0) && (cur_line[cur_pos - 1] == SPACE))
				{
					cur_pos -= 1;
					backspace ();
				}
				while ((cur_pos > 0) && (cur_line[cur_pos - 1] != SPACE))
				{
					cur_pos -= 1;
					backspace ();
				}
				clear_eoline (prompt);
				max_pos = cur_pos;
				break;
			case CR_1: case CR_2:
				cur_line[max_pos + 1] = '\0';
				if (cur_line[0]=='!')
				{
					char *token = NULL;
					token = (char*)MALLOC(sizeof(char)*strlen(cur_line));
					if (token)
					{
						char *line = NULL;
						strcpy(token,&cur_line[1]);
						setSearchedTokenInScilabHistory(token);
						line = getNextLineInScilabHistory();
						clear_line(prompt);
						if (line)
						{
							copy_line(line);
							FREE(line);
							line = NULL;
						}


						FREE(token);
						token = NULL;
					}
					else clear_line(prompt);
				}
				else
				{
					putc('\n',stdout);
					if (new_line) {FREE(new_line);new_line=NULL;}
					new_line = (char *) MALLOC (sizeof(char)*(strlen (cur_line) + 1));
					if (new_line) strcpy (new_line, cur_line);
					setSearchedTokenInScilabHistory(NULL);
					return (new_line);
				}
				break;

			default:
				break;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
/* redraw the entire line, putting the cursor where it belongs */
static void redraw_line (char *prompt)
{
	int i = 0;

	fputs (prompt, stdout);
	user_puts (cur_line);

	/* put the cursor where it belongs */
	for (i = max_pos; i > cur_pos; i--)  backspace ();
}
/*--------------------------------------------------------------------------*/
/* fix up the line from cur_pos to max_pos */
/* do not need any terminal capabilities except backspace, */
/* and space overwrites a character */
static void fix_line(void)
{
	int i = 0;

	/* write tail of string */
	for (i = cur_pos; i < max_pos; i++) user_putc (cur_line[i]);

	/* write a space at the end of the line in case we deleted one */
	user_putc (SPACE);

	/* backup to original position */
	for (i = max_pos + 1; i > cur_pos; i--) backspace ();
}
/*--------------------------------------------------------------------------*/
/* clear cur_line and the screen line */
static void clear_line (char *prompt)
{
	int i = 0;

	for (i = 0; i < max_pos; i++) cur_line[i] = '\0';

	for (i = cur_pos; i > 0; i--) backspace ();

	for (i = 0; i < max_pos; i++) putc (SPACE, stdout);
	putc ('\r', stdout);
	fputs (prompt, stdout);

	cur_pos = 0;
	max_pos = 0;
}
/*--------------------------------------------------------------------------*/
/* clear to end of line and the screen end of line */
static void clear_eoline (char *prompt)
{
	int i = 0;
	for (i = cur_pos; i < max_pos; i++)  cur_line[i] = '\0';

	for (i = cur_pos; i < max_pos; i++) putc (SPACE, stdout);
	for (i = cur_pos; i < max_pos; i++) backspace ();
}
/*--------------------------------------------------------------------------*/
/* copy line to cur_line, draw it and set cur_pos and max_pos */
static void copy_line (char *line)
{
	if (line)
	{
		char *OEM_string = (char*)MALLOC(sizeof(char)*(strlen(line)+1));
		if (OEM_string)
		{
			CharToOem(line,OEM_string);
			strcpy (cur_line,OEM_string);
			user_puts (cur_line);
			cur_pos = max_pos = (int)strlen (cur_line);
			FREE(OEM_string);
			OEM_string = NULL;
		}
	}
}
/*--------------------------------------------------------------------------*/
/* Convert Arrow keystrokes to Control characters: */
static char msdos_getch (void)
{
	char c = 0;

	if (!IsFromC())
	{
		while( !_kbhit() && ismenu()==0)  /* Test on ismenu added (bug 1052) - Francois VOGEL */
		{
			Sleep(1);
		}
	}
	if (ismenu()==1) return 0;  /* This line added to fix bug 1052 - Francois VOGEL */

	c = (char)_getch ();

	Sleep(1);

	if ( (c == CTRL_C) || (c == CTRL_X) )
	{
		/** control-C : we return a \n for stopping line processing **/
		int j = 2;
		C2F (sigbas) (&j);
		return ((int) '\n');
	}

	if (c == -32) /* 0 avant ? ? ? */
	{
		c = (char)_getch ();		/* Get the extended code. */

		switch (c)
		{
		case LEFT_ARROW:
			c = CTRL_B;
			break;

		case RIGHT_ARROW:
			c = CTRL_F;
			break;

		case UP_ARROW:
			c = CTRL_P;
			break;

		case DOWN_ARROW:
			c = CTRL_N;
			break;
		case CTRL_LEFT_ARROW: case HOME_KEY:
			c = CTRL_A;
			break;
		case CTRL_RIGHT_ARROW: case END_KEY:
			c = CTRL_E;
			break;
		case DELETE_KEY:
			c = CTRL_D;
			break;
		default:
			c = 0;
			break;
		}
	}
	else if (c == ESC_KEY)
	{				
		/* ESC */
		c = CTRL_U;
	}
	return c;
}
/*--------------------------------------------------------------------------*/
static void doCompletion(const char *current_line,const char *prompt)
{
	const char *wordToFind = NULL;
	char *backup_line = NULL;
	char **completionDictionary = NULL;
	int sizeCompletionDictionary = 0;

	wordToFind = preparse_line_for_completion_nw(current_line);

	if (wordToFind) 
	{
		completionDictionary = completion((char*)wordToFind,&sizeCompletionDictionary);
		if (sizeCompletionDictionary)
		{
			if (sizeCompletionDictionary == 1)
			{
				char *wordtodisp = completionDictionary[0];
				if ( strcmp(wordtodisp,wordToFind) != 0)
				{
					backup_line = strdup( cur_line);
					backup_line[strlen (cur_line) - strlen(wordToFind)] = '\0';

					doNewLine(FALSE);
					fputs ("\n", stdout);
					redraw_line((char*)prompt);
					strcat(backup_line,wordtodisp);
					strcpy(cur_line,backup_line);
					fputs (cur_line, stdout);
					cur_pos = (int)strlen(cur_line);
					max_pos = (int)strlen(cur_line);
					FREE(backup_line);
				}
			}
			else
			{
				int i = 0;
				char *pieceOfWord = NULL;

				backup_line = strdup(cur_line);

				doNewLine(FALSE);
				fputs ("\n", stdout);
				for(i= 0;i<sizeCompletionDictionary;i++)
				{
					int newlenLine = lenCurrentLine + (int)strlen(completionDictionary[i]) + (int)strlen(" ");
					if ( newlenLine >= (getColumnsSize() - 10) )
					{
						fputs ("\n", stdout);
						lenCurrentLine = 0;
					}
					else
					{
						lenCurrentLine = newlenLine;
					}

					fputs (completionDictionary[i], stdout);
					fputs (" ", stdout);

				}
				fputs ("\n", stdout);
				lenCurrentLine = 0;
				redraw_line((char*)prompt);
				strcpy(cur_line,backup_line);
				
				for (i = 0; i < sizeCompletionDictionary; i++)
				{
					 if ( strncmp(completionDictionary[0],completionDictionary[i],strlen(completionDictionary[0])) == 0)
					 {
						 if (pieceOfWord) {FREE(pieceOfWord); pieceOfWord = NULL;}
						 pieceOfWord = strdup(completionDictionary[0]);
					 }
					 else
					 {
						 if (pieceOfWord) {FREE(pieceOfWord); pieceOfWord = NULL;}
						 pieceOfWord = strdup(wordToFind);
						 break;
					 }
				}

				if (pieceOfWord)
				{
					strcpy(cur_line,pieceOfWord);
					FREE(pieceOfWord); pieceOfWord = NULL;
				}
				else
				{
					strcpy(cur_line,wordToFind);
				}

				fputs (cur_line, stdout);
				cur_pos = (int)strlen(cur_line);
				max_pos = (int)strlen(cur_line);
				FREE(backup_line);
			}
			freeArrayOfString(completionDictionary,sizeCompletionDictionary);
		}
	}
}
/*--------------------------------------------------------------------------*/
