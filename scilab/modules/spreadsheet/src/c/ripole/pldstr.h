#ifndef __PLDSTR__
#define __PLDSTR__

#ifndef FL
#define FL __FILE__,__LINE__
#endif

struct PLD_strtok
{
    char *start;
    char delimeter;
};

struct PLD_strreplace
{
    char *source;
    char *searchfor;
    char *replacewith;

    char *preexist;
    char *postexist;

    int replacenumber;

    int insensitive;
};

char *PLD_strstr(char *haystack, char *needle, int insensitive);
char *PLD_strncpy( char *dst, const char *src, size_t len );
char *PLD_strncat( char *dst, const char *src, size_t len );
char *PLD_strncate( char *dst, const char *src, size_t len, char *endpoint );
char *PLD_strtok( struct PLD_strtok *st, char *line, char *delimeters );
int PLD_strncasecmp( char *s1, char *s2, int n );
int PLD_strlower( unsigned char *convertme );

char *PLD_strreplace_general( struct PLD_strreplace *replace_details );
char *PLD_strreplace( char **source, char *searchfor, char *replacewith, int replacenumber );
char *PLD_dprintf(const char *fmt, ...);

#endif
