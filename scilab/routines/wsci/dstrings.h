
/*
 * The structure defined below is used to hold dynamic strings.  The only
 * field that clients should use is the string field, and they should
 * never modify it.
 */
#include <stdlib.h>
#include <string.h>

#define CONST const

#define SCI_DSTRING_STATIC_SIZE 200
typedef struct Sci_DString {
    char *string;		/* Points to beginning of string:  either
				 * staticSpace below or a malloced array. */
    int length;			/* Number of non-NULL characters in the
				 * string. */
    int spaceAvl;		/* Total number of bytes available for the
				 * string and its terminating NULL char. */
    char staticSpace[SCI_DSTRING_STATIC_SIZE];
				/* Space to use in common case where string
				 * is small. */
} Sci_DString;

#define Sci_DStringLength(dsPtr) ((dsPtr)->length)
#define Sci_DStringValue(dsPtr) ((dsPtr)->string)
#define Sci_DStringTrunc Sci_DStringSetLength

void Sci_DStringInit( Sci_DString *dsPtr) ;
char *Sci_DStringAppend(Sci_DString *dsPtr, const char *string,int length);
void Sci_DStringSetLength(  Sci_DString *dsPtr, int length) ;
void Sci_DStringFree( Sci_DString *dsPtr);

#define SCI_OK 1
#define SCI_ERROR 0
