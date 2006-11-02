
#ifndef LIBOLE
#define LIBOLE

#define LIBOLE_VERSION "200512112226"

#define OLE_OK 0
#define OLEER_NO_INPUT_FILE 100
#define OLEER_BAD_INPUT_FILE 101
#define OLEER_NOT_OLE_FILE 102
#define OLEER_INSANE_OLE_FILE 103

#define OLEER_DECODE_NULL_OBJECT					10
#define OLEER_DECODE_NULL_FILENAME				11
#define OLEER_DECODE_NULL_PATH					12

#define OLEER_LOADFAT_BAD_BOUNDARY				20

#define OLEER_MINIFAT_READ_FAIL					30
#define OLEER_PROPERTIES_READ_FAIL				31
#define OLEER_MINISTREAM_READ_FAIL				32
#define OLEER_MINISTREAM_STREAM_READ_FAIL    33
#define OLEER_NORMALSTREAM_STREAM_READ_FAIL  34

#define OLEER_GET_BLOCK_SEEK						41
#define OLEER_GET_BLOCK_READ						42

#define OLEER_MEMORY_OVERFLOW						50

#define OLE_VERBOSE_NORMAL 1
#define OLE_VERBOSE_FATREAD 2
#define OLE_VERBOSE_DIRREAD 4
#define OLE_VERBOSE_STREAMREAD 8
#define OLE_VERBOSE_STREAMDECODE 16

#define OLE_VNORMAL(x) ((x) && OLE_VERBOSE_NORMAL == OLE_VERBOSE_NORMAL )

#define OLE_DEBUG_NORMAL 1
#define OLE_DEBUG_PEDANTIC 2

#define OLE_DNORMAL(x) ((x) && OLE_DEBUG_NORMAL == OLE_DEBUG_NORMAL)
#define OLE_DPEDANTIC(x) ((x) && OLE_DEBUG_PEDANTIC == OLE_DEBUG_PEDANTIC)


#define OLE_HEADER_FAT_SECTOR_COUNT_LIMIT 109
struct OLE_header {
		unsigned int minor_version;
		unsigned int dll_version;
		unsigned int byte_order;
		unsigned int sector_shift;
		unsigned int sector_size;
		unsigned int mini_sector_shift;
		unsigned int mini_sector_size;
		unsigned int fat_sector_count;
		unsigned int directory_stream_start_sector;
		unsigned int mini_cutoff_size;
		unsigned int mini_fat_start;
		unsigned int mini_fat_sector_count;
		unsigned int dif_start_sector;
		unsigned int dif_sector_count;
		unsigned int FAT[OLE_HEADER_FAT_SECTOR_COUNT_LIMIT];
};

#define OLE_DIRECTORY_ELEMENT_NAME_SIZE 64
#define OLE_DIRECTORY_CLASS_SIZE 16
#define OLE_DIRECTORY_TIMESTAMPS_SIZE 16
struct OLE_directory_entry {
	char element_name[OLE_DIRECTORY_ELEMENT_NAME_SIZE];
	int element_name_byte_count;

	char element_type;
	char element_colour;

	unsigned int left_child;
	unsigned int right_child;
	unsigned int root;

	unsigned char class[OLE_DIRECTORY_CLASS_SIZE];
	unsigned int userflags;
	unsigned char timestamps[OLE_DIRECTORY_TIMESTAMPS_SIZE];
	unsigned int start_sector;
	unsigned int stream_size;
};


	
#define OLE_HEADER_BLOCK_SIZE 512
struct OLE_object {

	int error;
	size_t file_size;
	int last_sector;
	size_t last_chain_size;

	FILE *f;
	unsigned char *FAT;
	unsigned char *FAT_limit; /** Added to prevent segment violations **/
	unsigned char *miniFAT;
	unsigned char *miniFAT_limit; /** Added to prevent segment violations **/
	unsigned char header_block[OLE_HEADER_BLOCK_SIZE];
	unsigned char *ministream;
	unsigned char *properties;

	struct OLE_header header;
	
  /* End user configurable parameters:*/
	int debug;
	int verbose;
	int quiet;
	int save_unknown_streams;
	
	int save_streams;
	int save_mini_streams;
	int save_normal_streams;

	int decode_streams;
	int decode_mini_streams;
	int decode_normal_streams;

	int (*filename_report_fn)(char *);

};



/* Prototypes*/

int OLE_version( void );

int OLE_init( struct OLE_object *ole );
int OLE_set_verbose( struct OLE_object *ole, int level );
int OLE_set_debug( struct OLE_object *ole, int level );
int OLE_set_quiet( struct OLE_object *ole, int level );
int OLE_set_save_unknown_streams( struct OLE_object  *ole, int level );

int OLE_get_block( struct OLE_object *ole, int block_index, unsigned char *block_buffer );
int OLE_get_miniblock( struct OLE_object *ole, int block_index, unsigned char *block_buffer );
int OLE_dbstosbs( char *raw_string, size_t char_count, char *clean_string, int clean_string_len );
int OLE_print_string( char *string, size_t char_count);
int OLE_print_sector( struct OLE_object *ole, char *sector, unsigned int bytes);
int OLE_get_header( struct OLE_object *ole );
int OLE_convert_header( struct OLE_object *ole );
int OLE_print_header( struct OLE_object *ole );
int OLE_convert_directory(struct OLE_object *ole, unsigned char *buf, struct OLE_directory_entry *dir );
int OLE_print_directory( struct OLE_object *ole, struct OLE_directory_entry *dir );
int OLE_load_FAT( struct OLE_object *ole );
int OLE_follow_chain( struct OLE_object *ole, int FAT_sector_start );
int OLE_follow_minichain( struct OLE_object *ole, int miniFAT_sector_start );
unsigned char *OLE_load_minichain( struct OLE_object *ole, int miniFAT_sector_start );
unsigned char *OLE_load_chain( struct OLE_object *ole, int FAT_sector_start );
int OLE_open_file( struct OLE_object *ole, char *fullpath );
int OLE_decode_file( struct OLE_object *ole, char *fname, char *decode_path );
int OLE_decode_file_done( struct OLE_object *ole );


/* Our callbacks.*/
int OLE_set_filename_report_fn( struct OLE_object *ole, int (*ptr_to_fn)(char *) );

#endif
