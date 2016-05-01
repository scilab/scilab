
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __RIPOLE_H__
#define __RIPOLE_H__

struct ripOLE_object
{
    int debug;
    int verbose;
    int save_unknown_streams;

    char *inputfile;
    char *outputdir;

};

int ROLE_set_defaults( struct ripOLE_object *role );
int ROLE_parse_parameters( struct ripOLE_object *role, int argc, char **argv );
int ROLE_set_parameters( struct ripOLE_object *role, struct OLE_object *ole );
int ROLE_report_filename_decoded(char *filename);
int ROLE_init(struct ripOLE_object *role);
int ROLE_done(struct ripOLE_object *role);
int ROLE_validate(struct ripOLE_object *role );
int ripole(char *inputfile, char *outputdir, int debug, int verbose);

#endif /* #define __RIPOLE_H__ */
