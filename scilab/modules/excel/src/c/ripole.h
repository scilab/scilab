/*------------------------------------------------------------------------*/
/* Copyright INRIA 2006                                                   */
/* Authors : Sylvestre ledru                                              */
/*------------------------------------------------------------------------*/
#ifndef __RIPOLE_H__
#define __RIPOLE_H__

struct ripOLE_object {
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
