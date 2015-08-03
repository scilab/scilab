
/* Abstract logging system used to facilitate multiple modes*/
/* of logging*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <syslog.h>
#endif
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

#include "localization.h"
#include "logger.h"
#include "sci_malloc.h"
#include "charEncoding.h"

//#define SYSLOG_ENABLE 1

#ifndef _MSC_VER
static int _LOGGER_mode = _LOGGER_SYSLOG;
static int _LOGGER_syslog_mode = LOG_MAIL | LOG_INFO;
#else
static int _LOGGER_mode = _LOGGER_STDERR;
static int _LOGGER_syslog_mode = 0;
#endif

static FILE *_LOGGER_outf;

struct LOGGER_globals
{
    int wrap;
    int wraplength;
};

/* Create and Initialise the global structure for LOGGER,*/
/*		we init it to have NO wrapping.*/

static struct LOGGER_globals LOGGER_glb = { 0, 0 };

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

/*------------------------------------------------------------------------
Procedure:     LOGGER_get_file ID:1
Purpose:       Returns the pointer to the file being used to output logs to
Input:
Output:
Errors:
------------------------------------------------------------------------*/
FILE *LOGGER_get_file( void )
{
    return _LOGGER_outf;
}


/*------------------------------------------------------------------------
Procedure:     LOGGER_set_output_mode ID:1
Purpose:       Sets the message/log output method, ie, stderr, stdout
or syslog
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_output_mode( int modechoice )
{
    _LOGGER_mode = modechoice;
    return 0;
}

/*------------------------------------------------------------------------
Procedure:     LOGGER_set_output_file ID:1
Purpose:       Sets the output file for when _LOGGER_mode is set to
_LOGGER_file
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_output_file( FILE *f )
{
    _LOGGER_outf = f;
    return 0;
}

/*------------------------------------------------------------------------
Procedure:     LOGGER_set_syslog_mode ID:1
Purpose:       Sets the mode that messaging to the syslog daemon will
be sent as (ie, LOG_MAIL|LOG_INFO)
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_syslog_mode( int syslogmode )
{
    _LOGGER_syslog_mode = syslogmode;
    return 0;
}




/*------------------------------------------------------------------------
Procedure:     LOGGER_set_logfile ID:1
Purpose:       Opens and setups the internal Log file file pointer with the
log file as given by lfname
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_logfile( char *lfname )
{
    int result = 0;

    wcfopen(_LOGGER_outf, lfname, "a");
    if (!_LOGGER_outf)
    {
#if !defined(_MSC_VER) && defined(SYSLOG_ENABLE)
        syslog(1, _("LOGGER_set_logfile: ERROR - Cannot open logfile '%s' (%s)"), lfname, strerror(errno));
#else
        fprintf(stderr, _("LOGGER_set_logfile: ERROR - Cannot open logfile '%s' (%s)\n"), lfname, strerror(errno));
#endif
        result = -1;
    }

    return result;
}



/*------------------------------------------------------------------------
Procedure:     LOGGER_set_wraplength ID:1
Purpose:       Sets the character count at which LOGGER will break a line
Input:         int length: Positive int indicating number of chracters at which to wrap at
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_wraplength( int length )
{
    if ( length >= 0 )
    {
        LOGGER_glb.wraplength = length;
    }

    return LOGGER_glb.wraplength;
}

/*------------------------------------------------------------------------
Procedure:     LOGGER_set_wrap ID:1
Purpose:       Set log output wrapping to on or off
Input:         int level: 0 = no wrap, > 0 = wrap.
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_set_wrap( int level )
{
    if ( level >= 0 )
    {
        LOGGER_glb.wrap = level;
    }

    return LOGGER_glb.wrap;
}



/*------------------------------------------------------------------------
Procedure:     LOGGER_close_logfile ID:1
Purpose:       Closes the modules log file pointer.
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_close_logfile( void )
{
    int result = 0;

    if (_LOGGER_outf)
    {
        fclose(_LOGGER_outf);
    }

    return result;
}



/*------------------------------------------------------------------------
Procedure:     LOGGER_clean_output ID:1
Purpose:       Checks through the output string for any characters which could cause
potential 'isssues' with the data writing calls, items such as stray non-escaped
% characters can cause havoc.
Input:         char *string: Raw string
int maxsize: Maximum available buffer size for this string to expand to
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_clean_output( char *string, char **buffer )
{
    char *newstr;
    char *p, *q;
    char *next_space;

    int pc;
    int slen = (int)strlen( string );
    int line_size;
    int maxsize = slen * 2;

    /* First up, allocate maxsize bytes for a temporary new string.*/
    newstr = MALLOC(slen * 2 + 1);
    if ( newstr == NULL )
    {
        /* FIXME - Report an error here ... to -somewhere-*/
        return -1;
    }

    p = newstr;
    q = string;
    pc = 0;
    line_size = 0;

    while (slen--)
    {

        /* Do we need to apply any wrapping to the output? If so then we*/
        /*		shall embark on a journey of strange space and distance*/
        /*		evaluations to determine if we should wrap now or later*/

        if ( LOGGER_glb.wrap > 0 )
        {
            if (isspace((int)*q))
            {
                next_space = strpbrk( (q + 1), "\t\n\v " );
                if (next_space != NULL)
                {
                    if ((line_size + (next_space - q)) >= LOGGER_glb.wraplength)
                    {
                        *p = '\n';
                        p++;
                        pc++;
                        line_size = 0;
                    }
                }
            }

            if ( line_size >= LOGGER_glb.wraplength )
            {
                *p = '\n';
                p++;
                pc++;
                line_size = 0;
            }
        }

        /* If the string has a % in it, then we need to encode it as*/
        /*	a DOUBLE % symbol.*/

        if (*q == '%')
        {
            /*			if (strchr("fdlsxXn",*(q+1)))*/
            /*			{*/
            *p = '%';
            p++;
            pc++;
            /*			}*/
        }

        /* Copy the character of the string in*/
        *p = *q;

        /* Move everything along.*/
        q++;
        p++;
        pc++;
        line_size++;

        if ( pc > (maxsize - 1) )
        {
            break;
        }
    }

    *p = '\0';

    /* This will have to be deallocated later!*/
    if (newstr)
    {
        *buffer = newstr;
    }

    return 0;
}

/*------------------------------------------------------------------------
Procedure:     LOGGER_log ID:1
Purpose:       Logs the params as supplied to the required
output as defined by LOGGER_set_output
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int LOGGER_log( char *format, ...)
{
    va_list ptr;
    char tmpoutput[10240];
    char linebreak[] = "\n";
    char nolinebreak[] = "";
    char *lineend;
    char *output;


    /* get our variable arguments*/
    va_start(ptr, format);

    /* produce output, and spit to the log file*/
#ifdef NO_SNPRINTF
    vsprintf(tmpoutput, format, ptr);
#else
    vsnprintf(tmpoutput, 10240, format, ptr);
#endif

    LOGGER_clean_output( tmpoutput, &output );

    if ( output[strlen(output) - 1] == '\n' )
    {
        lineend = nolinebreak;
    }
    else
    {
        lineend = linebreak;
    }

    if ( output[strlen(output) - 1] == '\n' )
    {
        lineend = nolinebreak;
    }
    else
    {
        lineend = linebreak;
    }

    /* Send the output to the appropriate output destination*/
    switch (_LOGGER_mode)
    {
        case _LOGGER_SYSLOG:
#if !defined(_MSC_VER) && defined(SYSLOG_ENABLE)
            syslog(_LOGGER_syslog_mode, "%s", output);
            break;
#endif
        case _LOGGER_STDERR:
            fprintf(stderr, "%s%s", output, lineend );
            break;

        case _LOGGER_STDOUT:
            fprintf(stdout, "%s%s", output, lineend);
            fflush(stdout);
            break;
        case _LOGGER_FILE:
            fprintf(_LOGGER_outf, "%s%s", output, lineend);
            fflush(_LOGGER_outf);
            break;
        default:
            fprintf(stdout, _("LOGGER-Default: %s%s"), output, lineend);
    }


    if (output)
    {
        FREE(output);
    }

    return 0;
}




