
#define OLEUW_ELEMENT_10NATIVE 10
#define OLEUW_ELEMENT_10NATIVE_STRING "Ole10Native"

#define OLEUW_OK 0
#define OLEUW_STREAM_NOT_DECODED 100

struct OLEUNWRAP_object {
	int (*filename_report_fn)(char *);
	int debug;
	int verbose;
	int save_unknown_streams;
};


int OLEUNWRAP_init( struct OLEUNWRAP_object *oleuw );
int OLEUNWRAP_set_debug( struct OLEUNWRAP_object *oleuw, int level );
int OLEUNWRAP_set_verbose( struct OLEUNWRAP_object *oleuw, int level );
int OLEUNWRAP_set_save_unknown_streams( struct OLEUNWRAP_object *oleuw, int level );

int OLEUNWRAP_save_stream( struct OLEUNWRAP_object *oleuw, char *fname, char *decode_path, char *stream, size_t bytes );
int OLEUNWRAP_decode_attachment( struct OLEUNWRAP_object *oleuw, char *stream, size_t stream_size, char *decode_path );
int OLEUNWRAP_decodestream( struct OLEUNWRAP_object *oleuw, char *element_string, char *stream, size_t stream_size, char *decode_path );

int OLEUNWRAP_set_filename_report_fn( struct OLEUNWRAP_object *oleuw, int (*ptr_to_fn)(char *) );

