
/* Microsoft OLE2 stream parser.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logger.h"
#include "pldstr.h"
#include "ole.h"
#include "ripole.h"
#include "localization.h"

#undef WITHMAIN

#include "sci_malloc.h"

#define ROLE_VERSION "0.2.0"

static char defaultdir[] = ".";
/*static char version[] = "0.2.0 - 12-December-2005 (C) PLDaniels http:/*www.pldaniels.com/ripole";
static char help[] = "ripOLE -i <OLE2 file> [ -d <directory> ] [--save-unknown-streams] [--version|-V] [--verbose|-v] [--debug] [--help|-h]";
*/
/*-----------------------------------------------------------------\
  Function Name	: set_defaults
  Returns Type	: int
  ----Parameter List
  1. struct ripOLE_object *role ,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int ROLE_set_defaults(struct ripOLE_object *role)
{
    role->outputdir = defaultdir;
    role->debug = 0;
    role->verbose = 0;
    role->save_unknown_streams = 0;
    role->inputfile = NULL;

    return 0;
}

/*-----------------------------------------------------------------\
  Function Name	: ripOLE_report_filename_decoded
  Returns Type	: int
  ----Parameter List
  1. char *filename,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int ROLE_report_filename_decoded(char *filename)
{
    LOGGER_log(_("Decoding filename=%s"), filename);

    return 0;
}

/*-----------------------------------------------------------------\
  Function Name	: ROLE_init
  Returns Type	: int
  ----Parameter List
  1. struct ripOLE_object *role,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int ROLE_init(struct ripOLE_object *role)
{
    role->debug = 0;
    role->verbose = 0;
    role->save_unknown_streams = 0;

    role->inputfile = NULL;
    role->outputdir = NULL;

    return 0;
}

/*-----------------------------------------------------------------\
  Function Name	: ROLE_done
  Returns Type	: int
  ----Parameter List
  1. struct ripOLE_object *role,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int ROLE_done(struct ripOLE_object *role)
{
    if (role->inputfile != NULL)
    {
        FREE(role->inputfile);
    }
    if (role->outputdir != NULL)
    {
        FREE(role->outputdir);
    }

    return 0;
}

/*-----------------------------------------------------------------\
  Function Name	: ROLE_validate
  Returns Type	: int
  ----Parameter List
  1. struct ripOLE_object *role ,
  ------------------
  Exit Codes	:
  Side Effects	:
  --------------------------------------------------------------------
Comments:

--------------------------------------------------------------------
Changes:

\------------------------------------------------------------------*/
int ROLE_validate(struct ripOLE_object *role)
{
    int result = 0;

    if (role->inputfile == NULL)
    {
        fprintf(stderr, _("ripOLE requires an input file to decode.\n"));
        return -1;
    }

    return result;
}

int ripole(char *inputfile, char *outputdir, int debug, int verbose)
{
    /*struct OLE_object *ole = NULL; */
    struct OLE_object ole;
    int result = 0;

    /*ole = MALLOC(sizeof(struct OLE_object));
     * if (ole == NULL)
     * {
     * LOGGER_log(_("ripOLE: Cannot allocate memory for OLE object"));
     * return 1;
     * } */

    LOGGER_set_output_mode(_LOGGER_STDOUT);

    OLE_init(&ole);
    if (debug == 1)
    {
        OLE_set_debug(&ole, OLE_DEBUG_NORMAL);
    }
    if (verbose == 1)
    {
        OLE_set_verbose(&ole, OLE_VERBOSE_NORMAL);
    }
    /*ole->save_unknown_streams==2 added for Scilab to extract onlyspreadsheet streams
     * see  OLE_decode_stream in ole.c file */
    OLE_set_save_unknown_streams(&ole, 2);  /* get only spreadsheet streams */

    OLE_set_filename_report_fn(&ole, ROLE_report_filename_decoded);

    result = OLE_decode_file(&ole, inputfile, outputdir);
    OLE_decode_file_done(&ole);

    if ((result != 0) && (verbose == 1))
    {
        LOGGER_log(_("ripOLE: decoding of %s resulted in error %d.\n"), inputfile, result);
    }

    /*if (ole != NULL) FREE(ole); */
    return result;
}
