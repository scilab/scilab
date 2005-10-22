
/* Microsoft OLE2 stream parser.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logger.h"
#include "pldstr.h"
#include "ole.h"
#undef WITHMAIN

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


struct ripOLE_object {
	int debug;
	int verbose;
	int save_unknown_streams;

	char *inputfile;
	char *outputdir;
};


static char defaultdir[]=".";
static char version[]="0.1.4 - 27-November-2004 (C) PLDaniels http://www.pldaniels.com/ripole";
static char help[]="ripOLE -i <OLE2 file> [ -d <directory> ] [--save-unknown-streams] [--version|-V] [--verbose|-v] [--debug] [--help|-h]";

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
int set_defaults( struct ripOLE_object *role )
{
	role->outputdir = defaultdir;
	role->debug = 0;
	role->verbose = 0;
	role->save_unknown_streams = 0;
	role->inputfile = NULL;

	return 0;
}

/*-----------------------------------------------------------------\
 Function Name	: parse_parameters
 Returns Type	: int
 	----Parameter List
	1. struct ripOLE_object *role, 
	2.  int argc, 
	3.  char **argv , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/
int parse_parameters( struct ripOLE_object *role, int argc, char **argv )
{
	int i;
	int result = 0;

	role->outputdir = defaultdir;

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'i':
					i++;
					role->inputfile = strdup(argv[i]);
					break;

				case 'd':
					i++;
					role->outputdir = strdup(argv[i]);
					break;

				case 'v':
					role->verbose = 1;
					break;

				case 'V':
					fprintf (stdout, "%s\n", version);
					exit (1);
					break;
				case 'h':
					fprintf (stdout, "%s\n", help);
					exit (1);
					break;

					/*  if we get ANOTHER - symbol, then we have an extended flag */

				case '-':
						if (strncmp (&(argv[i][2]), "verbose", 7) == 0)
						{
							role->verbose=1;
							
						} else if (strncmp (&(argv[i][2]), "save-unknown-streams", 20) == 0) {
							role->save_unknown_streams = 1;
						
						} else if (strncmp (&(argv[i][2]), "debug", 5) == 0) {
							role->debug=1;
							
						} else if (strncmp (&(argv[i][2]), "version", 7) == 0) {
							fprintf (stdout, "%s\n", version);
							exit (1);
							
						} else if (strncmp(&(argv[i][2]),"help",4)==0) {
							fprintf(stdout,"%s\n",help);
							exit(1);

						} else {
							fprintf(stdout,"Cannot interpret option \"%s\"\n%s\n", argv[i], help);
							exit (1);
							break;
						}
					break;

					/*  else, just dump out the help message */

				default:
					fprintf(stdout,"Cannot interpret option \"%s\"\n%s\n", argv[i], help);
					exit (1);
					break;

			}			/* Switch argv[i][1] */

		}			/*  if argv[i][0] == -*/

	}				/*  for */

	return result;
}

/*-----------------------------------------------------------------\
 Function Name	: set_parameters
 Returns Type	: int
 	----Parameter List
	1. struct ripOLE_object *role, 
	2.  struct OLE_object *ole , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/
int set_parameters( struct ripOLE_object *role, struct OLE_object *ole )
{
	if(role->debug == 1)
	{
		OLE_set_debug(ole, OLE_DEBUG_NORMAL);
	} 

	if (role->verbose == 1)
	{
		OLE_set_verbose(ole, OLE_VERBOSE_NORMAL);
	}

	if (role->save_unknown_streams == 1)
	{
		OLE_set_save_unknown_streams(ole, 1);
	}

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
int ripOLE_report_filename_decoded(char *filename)
{
	LOGGER_log("Decoding filename=%s", filename);

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
	if (role->inputfile != NULL) FREE(role->inputfile);
	if (role->outputdir != NULL) FREE(role->outputdir);

	return 0;
}

#ifdef WITHMAIN
/*-----------------------------------------------------------------\
 Function Name	: main
 Returns Type	: int
 	----Parameter List
	1. int argc, 
	2.  char **argv , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/

int main( int argc, char **argv )
{
	struct ripOLE_object role;
	struct OLE_object *ole = NULL;
	int result = 0;

	if (argc == 1) { fprintf (stdout, "%s\n", help); exit(1); }

	ole = MALLOC(sizeof(struct OLE_object));
	if (ole == NULL)
	{
		LOGGER_log("ripOLE: Cannot allocate memory for OLE object");
		return 1;
	}

	LOGGER_set_output_mode(_LOGGER_STDOUT);

	OLE_init(ole);
	ROLE_init(&role);

	set_defaults( &role );
	parse_parameters(&role, argc, argv);
	set_parameters(&role, ole);

	OLE_set_filename_report_fn(ole, ripOLE_report_filename_decoded );

	result = OLE_decode_file( ole, role.inputfile, role.outputdir );
	OLE_decode_file_done(ole);

	if ((result != 0)&&(role.verbose)) LOGGER_log("ripOLE: decoding of %s resulted in error %d\n", role.inputfile, result );

	if (ole != NULL) FREE(ole);
	ROLE_done(&role);
	
	return result;
}
#endif
/*-----------------------------------------------------------------\
 Function Name	: ripole
 Returns Type	: int
 	----Parameter List
	1. char *inputfile
	2. char *outputdir
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments: This function has been added to the original distribution
           for Scilab. It uses a particular value (2)
           for ole->save_unknown_streams to ask only for excel stream
           if it exist the excel stream is saved under the fullpath 
           given by outfile. A slight modification has been done in 
           OLE_decode_file to handle this case see comments tagged Scilab.
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/

int ripole(char *inputfile, char *outputdir, int debug, int verbose)
{
  /*struct OLE_object *ole = NULL;*/
  struct OLE_object ole;
	int result = 0;

	/*ole = MALLOC(sizeof(struct OLE_object));
	if (ole == NULL)
	{
		LOGGER_log("ripOLE: Cannot allocate memory for OLE object");
		return 1;
	}*/

	LOGGER_set_output_mode(_LOGGER_STDOUT);

	OLE_init(&ole);
	if (debug ==1 ) OLE_set_debug(&ole, OLE_DEBUG_NORMAL);
	if (verbose == 1) OLE_set_verbose(&ole, OLE_VERBOSE_NORMAL);

	OLE_set_save_unknown_streams(&ole, 2); /* get only excel streams */

	OLE_set_filename_report_fn(&ole, ripOLE_report_filename_decoded );

	result = OLE_decode_file( &ole, inputfile, outputdir);
	OLE_decode_file_done(&ole);


	if ((result != 0)&&(verbose==1)) 
	  LOGGER_log("ripOLE: decoding of %s resulted in error %d\n", inputfile, result );

	/*if (ole != NULL) FREE(ole);*/
	return result;
}
