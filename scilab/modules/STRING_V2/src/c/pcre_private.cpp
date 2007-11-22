/*************************************************
*             PCRE testing program               *
*************************************************/

/* This program was hacked up as a tester for PCRE. I really should have
written it more tidily in the first place. Will I ever learn? It has grown and
been extended and consequently is now rather, er, *very* untidy in places.

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the University of Cambridge nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/



#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <errno.h>
#include "MALLOC.h"


/* A number of things vary for Windows builds. Originally, pcretest opened its
input and output without "b"; then I was told that "b" was needed in some
environments, so it was added for release 5.0 to both the input and output. (It
makes no difference on Unix-like systems.) Later I was told that it is wrong
for the input on Windows. I've now abstracted the modes into two macros that
are set here, to make it easier to fiddle with them, and removed "b" from the
input mode under Windows. */

#if defined(_WIN32) || defined(WIN32)
#include <io.h>                /* For _setmode() */
#include <fcntl.h>             /* For _O_BINARY */
#define INPUT_MODE   "r"
#define OUTPUT_MODE  "wb"

#else
#include <sys/time.h>          /* These two includes are needed */
#include <sys/resource.h>      /* for setrlimit(). */
#define INPUT_MODE   "rb"
#define OUTPUT_MODE  "wb"
#endif


/* We have to include pcre_internal.h because we need the internal info for
displaying the results of pcre_study() and we also need to know about the
internal macros, structures, and other internal data values; pcretest has
"inside information" compared to a program that strictly follows the PCRE API.

Although pcre_internal.h does itself include pcre.h, we explicitly include it
here before pcre_internal.h so that the PCRE_EXP_xxx macros get set
appropriately for an application, not for building PCRE. */

#include "pcre.h"
#include "pcre_internal.h"

/* We need access to the data tables that PCRE uses. So as not to have to keep
two copies, we include the source file here, changing the names of the external
symbols to prevent clashes. */

#define _pcre_utf8_table1      utf8_table1
#define _pcre_utf8_table1_size utf8_table1_size
#define _pcre_utf8_table2      utf8_table2
#define _pcre_utf8_table3      utf8_table3
#define _pcre_utf8_table4      utf8_table4
#define _pcre_utt              utt
#define _pcre_utt_size         utt_size
#define _pcre_utt_names        utt_names
#define _pcre_OP_lengths       OP_lengths

#include "pcre_tables.c"

/* We also need the pcre_printint() function for printing out compiled
patterns. This function is in a separate file so that it can be included in
pcre_compile.c when that module is compiled with debugging enabled.

The definition of the macro PRINTABLE, which determines whether to print an
output character as-is or as a hex value when showing compiled patterns, is
contained in this file. We uses it here also, in cases when the locale has not
been explicitly changed, so as to get consistent output from systems that
differ in their output from isprint() even in the "C" locale. */

//#include "pcre_printint.src"

#define PRINTHEX(c) (locale_set? isprint(c) : PRINTABLE(c))


/* It is possible to compile this test program without including support for
testing the POSIX interface, though this is not available via the standard
Makefile. */

#if !defined NOPOSIX
#include "pcreposix.h"
#endif

/* It is also possible, for the benefit of the version currently imported into
Exim, to build pcretest without support for UTF8 (define NOUTF8), without the
interface to the DFA matcher (NODFA), and without the doublecheck of the old
"info" function (define NOINFOCHECK). In fact, we automatically cut out the
UTF8 support if PCRE is built without it. */

#ifndef SUPPORT_UTF8
#ifndef NOUTF8
#define NOUTF8
#endif
#endif


/* Other parameters */

#ifndef CLOCKS_PER_SEC
#ifdef CLK_TCK
#define CLOCKS_PER_SEC CLK_TCK
#else
#define CLOCKS_PER_SEC 100
#endif
#endif

/* This is the default loop count for timing. */

#define LOOPREPEAT 500000

/* Static variables */

static FILE *outfile;
static int log_store = 0;
static int callout_count;
static int callout_extra;
static int callout_fail_count;
static int callout_fail_id;
static int debug_lengths;
static int first_callout;
static int locale_set = 0;
static int show_malloc;
static int use_utf8;
static size_t gotten_store;

/* The buffers grow automatically if very long input lines are encountered. */

static int buffer_size = 50000;
char *buffer = NULL;

static char *dbuffer = NULL;
static char *pbuffer = NULL;










/*************************************************
*              Callout function                  *
*************************************************/

/* Called from PCRE as a result of the (?C) item. We print out where we are in
the match. Yield zero unless more callouts than the fail count, or the callout
data is not zero. */

static int callout(pcre_callout_block *cb)
{






/* Always print appropriate indicators, with callout number if not already
shown. For automatic callouts, show the pattern offset. */










first_callout = 0;

if (cb->callout_data != NULL)
  {
  int callout_data = *((int *)(cb->callout_data));
  if (callout_data != 0)
    {
    return callout_data;
    }
  }

return (cb->callout_number != callout_fail_id)? 0 :
       (++callout_count >= callout_fail_count)? 1 : 0;
}


/*************************************************
*            Local malloc functions              *
*************************************************/

/* Alternative malloc function, to test functionality and show the size of the
compiled re. */

static void *new_malloc(size_t size)
{
void *block = MALLOC(size);
gotten_store = size;
return block;
}

static void new_free(void *block)
{
free(block);
}


/* For recursion malloc/free, to test stacking calls */

static void *stack_malloc(size_t size)
{
void *block = MALLOC(size);

return block;
}

static void stack_free(void *block)
{

FREE(block);
}


/*************************************************
*          Call pcre_fullinfo()                  *
*************************************************/

/* Get one piece of information from the pcre_fullinfo() function */

static void new_info(pcre *re, pcre_extra *study, int option, void *ptr)
{
int rc;
rc = pcre_fullinfo(re, study, option, ptr);
  
}



/*************************************************
*         Byte flipping function                 *
*************************************************/

static unsigned long int
byteflip(unsigned long int value, int n)
{
if (n == 2) return ((value & 0x00ff) << 8) | ((value & 0xff00) >> 8);
return ((value & 0x000000ff) << 24) |
       ((value & 0x0000ff00) <<  8) |
       ((value & 0x00ff0000) >>  8) |
       ((value & 0xff000000) >> 24);
}




/*************************************************
*        Check match or recursion limit          *
*************************************************/

static int
check_match_limit(pcre *re, pcre_extra *extra, char *bptr, int len,
  int start_offset, int options, int *use_offsets, int use_size_offsets,
  int flag, unsigned long int *limit, int errnumber, const char *msg)
{
int count;
int min = 0;
int mid = 64;
int max = -1;

extra->flags |= flag;

for (;;)
  {
  *limit = mid;

  count = pcre_exec(re, extra, (char *)bptr, len, start_offset, options,
    use_offsets, use_size_offsets);

  if (count == errnumber)
    {
    min = mid;
    mid = (mid == max - 1)? max : (max > 0)? (min + max)/2 : mid*2;
    }

  else if (count >= 0 || count == PCRE_ERROR_NOMATCH ||
                         count == PCRE_ERROR_PARTIAL)
    {
    if (mid == min + 1)
      {
      break;
      }
    max = mid;
    mid = (min + mid)/2;
    }
  else break;    /* Some other error */
  }

extra->flags &= ~flag;
return count;
}



/*************************************************
*         Case-independent strncmp() function    *
*************************************************/

/*
Arguments:
  s         first string
  t         second string
  n         number of characters to compare

Returns:    < 0, = 0, or > 0, according to the comparison
*/

static int
strncmpic(char *s, uschar *t, int n)
{
while (n--)
  {
  int c = tolower(*s++) - tolower(*t++);
  if (c) return c;
  }
return 0;
}



/*************************************************
*         Check newline indicator                *
*************************************************/

/* This is used both at compile and run-time to check for <xxx> escapes, where
xxx is LF, CR, CRLF, ANYCRLF, or ANY. Print a message and return 0 if there is
no match.

Arguments:
  p           points after the leading '<'
  f           file for error message

Returns:      appropriate PCRE_NEWLINE_xxx flags, or 0
*/

static int
check_newline(char *p, FILE *f)
{
if (strncmpic(p, (uschar *)"cr>", 3) == 0) return PCRE_NEWLINE_CR;
if (strncmpic(p, (uschar *)"lf>", 3) == 0) return PCRE_NEWLINE_LF;
if (strncmpic(p, (uschar *)"crlf>", 5) == 0) return PCRE_NEWLINE_CRLF;
if (strncmpic(p, (uschar *)"anycrlf>", 8) == 0) return PCRE_NEWLINE_ANYCRLF;
if (strncmpic(p, (uschar *)"any>", 4) == 0) return PCRE_NEWLINE_ANY;
if (strncmpic(p, (uschar *)"bsr_anycrlf>", 12) == 0) return PCRE_BSR_ANYCRLF;
if (strncmpic(p, (uschar *)"bsr_unicode>", 12) == 0) return PCRE_BSR_UNICODE;
fprintf(f, "Unknown newline type at: <%s\n", p);
return 0;
}





/*************************************************
*                Main Program                    *
*************************************************/

/* Read lines from named file or stdin and write to named file or stdout; lines
consist of a regular expression, in delimiters and optionally followed by
options, followed by a set of test data, terminated by an empty line. */

int pcre_private(char *INPUT_LINE,char *INPUT_PAT,int *Output_Start,int *Output_End)
{
FILE *infile = stdin;
int options = 0;
int study_options = 0;
int timeit = 0;
int timeitm = 0;
int showinfo = 0;
int showstore = 0;
int size_offsets = 45;
int size_offsets_max;
int *offsets = NULL;
#if !defined NOPOSIX
int posix = 0;
#endif
int debug = 0;
int all_use_dfa = 0;
int yield = 0;
int Cong_Tst;

/* These vectors store, end-to-end, a list of captured substring names. Assume
that 1024 is plenty long enough for the few names we'll be testing. */

uschar copynames[1024];
uschar getnames[1024];

uschar *copynamesptr;
uschar *getnamesptr;

/* Get buffers from malloc() so that Electric Fence will check their misuse
when I am debugging. They grow automatically when very long lines are read. */

buffer = (char *)MALLOC(buffer_size);
dbuffer = (char *)MALLOC(buffer_size);
pbuffer = (char *)MALLOC(buffer_size);

/* The outfile variable is static so that new_malloc can use it. */

outfile = stdout;



/* Get the store for the offsets vector, and remember what it was */

size_offsets_max = size_offsets;
offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
if (offsets == NULL)
  {
  printf("** Failed to get %d bytes of memory for offsets vector\n",
    (int)(size_offsets_max * sizeof(int)));
  yield = 1;
  goto EXIT;
  }




/* Set alternative malloc function */

pcre_malloc = new_malloc;
pcre_free = new_free;
pcre_stack_malloc = stack_malloc;
pcre_stack_free = stack_free;

/* Heading line unless quiet, then prompt for first regex if stdin */

//if (!quiet) fprintf(outfile, "PCRE version %s\n\n", pcre_version());

/* Main loop */
getchar();
Cong_Tst=0;
while (!Cong_Tst)
{
  pcre *re = NULL;
  pcre_extra *extra = NULL;
  
#if !defined NOPOSIX  /* There are still compilers that require no indent */
  regex_t preg;
  int do_posix = 0;
#endif

  const char *error;
  char *p; 
  char	*pp, *ppp;
  char *to_file = NULL;
  const char *tables = NULL;
  unsigned long int true_size, true_study_size = 0;
  size_t size, regex_gotten_store;
  int do_study = 0;
  int do_debug = debug;
  int do_G = 0;
  int do_g = 0;
  int do_showinfo = showinfo;
  int do_showrest = 0;
  int do_flip = 0;
  int erroroffset, len, delimiter, poffset;

  use_utf8 = 0;
  debug_lengths = 1;
  Cong_Tst=1;

 
  
  strcpy(buffer,INPUT_PAT); 
  p = buffer;
  while (isspace(*p)) p++;
  if (*p == 0) continue;

  /* See if the pattern is to be loaded pre-compiled from a file. */

  if (*p == '<' && strchr((char *)(p+1), '<') == NULL)
    {
    unsigned long int magic, get_options;
    uschar sbuf[8];
    FILE *f;

    p++;
    pp = p + (int)strlen((char *)p);
    while (isspace(pp[-1])) pp--;
    *pp = 0;

    f = fopen((char *)p, "rb");
    if (f == NULL)
      {
      continue;
      }

    if (fread(sbuf, 1, 8, f) != 8) goto FAIL_READ;

    true_size =
      (sbuf[0] << 24) | (sbuf[1] << 16) | (sbuf[2] << 8) | sbuf[3];
    true_study_size =
      (sbuf[4] << 24) | (sbuf[5] << 16) | (sbuf[6] << 8) | sbuf[7];

    re = (real_pcre *)new_malloc(true_size);
    regex_gotten_store = gotten_store;

    if (fread(re, 1, true_size, f) != true_size) goto FAIL_READ;

    magic = ((real_pcre *)re)->magic_number;
    if (magic != MAGIC_NUMBER)
      {
      if (byteflip(magic, sizeof(magic)) == MAGIC_NUMBER)
        {
        do_flip = 1;
        }
      else
        {
        continue;
        }
      }


    /* Need to know if UTF-8 for printing data strings */

    new_info(re, NULL, PCRE_INFO_OPTIONS, &get_options);
    use_utf8 = (get_options & PCRE_UTF8) != 0;

    /* Now see if there is any following study data */

    if (true_study_size != 0)
      {
      pcre_study_data *psd;

      extra = (pcre_extra *)new_malloc(sizeof(pcre_extra) + true_study_size);
      extra->flags = PCRE_EXTRA_STUDY_DATA;

      psd = (pcre_study_data *)(((char *)extra) + sizeof(pcre_extra));
      extra->study_data = psd;

      if (fread(psd, 1, true_study_size, f) != true_study_size)
        {
        FAIL_READ:
        if (extra != NULL) new_free(extra);
        if (re != NULL) new_free(re);
        fclose(f);
        continue;
        }
      do_study = 1;     /* To get the data output if requested */
      }
    goto SHOW_INFO;
  }

  /* In-line pattern (the usual case). Get the delimiter and seek the end of
  the pattern; if is isn't complete, read more. */

  delimiter = *p++;

  if (isalnum(delimiter) || delimiter == '\\')
    {
    fprintf(outfile, "** Delimiter must not be alphameric or \\\n");
    goto SKIP_DATA;
    }

  pp = p;
  poffset = (int)(p - buffer);

  for(;;)
    {
    while (*pp != 0)
      {
      if (*pp == '\\' && pp[1] != 0) pp++;
        else if (*pp == delimiter) break;
      pp++;
      }
    if (*pp != 0) break;

    }

  /* The buffer may have moved while being extended; reset the start of data
  pointer to the correct relative point in the buffer. */

  p = buffer + poffset;

  /* If the first character after the delimiter is backslash, make
  the pattern end with backslash. This is purely to provide a way
  of testing for the error message when a pattern ends with backslash. */

  if (pp[1] == '\\') *pp++ = '\\';

  /* Terminate the pattern at the delimiter, and save a copy of the pattern
  for callouts. */

  *pp++ = 0;
  strcpy((char *)pbuffer, (char *)p);

  /* Look for options after final delimiter */

  options = 0;
  study_options = 0;
  log_store = showstore;  /* default from command line */

  while (*pp != 0)
    {
    switch (*pp++)
      {
      case 'f': options |= PCRE_FIRSTLINE; break;
      case 'g': do_g = 1; break;
      case 'i': options |= PCRE_CASELESS; break;
      case 'm': options |= PCRE_MULTILINE; break;
      case 's': options |= PCRE_DOTALL; break;
      case 'x': options |= PCRE_EXTENDED; break;

      case '+': do_showrest = 1; break;
      case 'A': options |= PCRE_ANCHORED; break;
      case 'B': do_debug = 1; break;
      case 'C': options |= PCRE_AUTO_CALLOUT; break;
      case 'D': do_debug = do_showinfo = 1; break;
      case 'E': options |= PCRE_DOLLAR_ENDONLY; break;
      case 'F': do_flip = 1; break;
      case 'G': do_G = 1; break;
      case 'I': do_showinfo = 1; break;
      case 'J': options |= PCRE_DUPNAMES; break;
      case 'M': log_store = 1; break;
      case 'N': options |= PCRE_NO_AUTO_CAPTURE; break;

#if !defined NOPOSIX
      case 'P': do_posix = 1; break;
#endif

      case 'S': do_study = 1; break;
      case 'U': options |= PCRE_UNGREEDY; break;
      case 'X': options |= PCRE_EXTRA; break;
      case 'Z': debug_lengths = 0; break;
      case '8': options |= PCRE_UTF8; use_utf8 = 1; break;
      case '?': options |= PCRE_NO_UTF8_CHECK; break;

      case 'L':
      ppp = pp;
      /* The '\r' test here is so that it works on Windows. */
      /* The '0' test is just in case this is an unterminated line. */
      while (*ppp != 0 && *ppp != '\n' && *ppp != '\r' && *ppp != ' ') ppp++;
      *ppp = 0;
      if (setlocale(LC_CTYPE, (const char *)pp) == NULL)
        {
        goto SKIP_DATA;
        }
      locale_set = 1;
      tables = pcre_maketables();
      pp = ppp;
      break;

      case '>':
      to_file = pp;
      while (*pp != 0) pp++;
      while (isspace(pp[-1])) pp--;
      *pp = 0;
      break;

      case '<':
        {
        int x = check_newline(pp, outfile);
        if (x == 0) goto SKIP_DATA;
        options |= x;
        while (*pp++ != '>');
        }
      break;

      case '\r':                      /* So that it works in Windows */
      case '\n':
      case ' ':
      break;

      default:
      fprintf(outfile, "** Unknown option '%c'\n", pp[-1]);
      goto SKIP_DATA;
      }
    }

  /* Handle compiling via the POSIX interface, which doesn't support the
  timing, showing, or debugging options, nor the ability to pass over
  local character tables. */

#if !defined NOPOSIX
  if (posix || do_posix)
    {
    int rc;
    int cflags = 0;

    if ((options & PCRE_CASELESS) != 0) cflags |= REG_ICASE;
    if ((options & PCRE_MULTILINE) != 0) cflags |= REG_NEWLINE;
    if ((options & PCRE_DOTALL) != 0) cflags |= REG_DOTALL;
    if ((options & PCRE_NO_AUTO_CAPTURE) != 0) cflags |= REG_NOSUB;
    if ((options & PCRE_UTF8) != 0) cflags |= REG_UTF8;

    rc = regcomp(&preg, (char *)p, cflags);

    /* Compilation failed; go back for another re, skipping to blank line
    if non-interactive. */

    if (rc != 0)
      {
      (void)regerror(rc, &preg, (char *)buffer, buffer_size);
      fprintf(outfile, "Failed: POSIX code %d: %s\n", rc, buffer);
      goto SKIP_DATA;
      }
    }

  /* Handle compiling via the native interface */

  else
#endif  /* !defined NOPOSIX */

    {
    if (timeit > 0)
      {
      register int i;
      clock_t time_taken;
      clock_t start_time = clock();
      for (i = 0; i < timeit; i++)
        {
        re = pcre_compile((char *)p, options, &error, &erroroffset, tables);
        if (re != NULL) FREE(re);
        }
      time_taken = clock() - start_time;
      fprintf(outfile, "Compile time %.4f milliseconds\n",
        (((double)time_taken * 1000.0) / (double)timeit) /
          (double)CLOCKS_PER_SEC);
      }

    re = pcre_compile((char *)p, options, &error, &erroroffset, tables);

    /* Compilation failed; go back for another re, skipping to blank line
    if non-interactive. */

    if (re == NULL)
      {
      fprintf(outfile, "Failed: %s at offset %d\n", error, erroroffset);
      SKIP_DATA:
      if (infile != stdin)
        {
        for (;;)
          {
          /*if (extend_inputline(infile, buffer) == NULL)
            {
            done = 1;
            goto CONTINUE;
            }*/
          len = (int)strlen((char *)buffer);
          while (len > 0 && isspace(buffer[len-1])) len--;
          if (len == 0) break;
          }
        fprintf(outfile, "\n");
        }
      goto CONTINUE;
      }

    /* Compilation succeeded; print data if required. There are now two
    info-returning functions. The old one has a limited interface and
    returns only limited data. Check that it agrees with the newer one. */

    if (log_store)
      fprintf(outfile, "Memory allocation (code space): %d\n",
        (int)(gotten_store -
              sizeof(real_pcre) -
              ((real_pcre *)re)->name_count * ((real_pcre *)re)->name_entry_size));

    /* Extract the size for possible writing before possibly flipping it,
    and remember the store that was got. */

    true_size = ((real_pcre *)re)->size;
    regex_gotten_store = gotten_store;



   




    /* Extract information from the compiled data if required */

    SHOW_INFO:

 /*   if (do_debug)
      {
      fprintf(outfile, "------------------------------------------------------------------\n");
      pcre_printint(re, outfile, debug_lengths);
      }*/

    if (do_showinfo)
      {
      unsigned long int get_options, all_options;
#if !defined NOINFOCHECK
      int old_first_char, old_options, old_count;
#endif
      int count, backrefmax, first_char, need_char, okpartial, jchanged,
        hascrorlf;
      int nameentrysize, namecount;
      const uschar *nametable;

      new_info(re, NULL, PCRE_INFO_OPTIONS, &get_options);
      new_info(re, NULL, PCRE_INFO_SIZE, &size);
      new_info(re, NULL, PCRE_INFO_CAPTURECOUNT, &count);
      new_info(re, NULL, PCRE_INFO_BACKREFMAX, &backrefmax);
      new_info(re, NULL, PCRE_INFO_FIRSTBYTE, &first_char);
      new_info(re, NULL, PCRE_INFO_LASTLITERAL, &need_char);
      new_info(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &nameentrysize);
      new_info(re, NULL, PCRE_INFO_NAMECOUNT, &namecount);
      new_info(re, NULL, PCRE_INFO_NAMETABLE, (void *)&nametable);
      new_info(re, NULL, PCRE_INFO_OKPARTIAL, &okpartial);
      new_info(re, NULL, PCRE_INFO_JCHANGED, &jchanged);
      new_info(re, NULL, PCRE_INFO_HASCRORLF, &hascrorlf);

#if !defined NOINFOCHECK
      old_count = pcre_info(re, &old_options, &old_first_char);
      if (count < 0) fprintf(outfile,
        "Error %d from pcre_info()\n", count);
      else
        {
        if (old_count != count) fprintf(outfile,
          "Count disagreement: pcre_fullinfo=%d pcre_info=%d\n", count,
            old_count);

        if (old_first_char != first_char) fprintf(outfile,
          "First char disagreement: pcre_fullinfo=%d pcre_info=%d\n",
            first_char, old_first_char);

        if (old_options != (int)get_options) fprintf(outfile,
          "Options disagreement: pcre_fullinfo=%ld pcre_info=%d\n",
            get_options, old_options);
        }
#endif

      if (size != regex_gotten_store) fprintf(outfile,
        "Size disagreement: pcre_fullinfo=%d call to malloc for %d\n",
        (int)size, (int)regex_gotten_store);

      fprintf(outfile, "Capturing subpattern count = %d\n", count);
      if (backrefmax > 0)
        fprintf(outfile, "Max back reference = %d\n", backrefmax);

      if (namecount > 0)
        {
        fprintf(outfile, "Named capturing subpatterns:\n");
        while (namecount-- > 0)
          {
          fprintf(outfile, "  %s %*s%3d\n", nametable + 2,
            nameentrysize - 3 - (int)strlen((char *)nametable + 2), "",
            GET2(nametable, 0));
          nametable += nameentrysize;
          }
        }

      if (!okpartial) fprintf(outfile, "Partial matching not supported\n");
      if (hascrorlf) fprintf(outfile, "Contains explicit CR or LF match\n");

      all_options = ((real_pcre *)re)->options;
      if (do_flip) all_options = byteflip(all_options, sizeof(all_options));

      if (get_options == 0) fprintf(outfile, "No options\n");
        else fprintf(outfile, "Options:%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
          ((get_options & PCRE_ANCHORED) != 0)? " anchored" : "",
          ((get_options & PCRE_CASELESS) != 0)? " caseless" : "",
          ((get_options & PCRE_EXTENDED) != 0)? " extended" : "",
          ((get_options & PCRE_MULTILINE) != 0)? " multiline" : "",
          ((get_options & PCRE_FIRSTLINE) != 0)? " firstline" : "",
          ((get_options & PCRE_DOTALL) != 0)? " dotall" : "",
          ((get_options & PCRE_BSR_ANYCRLF) != 0)? " bsr_anycrlf" : "",
          ((get_options & PCRE_BSR_UNICODE) != 0)? " bsr_unicode" : "",
          ((get_options & PCRE_DOLLAR_ENDONLY) != 0)? " dollar_endonly" : "",
          ((get_options & PCRE_EXTRA) != 0)? " extra" : "",
          ((get_options & PCRE_UNGREEDY) != 0)? " ungreedy" : "",
          ((get_options & PCRE_NO_AUTO_CAPTURE) != 0)? " no_auto_capture" : "",
          ((get_options & PCRE_UTF8) != 0)? " utf8" : "",
          ((get_options & PCRE_NO_UTF8_CHECK) != 0)? " no_utf8_check" : "",
          ((get_options & PCRE_DUPNAMES) != 0)? " dupnames" : "");

      if (jchanged) fprintf(outfile, "Duplicate name status changes\n");

      switch (get_options & PCRE_NEWLINE_BITS)
        {
        case PCRE_NEWLINE_CR:
        fprintf(outfile, "Forced newline sequence: CR\n");
        break;

        case PCRE_NEWLINE_LF:
        fprintf(outfile, "Forced newline sequence: LF\n");
        break;

        case PCRE_NEWLINE_CRLF:
        fprintf(outfile, "Forced newline sequence: CRLF\n");
        break;

        case PCRE_NEWLINE_ANYCRLF:
        fprintf(outfile, "Forced newline sequence: ANYCRLF\n");
        break;

        case PCRE_NEWLINE_ANY:
        fprintf(outfile, "Forced newline sequence: ANY\n");
        break;

        default:
        break;
        }

      if (first_char == -1)
        {
        fprintf(outfile, "First char at start or follows newline\n");
        }
      else if (first_char < 0)
        {
        fprintf(outfile, "No first char\n");
        }
      else
        {
     /*   if (PRINTHEX(ch))
          fprintf(outfile, "First char = \'%c\'%s\n", ch, caseless);
        else
          fprintf(outfile, "First char = %d%s\n", ch, caseless);*/
        }

      if (need_char < 0)
        {
        fprintf(outfile, "No need char\n");
        }
      else
        {
//        if (PRINTHEX(ch))
//          fprintf(outfile, "Need char = \'%c\'%s\n", ch, caseless);
//        else
//          fprintf(outfile, "Need char = %d%s\n", ch, caseless);
        }

      /* Don't output study size; at present it is in any case a fixed
      value, but it varies, depending on the computer architecture, and
      so messes up the test suite. (And with the /F option, it might be
      flipped.) */

      if (do_study)
        {
        if (extra == NULL)
          fprintf(outfile, "Study returned NULL\n");
        else
          {
          uschar *start_bits = NULL;
          new_info(re, extra, PCRE_INFO_FIRSTTABLE, &start_bits);

          if (start_bits == NULL)
            fprintf(outfile, "No starting byte set\n");
          else
            {
            int i;
            int c = 24;
            fprintf(outfile, "Starting byte set: ");
            for (i = 0; i < 256; i++)
              {
              if ((start_bits[i/8] & (1<<(i&7))) != 0)
                {
                if (c > 75)
                  {
                  fprintf(outfile, "\n  ");
                  c = 2;
                  }
                /*if (PRINTHEX(i) && i != ' ')
                  {
                  fprintf(outfile, "%c ", i);
                  c += 2;
                  }
                else
                  {
                  fprintf(outfile, "\\x%02x ", i);
                  c += 5;
                  }*/
                }
              }
            fprintf(outfile, "\n");
            }
          }
        }
      }

    /* If the '>' option was present, we write out the regex to a file, and
    that is all. The first 8 bytes of the file are the regex length and then
    the study length, in big-endian order. */

    if (to_file != NULL)
      {
      FILE *f = fopen((char *)to_file, "wb");
      if (f == NULL)
        {
        fprintf(outfile, "Unable to open %s: %s\n", to_file, strerror(errno));
        }
      else
        {
        uschar sbuf[8];
        sbuf[0] = (uschar)((true_size >> 24) & 255);
        sbuf[1] = (uschar)((true_size >> 16) & 255);
        sbuf[2] = (uschar)((true_size >>  8) & 255);
        sbuf[3] = (uschar)((true_size) & 255);

        sbuf[4] = (uschar)((true_study_size >> 24) & 255);
        sbuf[5] = (uschar)((true_study_size >> 16) & 255);
        sbuf[6] = (uschar)((true_study_size >>  8) & 255);
        sbuf[7] = (uschar)((true_study_size) & 255);

        if (fwrite(sbuf, 1, 8, f) < 8 ||
            fwrite(re, 1, true_size, f) < true_size)
          {
          fprintf(outfile, "Write error on %s: %s\n", to_file, strerror(errno));
          }
        else
          {
          fprintf(outfile, "Compiled regex written to %s\n", to_file);
          if (extra != NULL)
            {
            if (fwrite(extra->study_data, 1, true_study_size, f) <
                true_study_size)
              {
              fprintf(outfile, "Write error on %s: %s\n", to_file,
                strerror(errno));
              }
            else fprintf(outfile, "Study data written to %s\n", to_file);

            }
          }
        fclose(f);
        }

      new_free(re);
      if (extra != NULL) new_free(extra);
      if (tables != NULL) new_free((void *)tables);
      continue;  /* With next regex */
      }
    }        /* End of non-POSIX compile */

  /* Read data lines and test them */

  //for (;;)
    {
    char *q;
    char *bptr;
    int *use_offsets = offsets;
    int use_size_offsets = size_offsets;
    int callout_data = 0;
    int callout_data_set = 0;
    int count, c;
    int copystrings = 0;
    int find_match_limit = 0;
    int getstrings = 0;
    int getlist = 0;
    int gmatched = 0;
    int start_offset = 0;
    int g_notempty = 0;
    int use_dfa = 0;
	

    options = 0;

    *copynames = 0;
    *getnames = 0;

    copynamesptr = copynames;
    getnamesptr = getnames;

    pcre_callout = callout;
    first_callout = 1;
    callout_extra = 0;
    callout_count = 0;
    callout_fail_count = 999999;
    callout_fail_id = -1;
    show_malloc = 0;

    if (extra != NULL) extra->flags &=
      ~(PCRE_EXTRA_MATCH_LIMIT|PCRE_EXTRA_MATCH_LIMIT_RECURSION);

    len = 0;

	strcpy(buffer,INPUT_LINE); 

    p = buffer;
    while (isspace(*p)) p++;

    bptr = q = dbuffer;
    while ((c = *p++) != 0)
      {
      int i = 0;
      int n = 0;

      if (c == '\\') switch ((c = *p++))
        {
        case 'a': c =    7; break;
        case 'b': c = '\b'; break;
        case 'e': c =   27; break;
        case 'f': c = '\f'; break;
        case 'n': c = '\n'; break;
        case 'r': c = '\r'; break;
        case 't': c = '\t'; break;
        case 'v': c = '\v'; break;

        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        c -= '0';
        while (i++ < 2 && isdigit(*p) && *p != '8' && *p != '9')
          c = c * 8 + *p++ - '0';


        break;

        case 'x':

        /* Handle \x{..} specially - new Perl thing for utf8 */



        /* Ordinary \x */

        c = 0;
        while (i++ < 2 && isxdigit(*p))
          {
          c = c * 16 + tolower(*p) - ((isdigit(*p))? '0' : 'W');
          p++;
          }
        break;

        case 0:   /* \ followed by EOF allows for an empty line */
        p--;
        continue;

        case '>':
        while(isdigit(*p)) start_offset = start_offset * 10 + *p++ - '0';
        continue;

        case 'A':  /* Option setting */
        options |= PCRE_ANCHORED;
        continue;

        case 'B':
        options |= PCRE_NOTBOL;
        continue;

        case 'C':
        if (isdigit(*p))    /* Set copy string */
          {
          while(isdigit(*p)) n = n * 10 + *p++ - '0';
          copystrings |= 1 << n;
          }
        else if (isalnum(*p))
          {
          uschar *npp = copynamesptr;
          while (isalnum(*p)) *npp++ = *p++;
          *npp++ = 0;
          *npp = 0;
          n = pcre_get_stringnumber(re, (char *)copynamesptr);
          if (n < 0)
            fprintf(outfile, "no parentheses with name \"%s\"\n", copynamesptr);
          copynamesptr = npp;
          }
        else if (*p == '+')
          {
          callout_extra = 1;
          p++;
          }
        else if (*p == '-')
          {
          pcre_callout = NULL;
          p++;
          }
        else if (*p == '!')
          {
          callout_fail_id = 0;
          p++;
          while(isdigit(*p))
            callout_fail_id = callout_fail_id * 10 + *p++ - '0';
          callout_fail_count = 0;
          if (*p == '!')
            {
            p++;
            while(isdigit(*p))
              callout_fail_count = callout_fail_count * 10 + *p++ - '0';
            }
          }
        else if (*p == '*')
          {
          int sign = 1;
          callout_data = 0;
          if (*(++p) == '-') { sign = -1; p++; }
          while(isdigit(*p))
            callout_data = callout_data * 10 + *p++ - '0';
          callout_data *= sign;
          callout_data_set = 1;
          }
        continue;



        case 'G':
        if (isdigit(*p))
          {
          while(isdigit(*p)) n = n * 10 + *p++ - '0';
          getstrings |= 1 << n;
          }
        else if (isalnum(*p))
          {
          uschar *npp = getnamesptr;
          while (isalnum(*p)) *npp++ = *p++;
          *npp++ = 0;
          *npp = 0;
          n = pcre_get_stringnumber(re, (char *)getnamesptr);
          if (n < 0)
            fprintf(outfile, "no parentheses with name \"%s\"\n", getnamesptr);
          getnamesptr = npp;
          }
        continue;

        case 'L':
        getlist = 1;
        continue;

        case 'M':
        find_match_limit = 1;
        continue;

        case 'N':
        options |= PCRE_NOTEMPTY;
        continue;

        case 'O':
        while(isdigit(*p)) n = n * 10 + *p++ - '0';
        if (n > size_offsets_max)
          {
          size_offsets_max = n;
          FREE(offsets);
          use_offsets = offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
          if (offsets == NULL)
            {
            printf("** Failed to get %d bytes of memory for offsets vector\n",
              (int)(size_offsets_max * sizeof(int)));
            yield = 1;
            goto EXIT;
            }
          }
        use_size_offsets = n;
        if (n == 0) use_offsets = NULL;   /* Ensures it can't write to it */
        continue;

        case 'P':
        options |= PCRE_PARTIAL;
        continue;

        case 'Q':
        while(isdigit(*p)) n = n * 10 + *p++ - '0';
        if (extra == NULL)
          {
          extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
          extra->flags = 0;
          }
        extra->flags |= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
        extra->match_limit_recursion = n;
        continue;

        case 'q':
        while(isdigit(*p)) n = n * 10 + *p++ - '0';
        if (extra == NULL)
          {
          extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
          extra->flags = 0;
          }
        extra->flags |= PCRE_EXTRA_MATCH_LIMIT;
        extra->match_limit = n;
        continue;

#if !defined NODFA
        case 'R':
        options |= PCRE_DFA_RESTART;
        continue;
#endif

        case 'S':
        show_malloc = 1;
        continue;

        case 'Z':
        options |= PCRE_NOTEOL;
        continue;

        case '?':
        options |= PCRE_NO_UTF8_CHECK;
        continue;

        case '<':
          {
          int x = check_newline(p, outfile);
          if (x == 0) goto NEXT_DATA;
          options |= x;
          while (*p++ != '>');
          }
        continue;
        }
      *q++ =(char)c;
      }
    *q = 0;
    len = (int)(q - dbuffer);

    if ((all_use_dfa || use_dfa) && find_match_limit)
      {
      printf("**Match limit not relevant for DFA matching: ignored\n");
      find_match_limit = 0;
      }

    /* Handle matching via the POSIX interface, which does not
    support timing or playing with the match limit or callout data. */



    for (;; gmatched++)    /* Loop for /g or /G */
      {
      if (timeitm > 0)
        {
        register int i;
        clock_t time_taken;
        clock_t start_time = clock();

#if !defined NODFA
        if (all_use_dfa || use_dfa)
          {
          int workspace[1000];
          for (i = 0; i < timeitm; i++)
            count = pcre_dfa_exec(re, NULL, (char *)bptr, len, start_offset,
              options | g_notempty, use_offsets, use_size_offsets, workspace,
              sizeof(workspace)/sizeof(int));
          }
        else
#endif

        for (i = 0; i < timeitm; i++)
          count = pcre_exec(re, extra, (char *)bptr, len,
            start_offset, options | g_notempty, use_offsets, use_size_offsets);

        time_taken = clock() - start_time;
        fprintf(outfile, "Execute time %.4f milliseconds\n",
          (((double)time_taken * 1000.0) / (double)timeitm) /
            (double)CLOCKS_PER_SEC);
        }

      /* If find_match_limit is set, we want to do repeated matches with
      varying limits in order to find the minimum value for the match limit and
      for the recursion limit. */

      if (find_match_limit)
        {
        if (extra == NULL)
          {
          extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
          extra->flags = 0;
          }

        (void)check_match_limit(re, extra, bptr, len, start_offset,
          options|g_notempty, use_offsets, use_size_offsets,
          PCRE_EXTRA_MATCH_LIMIT, &(extra->match_limit),
          PCRE_ERROR_MATCHLIMIT, "match()");

        count = check_match_limit(re, extra, bptr, len, start_offset,
          options|g_notempty, use_offsets, use_size_offsets,
          PCRE_EXTRA_MATCH_LIMIT_RECURSION, &(extra->match_limit_recursion),
          PCRE_ERROR_RECURSIONLIMIT, "match() recursion");
        }

      /* If callout_data is set, use the interface with additional data */

      else if (callout_data_set)
        {
        if (extra == NULL)
          {
          extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
          extra->flags = 0;
          }
        extra->flags |= PCRE_EXTRA_CALLOUT_DATA;
        extra->callout_data = &callout_data;
        count = pcre_exec(re, extra, (char *)bptr, len, start_offset,
          options | g_notempty, use_offsets, use_size_offsets);
        extra->flags &= ~PCRE_EXTRA_CALLOUT_DATA;
        }

      /* The normal case is just to do the match once, with the default
      value of match_limit. */


      else
        {
        count = pcre_exec(re, extra, (char *)bptr, len,
          start_offset, options | g_notempty, use_offsets, use_size_offsets);
        if (count == 0)
          {
          fprintf(outfile, "Matched, but too many substrings\n");
          count = use_size_offsets/3;
          }
        }

      /* Matched */

      if (count >= 0)
        {
        int i, maxcount;


          maxcount = use_size_offsets/3;

        /* This is a check against a lunatic return value. */

        if (count > maxcount)
          {
          fprintf(outfile,
            "** PCRE error: returned count %d is too big for offset size %d\n",
            count, use_size_offsets);
          count = use_size_offsets/3;
          if (do_g || do_G)
            {
            do_g = do_G = FALSE;        /* Break g/G loop */
            }
          }

        for (i = 0; i < count * 2; i += 2)
          {
          if (use_offsets[i] < 0)
            fprintf(outfile, "%2d: <unset>\n", i/2);
          else
            {
            //fprintf(outfile, "%2d: ", i/2);
            //(void)pchars(bptr + use_offsets[i],
            //  use_offsets[i+1] - use_offsets[i], outfile);
            //fprintf(outfile, "\n");
		    *Output_Start=use_offsets[i];
			*Output_End=use_offsets[i+1];
				
				return 0;
            
            }
          }

        for (i = 0; i < 32; i++)
          {
          if ((copystrings & (1 << i)) != 0)
            {
            char copybuffer[256];
            int rc = pcre_copy_substring((char *)bptr, use_offsets, count,
              i, copybuffer, sizeof(copybuffer));
            if (rc < 0)
              fprintf(outfile, "copy substring %d failed %d\n", i, rc);
            else
              fprintf(outfile, "%2dC %s (%d)\n", i, copybuffer, rc);
            }
          }

        for (copynamesptr = copynames;
             *copynamesptr != 0;
             copynamesptr += (int)strlen((char*)copynamesptr) + 1)
          {
          char copybuffer[256];
          int rc = pcre_copy_named_substring(re, (char *)bptr, use_offsets,
            count, (char *)copynamesptr, copybuffer, sizeof(copybuffer));
          if (rc < 0)
            fprintf(outfile, "copy substring %s failed %d\n", copynamesptr, rc);
          else
            fprintf(outfile, "  C %s (%d) %s\n", copybuffer, rc, copynamesptr);
          }

        for (i = 0; i < 32; i++)
          {
          if ((getstrings & (1 << i)) != 0)
            {
            const char *substring;
            int rc = pcre_get_substring((char *)bptr, use_offsets, count,
              i, &substring);
            if (rc < 0)
              fprintf(outfile, "get substring %d failed %d\n", i, rc);
            else
              {
              fprintf(outfile, "%2dG %s (%d)\n", i, substring, rc);
              pcre_free_substring(substring);
              }
            }
          }

        for (getnamesptr = getnames;
             *getnamesptr != 0;
             getnamesptr += (int)strlen((char*)getnamesptr) + 1)
          {
          const char *substring;
          int rc = pcre_get_named_substring(re, (char *)bptr, use_offsets,
            count, (char *)getnamesptr, &substring);
          if (rc < 0)
            fprintf(outfile, "copy substring %s failed %d\n", getnamesptr, rc);
          else
            {
            fprintf(outfile, "  G %s (%d) %s\n", substring, rc, getnamesptr);
            pcre_free_substring(substring);
            }
          }

        if (getlist)
          {
          const char **stringlist;
          int rc = pcre_get_substring_list((char *)bptr, use_offsets, count,
            &stringlist);
          if (rc < 0)
            fprintf(outfile, "get substring list failed %d\n", rc);
          else
            {
            for (i = 0; i < count; i++)
              fprintf(outfile, "%2dL %s\n", i, stringlist[i]);
            if (stringlist[i] != NULL)
              fprintf(outfile, "string list not terminated by NULL\n");
            /* free((void *)stringlist); */
            pcre_free_substring_list(stringlist);
            }
          }
        }



 

      /* Failed to match. If this is a /g or /G loop and we previously set
      g_notempty after a null match, this is not necessarily the end. We want
      to advance the start offset, and continue. We won't be at the end of the
      string - that was checked before setting g_notempty.

      Complication arises in the case when the newline option is "any" or
      "anycrlf". If the previous match was at the end of a line terminated by
      CRLF, an advance of one character just passes the \r, whereas we should
      prefer the longer newline sequence, as does the code in pcre_exec().
      Fudge the offset value to achieve this.

      Otherwise, in the case of UTF-8 matching, the advance must be one
      character, not one byte. */

      else
        {
        if (g_notempty != 0)
          {
          int onechar = 1;
          unsigned int obits = ((real_pcre *)re)->options;
          use_offsets[0] = start_offset;
          if ((obits & PCRE_NEWLINE_BITS) == 0)
            {
            int d;
            (void)pcre_config(PCRE_CONFIG_NEWLINE, &d);
            obits = (d == '\r')? PCRE_NEWLINE_CR :
                    (d == '\n')? PCRE_NEWLINE_LF :
                    (d == ('\r'<<8 | '\n'))? PCRE_NEWLINE_CRLF :
                    (d == -2)? PCRE_NEWLINE_ANYCRLF :
                    (d == -1)? PCRE_NEWLINE_ANY : 0;
            }
          if (((obits & PCRE_NEWLINE_BITS) == PCRE_NEWLINE_ANY ||
               (obits & PCRE_NEWLINE_BITS) == PCRE_NEWLINE_ANYCRLF)
              &&
              start_offset < len - 1 &&
              bptr[start_offset] == '\r' &&
              bptr[start_offset+1] == '\n')
            onechar++;
          else if (use_utf8)
            {
            while (start_offset + onechar < len)
              {
              int tb = bptr[start_offset+onechar];
              if (tb <= 127) break;
              tb &= 0xc0;
              if (tb != 0 && tb != 0xc0) onechar++;
              }
            }
          use_offsets[1] = start_offset + onechar;
          }
        else
          {
          if (count == PCRE_ERROR_NOMATCH)
            {
            if (gmatched == 0) 
			{
				fprintf(outfile, "No match\n");
				return -1;
			}
			}
          else fprintf(outfile, "Error %d\n", count);
          break;  /* Out of the /g loop */
          }
        }

      /* If not /g or /G we are done */

      if (!do_g && !do_G) break;

      /* If we have matched an empty string, first check to see if we are at
      the end of the subject. If so, the /g loop is over. Otherwise, mimic
      what Perl's /g options does. This turns out to be rather cunning. First
      we set PCRE_NOTEMPTY and PCRE_ANCHORED and try the match again at the
      same point. If this fails (picked up above) we advance to the next
      character. */

      g_notempty = 0;

      if (use_offsets[0] == use_offsets[1])
        {
        if (use_offsets[0] == len) break;
        g_notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
        }

      /* For /g, update the start offset, leaving the rest alone */

      if (do_g) start_offset = use_offsets[1];

      /* For /G, update the pointer and length */

      else
        {
        bptr += use_offsets[1];
        len -= use_offsets[1];
        }
      }  /* End of loop for /g and /G */

    NEXT_DATA: continue;
    }    /* End of loop for data lines */

  CONTINUE:

#if !defined NOPOSIX
  if (posix || do_posix) regfree(&preg);
#endif

  if (re != NULL) new_free(re);
  if (extra != NULL) new_free(extra);
  if (tables != NULL)
    {
    new_free((void *)tables);
    setlocale(LC_CTYPE, "C");
    locale_set = 0;
    }
  }

if (infile == stdin) fprintf(outfile, "\n");

EXIT:

if (infile != NULL && infile != stdin) fclose(infile);
if (outfile != NULL && outfile != stdout) fclose(outfile);

FREE(buffer);
FREE(dbuffer);
FREE(pbuffer);
FREE(offsets);
getchar();


}