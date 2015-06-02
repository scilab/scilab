#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "logger.h"
#include "pldstr.h"
#include "bytedecoders.h"
#include "olestream-unwrap.h"
#include "localization.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"


#define DUW if (oleuw->debug)

struct OLE10_header
{
    unsigned char data[6];
    char *attach_name;
    unsigned char data2[8];
    char *fname_1;
    char *fname_2;
    size_t attach_size;
    size_t attach_size_1;
    size_t attach_start_offset;
};


/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_init
 Returns Type	: int
 	----Parameter List
	1. struct OLEUNWRAP_object *oleuw ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_init( struct OLEUNWRAP_object *oleuw )
{
    oleuw->debug = 0;
    oleuw->verbose = 0;
    oleuw->filename_report_fn = NULL;

    return OLEUW_OK;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_set_debug
 Returns Type	: int
 	----Parameter List
	1. struct OLEUNWRAP_object *oleuw,
	2.  int level ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_set_debug( struct OLEUNWRAP_object *oleuw, int level )
{
    oleuw->debug = level;
    return OLEUW_OK;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_set_verbose
 Returns Type	: int
 	----Parameter List
	1. struct OLEUNWRAP_object *oleuw,
	2.  int level ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_set_verbose( struct OLEUNWRAP_object *oleuw, int level )
{
    oleuw->verbose = level;
    return OLEUW_OK;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_set_save_unknown_streams
 Returns Type	: int
 	----Parameter List
	1. struct OLEUNWRAP_object *oleuw,
	2.  int level ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_set_save_unknown_streams( struct OLEUNWRAP_object *oleuw, int level )
{
    oleuw->save_unknown_streams = level;
    return OLEUW_OK;
}


/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_save_stream
 Returns Type	: int
 	----Parameter List
	1. char *fname,
	2.  char *stream,
	3.  size_t bytes ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_save_stream( struct OLEUNWRAP_object *oleuw, char *fname, char *decode_path, char *stream, size_t bytes )
{
    char *full_name;
    FILE *f;
    int result = 0;

    DUW LOGGER_log("%s:%d:OLEUNWRAP_save_stream:DEBUG: fname=%s, decodepath=%s, size=%ld"
                   , FL
                   , fname
                   , decode_path
                   , bytes
                  );

    full_name = PLD_dprintf("%s/%s", decode_path, fname );
    if (full_name == NULL)
    {
        LOGGER_log(_("%s:%d:OLEUNWRAP_save_stream:ERROR: Unable to create filename string from '%s' and '%s'"), FL, fname, decode_path);
        return -1;
    }

    wcfopen(f, full_name, "wb");
    if (f != NULL)
    {
        size_t write_count;

        write_count = fwrite( stream, 1, bytes, f );
        if (write_count != bytes)
        {
            LOGGER_log(_("%s:%d:OLEUNWRAP_save_stream:WARNING: Only wrote %d of %d bytes to file %s\n"), FL, write_count, bytes, full_name );
        }

        fclose(f);


    }
    else
    {
        LOGGER_log(_("%s:%d:OLEUNWRAP_save_stream:ERROR: Unable to open %s for writing (%s)\n"), FL, full_name, strerror(errno));
        result = -1;
    }

    if (full_name)
    {
        FREE(full_name);
    }

    DUW LOGGER_log("%s:%d:OLEUNWRAP_save_stream:DEBUG: Done saving '%s'", FL, fname);

    return result;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_sanitize_filename
 Returns Type	: int
 	----Parameter List
	1. char *fname ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_sanitize_filename( char *fname )
{
    while (*fname)
    {
        if ( !isalnum((int)*fname) && (*fname != '.') )
        {
            *fname = '_';
        }
        if ( (*fname < ' ') || (*fname > '~') )
        {
            *fname = '_';
        }
        fname++;
    }
    return 0;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_decode_attachment
 Returns Type	: int
 	----Parameter List
	1. char *stream ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_decode_attachment( struct OLEUNWRAP_object *oleuw, char *stream, size_t stream_size, char *decode_path )
{
    struct OLE10_header oh;
    char *sp = stream;
    char *data_start_point = stream;
    int result = OLEUW_OK;

    /* Get the data size*/
    oh.attach_size_1 = (size_t)get_4byte_value( (unsigned char *) sp );
    sp += 4;

    DUW LOGGER_log("%s:%d:OLEUNWRAP_decode_attachment:DEBUG: attachsize = %d, stream length = %d\n", FL, oh.attach_size_1, stream_size );

    oh.attach_start_offset = (stream_size - oh.attach_size_1);
    data_start_point = stream + oh.attach_start_offset;

    /*if (oh.attach_start_offset == 4)*/
    if (oh.attach_start_offset < 4)
    {
        /* If we only had the stream byte-length in our header*/
        /*		then we know we don't have a complex header.*/
        oh.attach_name = PLD_dprintf("unknown-%ld", oh.attach_size_1);
        oh.attach_size = oh.attach_size_1;
    }
    else
    {

        DUW LOGGER_log("%s:%d:OLEUNWRAP_decode_attachment:DEBUG: Decoding file information header", FL);
        /* Unknown memory segment*/
        memcpy( oh.data, sp, 2 );
        sp += 2;

        /* Full attachment string*/
        oh.attach_name = os_strdup( sp );
        sp = sp + strlen(oh.attach_name) + 1;

        /* Attachment full path*/
        oh.fname_1 = os_strdup( sp );
        sp += strlen(oh.fname_1) + 1;

        /* Unknown memory segment*/
        memcpy( oh.data2, sp, 8 );
        sp = sp + 8;

        /* Attachment full path*/
        oh.fname_2 = os_strdup( sp );
        sp += strlen(oh.fname_2) + 1;

        oh.attach_size = (size_t)get_4byte_value( (unsigned char*) sp );
        sp += 4;

        if (oh.attach_size > stream_size)
        {
            oh.attach_size = stream_size;
        }

        data_start_point = sp;
    }

    DUW LOGGER_log(_("%s:%d:OLEUNWRAP_decode_attachment:DEBUG: Attachment %s:%s:%s size = %d\n"), FL, oh.attach_name, oh.fname_1, oh.fname_2, oh.attach_size );


    /** 20050119:2053:PLD - Added to sanitize 8-bit filenames **/
    /** Sanitize the output filename **/
    OLEUNWRAP_sanitize_filename(oh.attach_name);
    OLEUNWRAP_sanitize_filename(oh.fname_1);
    OLEUNWRAP_sanitize_filename(oh.fname_2);

    result = OLEUNWRAP_save_stream( oleuw, oh.attach_name, decode_path, data_start_point, oh.attach_size );
    if (result == OLEUW_OK)
    {
        if (oleuw->debug > 0)
        {
            LOGGER_log(_("%s:%d:OLEUNWRAP_decode_attachment:DEBUG: Calling reporter for the filename"), FL);
        }
        if ((oleuw->verbose > 0) && (oleuw->filename_report_fn != NULL))
        {
            oleuw->filename_report_fn(oh.attach_name);
        }
        /* Do call back to reporting function*/
    }

    /* Clean up our previously allocated data*/
    if (oh.fname_1 != NULL)
    {
        FREE(oh.fname_1);
    }
    if (oh.attach_name != NULL)
    {
        FREE(oh.attach_name);
    }
    if (oh.fname_2 != NULL)
    {
        FREE(oh.fname_2);
    }

    return OLEUW_OK;
}

/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_decodestream
 Returns Type	: int
 	----Parameter List
	1. char *element_string,
	2.  char *stream ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_decodestream( struct OLEUNWRAP_object *oleuw, char *element_string, char *stream, size_t stream_size, char *decode_path )
{
    int result = OLEUW_OK;

    if (strstr(element_string, OLEUW_ELEMENT_10NATIVE_STRING) != NULL)
    {
        OLEUNWRAP_decode_attachment( oleuw, stream, stream_size, decode_path );

    }
    else
    {
        if (oleuw->debug)
        {
            LOGGER_log(_("Unable to decode stream with element string '%s'\n"), element_string);
        }
        result = OLEUW_STREAM_NOT_DECODED;
    }

    return result;
}


/*-----------------------------------------------------------------\
 Function Name	: OLEUNWRAP_set_filename_report_fn
 Returns Type	: int
 	----Parameter List
	1. struct OLEUNWRAP_object *oleuw,
	2.  int (*ptr_to_fn)(char *) ,
 	------------------
 Exit Codes	:
 Side Effects	:
--------------------------------------------------------------------
 Comments:

--------------------------------------------------------------------
 Changes:

\------------------------------------------------------------------*/
int OLEUNWRAP_set_filename_report_fn( struct OLEUNWRAP_object *oleuw, int (*ptr_to_fn)(char *) )
{

    oleuw->filename_report_fn = ptr_to_fn;

    return 0;
}

