#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include "logger.h"
#include "pldstr.h"
#include "sci_malloc.h"
#include "localization.h"

#include "os_string.h"

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

/*-----------------------------------------------------------------\
 Function Name	: *PLD_strstr
 Returns Type	: char
 	----Parameter List
	1. char *haystack,
	2.  char *needle,
	3.  int insensitive,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
char *PLD_strstr(char *haystack, char *needle, int insensitive)
{
    char *hs;
    char *ne;
    char *result;

    /*	LOGGER_log("%s:%d:\nHS=%s\nNE=%s\nIS=%d\n",FL, haystack, needle, insensitive );*/

    if (insensitive > 0)
    {
        hs = os_strdup(haystack);
        PLD_strlower((unsigned char*) hs);
        ne = os_strdup(needle);
        PLD_strlower((unsigned char*) ne);
    }
    else
    {
        hs = haystack;
        ne = needle;
    }

    result = strstr(hs, ne);
    /*	if (result) LOGGER_log("%s:%d:HIT: %s",FL, result);*/
    /*	else LOGGER_log("%s:%d:MISS (looking for %s|%s)",FL, needle,ne);*/

    if ((result != NULL) && (insensitive > 0))
    {
        result = result - hs + haystack;
        FREE(hs);
        FREE(ne);

        /*		LOGGER_log("%s:%d:HIT - %s",FL, result );*/
    }

    return result;
}

/*------------------------------------------------------------------------
Procedure:     PLD_strncpy ID:1
Purpose:       Copy characters from 'src' to 'dst', writing not more than 'len'
characters to the destination, including the terminating \0.
Thus, for any effective copying, len must be > 1.
Input:         char *dst: Destination string
char *src: Source string
size_t len: length of string
Output:        Returns a pointer to the destination string.
Errors:
------------------------------------------------------------------------*/
char *PLD_strncpy (char *dst, const char *src, size_t len)
{

    /* Thanks go to 'defrost' of #c for providing the replacement*/
    /*		code which you now see here.  It covers the errors better*/
    /*		than my own previous code.*/

    /* If we have no buffer space, then it's futile attempting*/
    /*		to copy anything, just return NULL*/
    if (len == 0)
    {
        return NULL;
    }

    /* Providing our destination pointer isn't NULL, we can*/
    /*		commence copying data across*/

    if (dst)
    {
        char *dp = dst;

        /* If our source string exists, start moving it to the*/
        /*		destination string character at a time.*/
        if (src)
        {
            char *sp = (char *)src;
            while ((--len) && (*sp))
            {
                *dp = *sp;
                dp++;
                sp++;
            }
        }

        *dp = '\0';
    }

    return dst;
}



/*------------------------------------------------------------------------
Procedure:     PLD_strncat ID:1
Purpose:       Buffer size limited string concat function for two strings.
Input:         char *dst: Destination string
char *src: Source string
size_t len: Destination string buffer size - total string size cannot exceed this
Output:
Errors:        If the length of both strings in total is greater than the available buffer space
in *dst, we copy the maximum possible amount of chars from *src such that
buffer does not overflow.  A suffixed '\0' will always be appended.
------------------------------------------------------------------------*/
char *PLD_strncat( char *dst, const char *src, size_t len )
{
    char *dp = dst;
    const char *sp = src;
    size_t cc;

    if (len == 0)
    {
        return dst;
    }

    len--;

    /* Locate the end of the current string.*/
    cc = 0;
    while ((*dp) && (cc < len))
    {
        dp++;
        cc++;
    }

    /* If we have no more buffer space, then return the destination*/

    if (cc >= len)
    {
        return dst;
    }

    /* While we have more source, and there's more char space left in the buffer*/

    while ((*sp) && (cc < len))
    {
        cc++;
        *dp = *sp;
        dp++;
        sp++;
    }

    /* Terminate dst, as a gaurantee of string ending.*/

    *dp = '\0';

    return dst;
}


/*------------------------------------------------------------------------
Procedure:     PLD_strncate ID:1
Purpose:       Catencates a source string to the destination string starting from a given
endpoint.  This allows for faster catencation of strings by avoiding the
computation required to locate the endpoint of the destination string.
Input:         char *dst: Destination string
char *src: Source string
size_t len: Destination buffer size
char *endpoint: Endpoint of destination string, location from where new
string will be appended
Output:
Errors:
------------------------------------------------------------------------*/
char *PLD_strncate( char *dst, const char *src, size_t len, char *endpoint )
{
    char *dp = dst;
    const char *sp = src;
    size_t cc = 0;

    if (len == 0)
    {
        return dst;
    }

    len--;

    /* If endpoint does not relate correctly, then force manual detection*/
    /* of the endpoint.*/

    if ((!endpoint) || (endpoint == dst) || ((endpoint - dst + 1) > (int)len))
    {
        /* Locate the end of the current string.*/
        cc = 0;
        while ((*dp != '\0') && (cc < len))
        {
            dp++;
            cc++;
        }
    }
    else
    {
        cc = endpoint - dst + 1;
        dp = endpoint;
    }

    /* If we have no more buffer space, then return the destination*/

    if (cc >= len)
    {
        return dst;
    }

    /* While we have more source, and there's more char space left in the buffer*/

    while ((*sp) && (cc < len))
    {
        cc++;
        *dp = *sp;
        dp++;
        sp++;
    }

    /* Terminate dst, as a gaurantee of string ending.*/

    *dp = '\0';

    return dst;
}





/*------------------------------------------------------------------------
Procedure:     XAM_strncasecmp ID:1
Purpose:       Portable version of strncasecmp(), this may be removed in later
versions as the strncase* type functions are more widely
implemented
Input:
Output:
Errors:
------------------------------------------------------------------------*/
int PLD_strncasecmp( char *s1, char *s2, int n )
{
    char *ds1 = s1, *ds2 = s2;
    char c1, c2;
    int result = 0;

    while (n > 0)
    {
        c1 = (char)tolower(*ds1);
        c2 = (char)tolower(*ds2);

        if (c1 == c2)
        {
            n--;
            ds1++;
            ds2++;
        }
        else
        {
            result = c2 - c1;
            n = 0;
        }

    }

    return result;

}





/*------------------------------------------------------------------------
Procedure:     XAM_strtok ID:1
Purpose:       A thread safe version of strtok()
Input:
Output:
Errors:
------------------------------------------------------------------------*/
char *PLD_strtok( struct PLD_strtok *st, char *line, char *delimeters )
{
    char *stop;
    char *dc;
    char *result = NULL;

    if ( line )
    {
        st->start = line;
    }

    /*Strip off any leading delimeters*/

    dc = delimeters;
    while ((st->start) && (*dc != '\0'))
    {
        if (*dc == *(st->start))
        {
            st->start++;
            dc = delimeters;
        }
        else
        {
            dc++;
        }
    }

    /* Where we are left, is the start of our token.*/

    result = st->start;

    if ((st->start) && (*st->start != '\0'))
    {
        stop = strpbrk( st->start, delimeters ); /* locate our next delimeter */

        /* If we found a delimeter, then that is good.  We must now break the string here*/
        /* and don't forget to store the character which we stopped on.  Very useful bit*/
        /* of information for programs which process expressions.*/

        if (stop)
        {

            /* Store our delimeter.*/

            st->delimeter = *stop;

            /* Terminate our token.*/

            *stop = '\0';


            /* Because we're emulating strtok() behaviour here, we have to*/
            /* absorb all the concurrent delimeters, that is, unless we*/
            /* reach the end of the string, we cannot return a string with*/
            /* no chars.*/

            stop++;
            dc = delimeters;
            while (*dc != '\0')
            {
                if (*dc == *stop)
                {
                    stop++;
                    dc = delimeters;
                }
                else
                {
                    dc++;
                }
            } /* While*/

            if (*stop == '\0')
            {
                st->start = NULL;
            }
            else
            {
                st->start = stop;
            }

        }
        else
        {
            st->start = NULL;
            st->delimeter = '\0';
        }
    }
    else
    {
        st->start = NULL;
        result = NULL;
    }


    return result;
}



/*------------------------------------------------------------------------
Procedure:     PLD_strlower ID:1
Purpose:       Converts a string to lowercase
Input:         char *convertme : string to convert
Output:
Errors:
------------------------------------------------------------------------*/
int PLD_strlower( unsigned char *convertme )
{

    /* Updates:*/
    /*	09-11-2002 - changed from 'char *' to 'unsigned char *' to deal with*/
    /*		non-ASCII characters ( ie, french ).  Pointed out by Emmanuel Collignon*/

    unsigned char *c = convertme;

    while ( *c != '\0')
    {
        *c = (unsigned char)tolower((int) * c);
        c++;
    }

    return 0;
}


/*-----------------------------------------------------------------\
  Function Name	: *PLD_strreplace
  Returns Type	: char
  ----Parameter List
  1. char *source,		Original buffer, \0 terminated
  2.  char *searchfor, String sequence to search for
  3.  char *replacewith, String sequence to replace 'searchfor' with
  4.  int replacenumber , How many times to replace 'searchfor', 0 == unlimited
  ------------------
  Exit Codes	: Returns a pointer to the new buffer space.  The original
  buffer will still remain intact - ensure that the calling
  program FREE()'s the original buffer if it's no longer
  needed
  Side Effects	:
  --------------------------------------------------------------------
Comments:
Start out with static text matching - upgrade to regex later.

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
char *PLD_strreplace_general( struct PLD_strreplace *replace_details )
{
    char *new_buffer = NULL;
    char *source_end;
    char *segment_start, *segment_end, *segment_p;
    char *new_p;
    char *preexist_location = NULL;
    char *postexist_location = NULL;
    int replace_count = 0;
    int size_required;
    int size_difference;
    int source_length;
    int searchfor_length;
    int replacewith_length;
    int segment_ok;

    if (replace_details->source == NULL)
    {
        return NULL;
    }

    source_length = (int)strlen( replace_details->source );
    source_end = replace_details->source + source_length;
    searchfor_length = (int)strlen(replace_details->searchfor);
    replacewith_length = (int)strlen(replace_details->replacewith);
    size_difference = replacewith_length - searchfor_length;
    size_required = source_length;
    replace_count = replace_details->replacenumber;

    if ((replace_details->preexist != NULL) && (strlen(replace_details->preexist) < 1))
    {
        replace_details->preexist = NULL;
    }
    if ((replace_details->postexist != NULL) && (strlen(replace_details->postexist) < 1))
    {
        replace_details->postexist = NULL;
    }

    /* If we have a 'pre-exist' request, then we need to check this out first*/
    /*		because if the pre-exist string cannot be found, then there's very*/
    /*		little point us continuing on in our search ( because without the*/
    /*		preexist string existing, we are thus not qualified to replace anything )*/
    if (replace_details->preexist != NULL)
    {
        preexist_location = PLD_strstr(replace_details->source, replace_details->preexist, replace_details->insensitive);
        if (preexist_location == NULL)
        {
            return replace_details->source;
        }
    }

    /* Determine if initial POSTexist tests will pass, if we don't pick up*/
    /*		anything here, then there's no point in continuing either*/
    if (replace_details->postexist != NULL)
    {
        char *p = replace_details->source;
        postexist_location = NULL;
        do
        {
            p = PLD_strstr(p, replace_details->postexist, replace_details->insensitive);
            if (p != NULL)
            {
                postexist_location = p;
                p = p + strlen(replace_details->postexist);
            }
        }
        while (p != NULL);

        if (postexist_location == NULL)
        {
            return replace_details->source;
        }
    }


    /* Step 1 - determine the MAXIMUM number of times we might have to replace this string ( or the limit*/
    /*		set by replacenumber*/
    /**/
    /*	Note - we only need this number if the string we're going to be inserting into the */
    /*	source is larger than the one we're replacing - this is so that we can ensure that*/
    /*	we have sufficient memory available in the buffer.*/
    if (size_difference > 0)
    {
        if (replace_count == 0)
        {
            char *p, *q;

            p = replace_details->source;
            q = PLD_strstr(p, replace_details->searchfor, replace_details->insensitive);
            while (q != NULL)
            {
                replace_count++;
                /*size_required += size_difference;*/
                p = q + searchfor_length;
                q = PLD_strstr(p, replace_details->searchfor, replace_details->insensitive);
            }

        }
        size_required = source_length + (size_difference * replace_count) + 1;
    }
    else
    {
        size_required = source_length + 1;
    }


    /* Allocate the memory required to hold the new string [at least], check to see that*/
    /*		all went well, if not, then return an error*/
    new_buffer = MALLOC( sizeof(char) * size_required);
    if (new_buffer == NULL)
    {
        LOGGER_log(_("%s:%d:PLD_strreplace:ERROR: Cannot allocate %d bytes of memory to perform replacement operation"), FL, size_required);
        return replace_details->source;
    }

    /* Our segment must always start at the beginning of the source, */
    /*		on the other hand, the segment_end can be anything from the*/
    /*		next byte to NULL ( which is specially treated to mean to */
    /*		the end of the source )*/
    segment_start = replace_details->source;


    /* Locate the first segment */
    segment_ok = 0;
    segment_end = PLD_strstr(replace_details->source, replace_details->searchfor, replace_details->insensitive);

    /* Determine if the first segment is valid in the presence of the */
    /*	pre-exist and post-exist requirements*/
    while ((segment_end != NULL) && (segment_ok == 0)\
            && ((replace_details->preexist != NULL) || (replace_details->postexist != NULL)))
    {
        int pre_ok = 0;
        int post_ok = 0;

        /* The PREexist test assumes a couple of factors - please ensure these are*/
        /*		relevant if you change any code prior to this point.*/
        /*	*/
        /*	1. preexist_location has already been computed and is not NULL*/
        /**/
        /*	2. By relative position, the first preexist_location will be a valid location*/
        /*			on which to validate for ALL replacements beyond that point, thus, we*/
        /*			never actually have to recompute preexist_location again.*/
        /**/
        /* 3. Conversely, the last computed postexist_location is valid for all */
        /*			matches before it*/
        /**/
        if (preexist_location == NULL)
        {
            pre_ok = 1;
        }
        else if (preexist_location < segment_end)
        {
            pre_ok = 1;
        }

        if (postexist_location == NULL)
        {
            post_ok = 1;
        }
        else if (postexist_location > segment_end)
        {
            post_ok = 1;
        }

        if ((pre_ok == 0) || (post_ok == 0))
        {
            segment_end = PLD_strstr(segment_end + searchfor_length, replace_details->searchfor, replace_details->insensitive);
        }
        else
        {
            segment_ok = 1;
        }
    }

    segment_p = segment_start;
    new_p = new_buffer;
    while (segment_start != NULL)
    {
        int replacewith_count;
        char *replacewith_p;

        if (segment_end == NULL)
        {
            segment_end = source_end;
        }

        replace_count--;

        /* Perform the segment copy*/
        segment_p = segment_start;
        while ((segment_p < segment_end) && (size_required > 0))
        {
            *new_p = *segment_p;
            new_p++;
            segment_p++;
            size_required--;
        }

        /* Perform the string replacement*/
        if (segment_end < source_end)
        {
            replacewith_count = replacewith_length;
            replacewith_p = replace_details->replacewith;
            while ((replacewith_count--) && (size_required > 0))
            {
                *new_p = *replacewith_p;
                new_p++;
                replacewith_p++;
                size_required--;
            }
        }

        if (size_required < 1 )
        {
            LOGGER_log(_("%s:%d:PLD_strreplace_general: Allocated memory ran out while replacing '%s' with '%s'"), FL, replace_details->searchfor, replace_details->replacewith);
            *new_p = '\0';
            break;
        }

        /* Find the next segment*/
        segment_start = segment_end + searchfor_length;

        /* If we've reached the end of the number of replacements we're supposed*/
        /*		to do, then we prepare the termination of the while loop by setting*/
        /*		our segment end to the end of the source.*/
        /**/
        /* NOTE: Remember that the replace_count is pre-decremented at the start*/
        /*		of the while loop, so, if the caller requested '0' replacements*/
        /*		this will now be -1, thus, it won't get terminated from this == 0*/
        /*		match.  Just thought you'd like to be reminded of that incase you*/
        /*		were wondering "Huh? this would terminate an unlimited replacement"*/
        if (replace_count == 0)
        {
            segment_end = NULL;
        }
        else
        {
            /* If our new segment to copy starts after the*/
            /*		end of the source, then we actually have */
            /*		nothing else to copy, thus, we prepare the*/
            /*		segment_start varible to cause the while loop */
            /*		to terminate.*/
            /**/
            /* Otherwise, we try and locate the next segment*/
            /*		ending point, and set the starting point to*/
            /*		be on the 'other side' of the 'searchfor' string*/
            /*		which we found in the last search.*/
            /**/
            if (segment_start > source_end)
            {
                segment_start = NULL;
            }
            else
            {

                /* Try find the next segment*/
                segment_ok = 0;
                segment_end = PLD_strstr(segment_end + searchfor_length, replace_details->searchfor, replace_details->insensitive);

                /* If we have a pre/post-exist requirement, then enter into this*/
                /*		series of tests.  NOTE - at least one of the pre or post tests*/
                /*		must fire to give an meaningful result - else we'll end up with */
                /*		a loop which simply goes to the end of the searchspace buffer*/
                while ((segment_end != NULL) && (segment_ok == 0)\
                        && ((replace_details->preexist != NULL) || (replace_details->postexist != NULL)))
                {
                    int pre_ok = 0;
                    int post_ok = 0;

                    /* The PREexist test assumes a couple of factors - please ensure these are*/
                    /*		relevant if you change any code prior to this point.*/
                    /*	*/
                    /*	1. preexist_location has already been computed and is not NULL*/
                    /**/
                    /*	2. By relative position, the first preexist_location will be a valid location*/
                    /*			on which to validate for ALL replacements beyond that point, thus, we*/
                    /*			never actually have to recompute preexist_location again.*/
                    /**/
                    /* 3. Conversely, the last computed postexist_location is valid for all */
                    /*			matches before it*/
                    /**/
                    if (preexist_location == NULL)
                    {
                        pre_ok = 1;
                    }
                    else if (preexist_location < segment_end)
                    {
                        pre_ok = 1;
                    }

                    if (postexist_location == NULL)
                    {
                        post_ok = 1;
                    }
                    else if (postexist_location > segment_end)
                    {
                        post_ok = 1;
                    }

                    if ((pre_ok == 0) || (post_ok == 0))
                    {
                        segment_end = PLD_strstr(segment_end + searchfor_length, replace_details->searchfor, replace_details->insensitive);
                    }
                    else
                    {
                        segment_ok = 1;
                    }
                }

            } /* If-else segment_start > source_end*/

        }

    }

    *new_p = '\0';

    if (replace_details->source != NULL)
    {
        FREE (replace_details->source);
    }
    replace_details->source = new_buffer;
    return new_buffer;
}

/*-----------------------------------------------------------------\
  Function Name	: *PLD_strreplace
  Returns Type	: char
  ----Parameter List
  1. char **source,
  2.  char *searchfor,
  3.  char *replacewith,
  4.  int replacenumber ,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
char *PLD_strreplace( char **source, char *searchfor, char *replacewith, int replacenumber )
{
    struct PLD_strreplace replace_details;
    char *tmp_source;

    replace_details.source = *source;
    replace_details.searchfor = searchfor;
    replace_details.replacewith = replacewith;
    replace_details.replacenumber = replacenumber;
    replace_details.preexist = NULL;
    replace_details.postexist = NULL;
    replace_details.insensitive = 0;

    tmp_source = PLD_strreplace_general( &replace_details );

    if (tmp_source != *source)
    {
        *source = tmp_source;
    }

    return *source;
}


/*-----------------------------------------------------------------\
 Function Name	: *PLD_dprintf
 Returns Type	: char
 	----Parameter List
	1. const char *format,
	2.  ...,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:
	This is a dynamic string allocation function, not as fast as some
	other methods, but it works across the board with both glibc 2.0
	and 2.1 series.

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
char *PLD_dprintf(const char *format, ...)
{
    int n, size = 1024; /* Assume we don't need more than 1K to start with*/
    char *p;
    va_list ap;

    /* Attempt to allocate and then check */
    p = MALLOC(size * sizeof(char));
    if (p == NULL)
    {
        return NULL;
    }

    while (1)
    {
        /* Attempt to print out string out into the allocated space*/
        va_start(ap, format);

        n = vsnprintf (p, size, format, ap);
        va_end(ap);

        /* If things went well, then return the new string*/
        if ((n > -1) && (n < size))
        {
            return p;
        }

        /* If things didn't go well, then we have to allocate more space*/
        /*	based on which glibc we're using ( fortunately, the return codes*/
        /*	tell us which glibc is being used! *phew**/
        /**/
        /* If n > -1, then we're being told precisely how much space we need*/
        /* else (older glibc) we have to just guess again ...*/

        if (n > -1)
        {
            size = n + 1;    /* Allocate precisely what is needed*/
        }
        else
        {
            size *= 2;    /* Double the amount allocated, note, we could just increase by 1K, but if we have a long string, we'd end up using a lot of realloc's*/
        }

        /* We could just realloc 'blind', but that'd be wrong and potentially cause a DoS, so*/
        /*	instead, we'll be good and first attempt to realloc to a temp variable then, if all*/
        /*	is well, we go ahead and update*/
        if (1)
        {
            char *tmp_p;

            tmp_p = REALLOC(p, size);
            if (tmp_p == NULL)
            {
                if (p != NULL)
                {
                    FREE(p);
                }
                return NULL;
            }
            else
            {
                p = tmp_p;
            }
        }
    }

}


/*-----------------END.*/
