
#define MAX_LINE_LEN 512	/* maximum number of chars allowed on line */
#define NO_CARET (-1)

char * readline_win (char *prompt,int interrupt);
int read_line (char *prompt, int interrupt);