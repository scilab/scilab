/* Copyright INRIA/ENPC */
/***********************************************************************
 * zzledt.c - last line editing routine
 *
 * $Id: zzledt.c,v 1.3 2001/10/16 09:27:09 chanceli Exp $
 * $Log: zzledt.c,v $
 * Revision 1.3  2001/10/16 09:27:09  chanceli
 * includes
 *
 * Revision 1.2  2001/07/25 14:00:11  chanceli
 * declarations
 *
 * Revision 1.1  2001/06/18 11:38:48  chanceli
 * zzledt moved here
 *
 * Revision 1.2  2001/05/25 13:43:12  chanceli
 * commentaires
 *
 * Revision 1.1.1.1  2001/04/26 07:48:09  scilab
 * Imported sources
 *
 *
 * Revision 10.1  1993/02/11  14:34:03  mga
 * Initial Version
 *
 * Copyright (c) mitchell and gauthier assoc, inc 1990
 * All rights reserved
 *
 **********************************************************************/

#include <string.h>

#ifndef WIN32 
/** The win32 version is defined in the wsci directory **/

#ifdef __STDC__
#include <stdlib.h>
#include <unistd.h>
#endif

int gchar_no_echo();
extern int  Xorgetchar();
char Sci_Prompt[10];

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
#if !defined(linux) && !defined(netbsd) && !defined(freebsd)
#ifdef  __alpha
#define B42UNIX
#endif
#endif
#ifdef linux
#define ATTUNIX
#define TERMCAP
#endif
#if defined(netbsd) || defined(freebsd)
#define TERMCAP
#endif

#include <stdio.h>
#include <ctype.h>
#include "../machine.h"

#ifndef HAVE_TERMCAP
#undef TERMCAP
#endif

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

#include <termio.h>

static struct termio save_term;
static struct termio arg;
#endif

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

#define EXCL                  0x0021
#define UP_ARROW              0x0148
#define DOWN_ARROW            0x0150
#define LEFT_ARROW            0x014b
#define RIGHT_ARROW           0x014d
#define SEARCH_BACKWARD       0x015e
#define SEARCH_FORWARD        0x0160
#define HOME                  0x0001  /* control-A */
#define ENDS                  0x0005  /* control-E */
#define LDEL                  0x0015  /* control-U */
#define INS                   0x0014  /* control-T */
#define DEL                   0x007f
#define BS                    0x0008  /* control-H */
#define CR                    0x000d
#define LF                    0x000a
#define BEL                   0x0007
#define CTRL_B                0x0002  /* back a character */
#define CTRL_C                0x0003  /* redo line */
#define CTRL_D                0x0004  /* delete next char */
#define CTRL_F                0x0006  /* forward a character */
#define CTRL_K                0x000b  /* delete to end of line */
#define CTRL_L                0x000c  /* delete to end of line */
#define CTRL_N                0x000e  /* next line */
#define CTRL_P                0x0010  /* previous line */
#define CTRL_Y                0x0019  /* paste */

#define NUL '\0'
#define TRUE 1
#define FALSE 0
#define TAB_SKIP 8
#define NO_SAVED_LINES 100
#define WK_BUF_SIZE 520
#define SV_BUF_SIZE 5000

static int cbreak_crmod = 1;/* crmod on */
static int fd=0;              /* file number for standard in */

#define N_SEQS       6      /* number of special sequences */
#define MAX_SEQ_LEN  10     /* max chars in special termcap seqs */
#define ESC          0x01b
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


#ifdef TERMCAP
                            /* termcap capability string for arrow up,
                             * down, left, and right */
static char *tc_capabilities[] = { "ku", "kd", "kl", "kr", "" };
static char strings[128];   /* place to store output strings from termcap
                             * file */
static char *KS;            /* enable keypad */
static char *KE;            /* disable keypad */
static char *CE;            /* clear to end of line */
static char *BC;            /* backspace */
static char *IM;            /* start insert mode */
static char *IC;            /* insert character */
static char *EI;            /* end insert mode */
static char *CL;            /* clear screen */
#endif

static char sv_buf[SV_BUF_SIZE];
static char yank_buf[WK_BUF_SIZE + 1];
static char *sv_lines[NO_SAVED_LINES] = { NULL, NULL };
static char *sv_buf_point = sv_buf;
static char tosearch[SV_BUF_SIZE] = "";/* place to store search string */
static int last_line = 1;
static int insert_flag = 1;
static int init_flag = TRUE;
static int tty;
static    int cur_line_number = 0;

static void move_right(), move_left(), display_string();
static void get_line(), save_line(), backspace(), erase_nchar();
static void enable_keypad_mode(), disable_keypad_mode();
static void init_io(), set_cbreak(), set_crmod();
static void strip_blank();
static int  lines_equal();
static int  translate();
static int  search_line_backward(),search_line_forward();
void  set_echo_mode(),set_is_reading();
int   get_echo_mode();

#include "x_VTparse.h" 
extern int groundtable[]; /* character table */ 

/***********************************************************************
 * line editor
 **********************************************************************/
extern void
C2F(zzledt)(buffer, buf_size, len_line, eof, dummy1)
/**********************************************************************/
char *buffer;
int *buf_size;
int *len_line;
int *eof;
long int dummy1;                /* added by FORTRAN to give buffer length */
{
   int line_index = 0;
   int line_number;
   int cursor_max = 0;
   int cursor = 0;
   int yank_len,i;
   int keystroke;
   int character_count;
   char wk_buf[WK_BUF_SIZE + 1];

   if(init_flag) {
     init_io();
     init_flag = FALSE;
   }

   set_is_reading(TRUE);

   /* if not an interactive terminal */
   if(!tty) {
     /* read a line into the buffer, but not too
      * big */
     /** fputs("[file]",stdout); **/
     fputs("-->",stdout);
     *eof = (fgets(buffer, *buf_size, stdin) == NULL);
     *len_line = strlen(buffer);
     /* remove newline character if there */
     if(buffer[*len_line - 1] == '\n')
       (*len_line)--;
     return;
   }

#ifdef KEYPAD 
   set_cbreak();
   enable_keypad_mode();
#endif

                            /* write prompt */
   printf(Sci_Prompt);
                            /* empty work buffer */
    wk_buf[0] = NUL;

                            /* main loop to read keyboard input */
   while(1) {
                            /* get next keystroke (no echo) */
      keystroke = gchar_no_echo();
      /* jpc: 2000 added CASE_PRINT to check for ascii extended characters */
      if( ( iscntrl(keystroke) && groundtable[keystroke] != CASE_PRINT)
	  || keystroke > 0x0100 )
	{
	  /* stroke is line editing command */
         switch(keystroke) {

         case UP_ARROW:
         case DOWN_ARROW:
	 case CTRL_P:
	 case CTRL_N:

            if(keystroke == UP_ARROW || keystroke == CTRL_P) {
                            /* pick out previous line, if any */
               if(line_index < last_line) line_index++;
            }
            else {
                            /* pick out following line, if any */
               if(line_index > 0) line_index--;
            }
                            /* get a new line from the save buffer */
            get_line(line_index, wk_buf);
                            /* backspace to beginning of line */
            backspace(cursor);
                            /* copy to screen */
            display_string(wk_buf);
                            /* cursor set at end of line */
            cursor = strlen(wk_buf);
                            /* erase extra characters left over,
			     * if any */
            erase_nchar(MAX(0, cursor_max - cursor));
            cursor_max = cursor;
            break;

         case LEFT_ARROW:
	 case CTRL_B:
                            /* if room to move left */
            if(cursor > 0) {
               cursor--;
               backspace(1);
            }
	    else {
	       putchar(BEL);
            }
            break;

         case RIGHT_ARROW:
	 case CTRL_F:
                            /* if room to move right */
            if(cursor < cursor_max) {
               putchar(wk_buf[cursor++]);
            }
	    else {
	       putchar(BEL);
            }
            break;

         case HOME:
                            /* move to start of line */
            backspace(cursor);
            cursor = 0;
            break;

         case ENDS:
                            /* move to end of line */
            while(cursor < cursor_max) {
               putchar(wk_buf[cursor++]);
            }
            break;

         case INS:
                            /* toggle insert/overwrite flag */
            insert_flag = !insert_flag;
            break;
		    
	 case CTRL_C:
	    /* we must never be here */
	    break;

	 case CTRL_D:
	                    /* delete next character*/
            if(cursor == cursor_max) {
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
            break;

         case DEL:
         case BS:
                            /* backspace with delete */
            if(cursor == 0) {
			    /* reminder that backing up over edge */
	       putchar(BEL);
               break;
	    }
                            /* move string in work, one left from
			     * cursor */
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
            break;

	 case CTRL_K:
	                    /* delete to end of line*/
            if(cursor == cursor_max) {
			    /* reminder that backing up over edge */
	       putchar(BEL);
               break;
	    }
			    /* erase  character  at end. */
	    erase_nchar(cursor_max - cursor);
                            /* store cutted part */
            strcpy(yank_buf,&wk_buf[cursor]);
			    /* backspace to proper cursor position */
            wk_buf[cursor] = NUL;
            cursor_max = cursor;
            break;

	  case CTRL_Y:
	    yank_len=strlen(yank_buf);
            if(yank_len!=0 ){
              if (cursor==cursor_max) {
		strcpy(&wk_buf[cursor],yank_buf);
		display_string(&wk_buf[cursor]);
		cursor = cursor_max + yank_len;
		cursor_max = cursor;

	      }
	      else {
		for(i = 0; i <= cursor_max-cursor; i++)
		  wk_buf[cursor_max+yank_len-i]=wk_buf[cursor_max-i];
                wk_buf[cursor_max+yank_len]=NUL;
		strncpy(&wk_buf[cursor],yank_buf,yank_len);
		erase_nchar(cursor_max - cursor);
		display_string(&wk_buf[cursor]);
		backspace(cursor_max-cursor);
                cursor_max=cursor_max+yank_len;
                cursor=cursor+yank_len;
	      }

	    }
            break;

#ifdef TERMCAP
         case CTRL_L:

            fputs(CL,stdout);
            wk_buf[0]=NUL;
	    goto exit;
#endif

         case LDEL:
                            /* clear line buffer */
            backspace(cursor);
            erase_nchar(cursor_max);
            wk_buf[0] = NUL;
            cursor = cursor_max = 0;
            break;

         case CR:
         case LF:
	    cur_line_number = 0;
                            /* carrage return indicates line is ok;
                             * first strip any trailing blanks */
            strip_blank(wk_buf);
            if (wk_buf[0]==EXCL) {
              strcpy(tosearch,wk_buf);
              line_number = search_line_backward(tosearch);
	      if(line_number == 0) {
		putchar(BEL);
	      }
              else {
		cur_line_number=line_number;
		strcpy(wk_buf,sv_lines[line_number]);
		backspace(cursor);
		/* copy to screen */
		display_string(wk_buf);
		/* cursor set at end of line */
		cursor = strlen(wk_buf);
		/* erase extra characters left over,
		 * if any */
		erase_nchar(MAX(0, cursor_max - cursor));
		cursor_max = cursor;
	      }
	      break;
	    }
	    if(get_echo_mode()==1)   {
		/* if this line differs from last line */
		if(lines_equal(wk_buf, 1) == FALSE) {
		    /* put in save buffer */
		    save_line(wk_buf);
		}
	    }
            goto exit;

	  case SEARCH_BACKWARD:
            if(tosearch[0]!=EXCL) {
	      putchar(BEL);
	    }
	    else {
              line_number = search_line_backward(tosearch);
	      if(line_number == 0) {
		putchar(BEL);
	      }
              else {
		cur_line_number=line_number;
		strcpy(wk_buf,sv_lines[line_number]);
		backspace(cursor);
		/* copy to screen */
		display_string(wk_buf);
		/* cursor set at end of line */
		cursor = strlen(wk_buf);
		/* erase extra characters left over,
		 * if any */
		erase_nchar(MAX(0, cursor_max - cursor));
		cursor_max = cursor;
	      }
	    }
	    break;

	  case SEARCH_FORWARD:
            if(tosearch[0]!=EXCL) {
	      putchar(BEL);
	    }
	    else {
              line_number = search_line_forward(tosearch);
	      if(line_number == 0) {
		putchar(BEL);
	      }
              else {
		cur_line_number=line_number;
		strcpy(wk_buf,sv_lines[line_number]);
		backspace(cursor);
		/* copy to screen */
		display_string(wk_buf);
		/* cursor set at end of line */
		cursor = strlen(wk_buf);
		/* erase extra characters left over,
		 * if any */
		erase_nchar(MAX(0, cursor_max - cursor));
		cursor_max = cursor;
	      }
	    }
	    break;

         default:
	    putchar(BEL);
            break;

         }

      } 
      else {
                            /* alpha/numeric keystroke.
                             * substitute blank fill for tab char */
         if(keystroke == '\t') {
            keystroke = ' ';
            character_count = TAB_SKIP - (cursor%TAB_SKIP);
            if(character_count == 0)
               character_count = TAB_SKIP;
         }
         else {
	   if(keystroke == EOF) {
	     character_count = 0;
	   }
	   else {
	     character_count = 1;
	   }
         }
         while(character_count--) {
	     if(get_echo_mode()==0) {
		 wk_buf[cursor] = keystroke;
		 cursor++;
	     }
	     else {
		 if(insert_flag) {
		     /* insert mode, move rest of line right and
		      * add character at cursor */
		     move_right(&wk_buf[cursor], WK_BUF_SIZE - cursor);
		     /* bump max cursor but not over buffer
		      * size */
		     cursor_max = (++cursor_max > WK_BUF_SIZE)
			 ?WK_BUF_SIZE : cursor_max;
		     /* if cursor at end of line, backspace so
		      * that new character overwrites last one */
		     if(cursor == WK_BUF_SIZE) {
			 cursor--;
			 backspace(1);
		     }
		     wk_buf[cursor] = keystroke;
		     display_string(&wk_buf[cursor]);
		     cursor++;
		     backspace(cursor_max - cursor);
               
		 } 
		 else {
		     /* overstrike mode */
		     if(cursor == WK_BUF_SIZE) {
			 cursor--;
			 backspace(1);
		     }
		     wk_buf[cursor] = keystroke;
		     putchar(keystroke);
		     if(cursor < WK_BUF_SIZE - 1) {
			 cursor++;
			 cursor_max = MAX(cursor_max, cursor);
		     }
		     else {
			 backspace(1);
		     }
		 }
	     }
         }
     }
  }

 exit:
   /* copy to return buffer */
   if(get_echo_mode()==0)  
     {
       *len_line=cursor;
       strncpy(buffer,wk_buf,*buf_size);
       set_echo_mode(TRUE);
       wk_buf[0] = NUL;
     }
   else {
     strcpy(buffer, wk_buf);
     /* fprintf(stderr,"[%s]\r\n",buffer); */
     *len_line = strlen(wk_buf);
     putchar('\r'); /** jpc avril 97 **/
     putchar('\n');
   }
#ifdef KEYPAD
   set_crmod();
   disable_keypad_mode();
#endif
   *eof = FALSE;
   set_is_reading(FALSE);
   return;

}

/***********************************************************************
 * move_right - move source string to one address larger (right)
 **********************************************************************/
static void
move_right(source, max_chars)
/**********************************************************************/
char *source;
int  max_chars;
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
static void
move_left(source)
/**********************************************************************/
char *source;
{
   do {                     /* move from left edge to left */
      *source = *(source + 1);
   } while(*source++ != NUL);
}

/***********************************************************************
 * display_string - display string starting at current cursor position
 **********************************************************************/

static void
display_string(string)
     char *string;
{
   while(*string != NUL) {
      putchar(*string++);
   }
}
/***********************************************************************
 * get_line()
 **********************************************************************/

static void
get_line(line_index, source)
     int  line_index;
     char *source;
{
   char *p;
                            /* pointer to line in save buffer */
   p = sv_lines[line_index];
                            /* if NUL, it-s an empty line */
   if(p == NULL) {
      *source = NUL;
      return;
   }
   while(*p != NUL) {
                            /* move each character from save buffer */
      *source++ = *p++;
                            /* if at end of save buffer, wrap */
      if(p - sv_buf == SV_BUF_SIZE) p = sv_buf;
   }
                            /* add terminator to line */
   *source = NUL;
}
/***********************************************************************
 * save_line - save edited line in next position in save buffer
 **********************************************************************/
static void
save_line(source)
/**********************************************************************/
char *source;
{
   int i;
   int length;
                            /* if empty line, don-t save it */
   if(*source == NUL) return;
                            /* bump saved line count but not over max */
   if(last_line < NO_SAVED_LINES - 1) last_line++;
                            /* point last to nothing */
   sv_lines[last_line] = NULL;
                            /* move up rest to make space at slot one */
   for(i = last_line - 1; i > 1; i--) {
      sv_lines[i] = sv_lines[i - 1];
   }
                            /* point slot one to where new line
			     * is going */
   sv_lines[1] = sv_buf_point;
                            /* move active buffer to save with wrap */
   for(length = 0; *source != NUL; length++) {
      *sv_buf_point = *source++;
                            /* bump save buffer pointer and if at end of
                             * buffer, wrap back to zero */
      if((++sv_buf_point - sv_buf) == SV_BUF_SIZE)
         sv_buf_point = sv_buf;
   }
                            /* terminate string in save buffer and leave
			     * pointer ready for next time */
   *sv_buf_point++ = NUL;
                            /* check if any lines point into new line */
   for(i = last_line; i > 1; i--) {
                            /* if not pointing anywhere */
      if(sv_lines[i] == NULL)
         continue;
                            /* if pointing into new line */
      if((sv_lines[i] - sv_lines[1] + SV_BUF_SIZE)%SV_BUF_SIZE
						<= length + 1) {
         sv_lines[i] = NULL;
			    /* and reduce last saved line */
	 last_line = i;
      }
   }
}
/***********************************************************************
 * backspace - move cursor n char to the left
 **********************************************************************/
static void
backspace(n)
/**********************************************************************/
int n;
{
   if(n < 1)
      return;

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
static void
erase_nchar(n)
/**********************************************************************/
int n;
{
   int i;                   /* fill field with blanks */
   for(i = 0; i < n; i++) {
      putchar(' ');
   }
   backspace(n);            /* and back up over blanks just written */
}
/***********************************************************************
 * lines_equal(source, line_index)
 **********************************************************************/
static int
lines_equal(source, line_index)
/**********************************************************************/
char *source;
int  line_index;
{
   char *p;

                            /* if empty line, matches anything */
   if(*source == NUL) return(TRUE);
                            /* point to line start in save buffer */
   p = sv_lines[line_index];
                            /* if line empty, report not equal */
   if(p == NULL) return(FALSE);
   while(*source != NUL) {
                            /* if not equal, return false */
      if(*source++ != *p++) {
         return(FALSE);
      }
                            /* if outside buffer, wrap to beginning */
      if(p - sv_buf == SV_BUF_SIZE) {
         p = sv_buf;
      }
   }
   return (*p == NUL) ? TRUE : FALSE;
}
/***********************************************************************
 * strip_blank(source) - strip trailing blanks by inserting NUL-s
 **********************************************************************/
static void
strip_blank(source)
/**********************************************************************/
char *source;
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
/***********************************************************************
 * get sungle character with no echo
 **********************************************************************/
int gchar_no_echo()
/**********************************************************************/
{
   int i;
   /* get next character, gotten in cbreak mode
    * so no wait for <cr> */
   i = Xorgetchar();
   /* if more than one character */
   if(i == ESC) {
     /* translate control code sequences to codes over 100 hex */
     i = translate(i);
   }
   return(i);
}

/*----------------------------------------------------------------------
 * set CBREAK mode and switch off echo and disable CR action!
 *----------------------------------------------------------------------*/

static void set_cbreak()
{
  /* switch to CBREAK mode without flushing
   * line buffer */
#ifdef B42UNIX
   arg.sg_flags |= CBREAK;
   arg.sg_flags &= ~ECHO; 
   arg.sg_flags &= ~CRMOD;
   ioctl(fd, TIOCSETN, &arg);
#endif

#ifdef ATTUNIX
   arg.c_lflag &= ~ICANON;
   arg.c_lflag &= ~ECHO;
   arg.c_iflag &= ~ICRNL;
   arg.c_oflag &= ~OPOST;
   arg.c_cc [VMIN] = 1;
   arg.c_cc [VTIME] = 0;
   ioctl(fd, TCSETAW, &arg);
#endif

   cbreak_crmod = 0;

   return;
}

/*----------------------------------------------------------------------
 * reset to original mode 
 *----------------------------------------------------------------------*/

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
   ioctl(fd, TCSETAW, &save_term);
#endif
   cbreak_crmod = 1;

   return;
}

/***********************************************************************
 * translate escape sequences
 **********************************************************************/
static int
translate(ichar)
/**********************************************************************/
int ichar;
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
                            /* return sequence mapped to integer */
               return(key_map[i]);
            else
                            /* flag not done with this sequence yet */
               not_done = 1;
         }
      }
                            /* if any sequence not finished yet */
      if(not_done) {
            ichar = Xorgetchar();
      }
      else {
                            /* hopefully at first character */
         break;
      }
   }

   return(ichar);
}

/*----------------------------------------------------------------------
 * initialise the io sequences
 *----------------------------------------------------------------------*/

static void init_io()
{
  int  tgetent();
  char *getenv();
  char *tgetstr();
  char tc_buf[1024];       /* holds termcap buffer */
  char *area;
  char erase_char;
  int i;
  /* check standard for interactive */
  fd=fileno(stdin);
  tty = isatty(fileno(stdin));
  if (tty == 0) return;

#ifdef B42UNIX
  ioctl(fd,TIOCGETP,&arg);
  save_sg_flags = arg.sg_flags;
  ioctl(fd,TIOCGETC,&arg1);
  erase_char = arg.sg_erase;
#endif

#ifdef ATTUNIX
  ioctl(fd, TCGETA, &arg);
  ioctl(fd, TCGETA, &save_term);
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
    KS = tgetstr("ks", &area);
    KE = tgetstr("ke", &area);
    CE = tgetstr("ce", &area);
    BC = tgetstr("bc", &area);
    IM = tgetstr("im", &area);
    IC = tgetstr("ic", &area);
    EI = tgetstr("ei", &area);
    CL = tgetstr("cl", &area);
  }
#endif

  setvbuf(stdin, NULL, _IONBF, 0); /* ehrlich juin 2001 */

}
#ifdef TERMCAP
/************************************************************************
 * enable keypad mode if using termcap
 ***********************************************************************/
static void
enable_keypad_mode()
/***********************************************************************/
{
                            /* enable keypad transmit mode */
   if(KS && *KS)
      fputs(KS, stdout);
}
/************************************************************************
 * disable the keypad mode if using termcap
 ***********************************************************************/
static void
disable_keypad_mode()
/***********************************************************************/
{
                            /* disable keypad transmit mode */
   if(KE && *KE)
      fputs(KE, stdout);
}
#else 
/************************************************************************
 *wee need references to thoses function if using KEYPAD but not Having TERMCAP
 ***********************************************************************/
static void
enable_keypad_mode(){}

static void
disable_keypad_mode(){}
#endif

 
/***********************************************************************
 * search_line_backward(source)
 **********************************************************************/
static int
search_line_backward(source)
/**********************************************************************/
char *source;
{
   int  line_index,ok,length;
   char *p,*p1;
   
   length=strlen(source)-1;
   ok=0;
   if (length == 0) {
     return(0);
   }

   p1 = source;
   p1++;
   for(line_index = cur_line_number+1 ; line_index < last_line; line_index++) {

     p = sv_lines[line_index];
     if(strncmp(p1,p,length) == 0) {
       ok = 1;
       break;
     }
   }

   if(ok==1) {
/*     strcpy(source,p);*/
     return(line_index);
   }
   else {
     return(0);
   }
}

/***********************************************************************
 * search_line_forward(source)
 **********************************************************************/
static int
search_line_forward(source)
/**********************************************************************/
char *source;
{
   int  line_index,ok,length;
   char *p,*p1;
   
   length=strlen(source)-1;
   ok=0;
   if (length == 0) {
     return(0);
   }

   p1 = source;
   p1++;
   for(line_index = cur_line_number-1 ; line_index > 1; line_index--) {

     p = sv_lines[line_index];
     if(strncmp(p1,p,length) == 0) {
       ok = 1;
       break;
     }
   }

   if(ok==1) {
     return(line_index);
   }
   else {
     return(0);
   }
}


void C2F(setprlev)(pause)
     int *pause;
{
  if ( *pause == 0 ) 
    sprintf(Sci_Prompt,"-->");
  else if ( *pause > 0 )
    sprintf(Sci_Prompt,"-%d->",*pause);
  else
    sprintf(Sci_Prompt,">>");
}

     
#endif /** WIN32 **/
