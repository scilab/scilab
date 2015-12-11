
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
