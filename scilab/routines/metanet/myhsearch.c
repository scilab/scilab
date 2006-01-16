/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#include "mysearch.h"
/*-----------------------------------------------------------------------------------*/
typedef struct { 
    int   used;
    ENTRY entry;
} _ENTRY;
/*-----------------------------------------------------------------------------------*/
static _ENTRY   * htable = NULL;
static unsigned   hsize;
static unsigned   filled;
/*-----------------------------------------------------------------------------------*/
/* 
 * For the used double hash method the table size has to be a prime. To
 * correct the user given table size we need a prime test.  This trivial
 * algorithm is adequate because
 * a)  the code is (most probably) only called once per program run and
 * b)  the number is small because the table must fit in the core
 */
static int isprime(unsigned number)
{
    /* no even number will be passed */
    unsigned div = 3;

    while (div*div < number && number%div != 0)
        div += 2;

    return number%div != 0;
}
/*-----------------------------------------------------------------------------------*/
/*
 * Before using the hash table we must allocate memory for it.
 * Test for an existing table are done. We allocate one element
 * more as the found prime number says. This is done for more effective
 * indexing as explained in the comment for the hsearch function.
 * The contents of the table is zeroed, especially the field used 
 * becomes zero.
 */
int myhcreate(unsigned nel)
{
    /* There is still another table active. Return with error. */
    if (htable != NULL)
	return 0;

    /* Change nel to the first prime number not smaller as nel. */
    nel |= 1;      /* make odd */
    while (!isprime(nel)) nel += 2;

    hsize  = nel;
    filled = 0;

    /* allocate memory and zero out */
    if ((htable = (_ENTRY *)CALLOC(hsize+1, sizeof(_ENTRY))) == NULL)
	return 0;

    /* everything went alright */
    return 1;
}
/*-----------------------------------------------------------------------------------*/
/*
 * After using the hash table it has to be destroyed. The used memory can
 * be freed and the local static variable can be marked as not used.
 */
void myhdestroy()
{
    /* free used memory */
    FREE(htable);

    /* the sign for an existing table is an value != NULL in htable */ 
    htable = NULL;
}
/*-----------------------------------------------------------------------------------*/
/*
 * This is the search function. It uses double hashing with open adressing.
 * The argument item.key has to be a pointer to an zero terminated, most
 * probably strings of chars. The function for generating a number of the
 * strings is simple but fast. It can be replaced by a more complex function
 * like ajw (see [Aho,Sethi,Ullman]) if the needs are shown.
 *
 * We use an trick to speed up the lookup. The table is created by hcreate
 * with one more element available. This enables us to use the index zero
 * special. This index will never be used because we store the first hash
 * index in the field used where zero means not used. Every other value
 * means used. The used field can be used as a first fast comparison for
 * equality of the stored and the parameter value. This helps to prevent
 * unnecessary expensive calls of strcmp.
 */

ENTRY* myhsearch(ENTRY item,SCIACTION action)
{
    register unsigned hval;
    register unsigned hval2;
    register unsigned count;
    register unsigned len = strlen(item.key);
    register unsigned idx;

    /*
     * If table is full and another entry should be entered return with 
     * error.
     */
    if (action == SCIENTER && filled == hsize) 
        return NULL;
      

    /* Compute an value for the given string. Perhaps use a better method. */
    hval  = len;
    count = len;
    while (count-- > 0) {
        hval <<= 4;
	hval += item.key[count];
    }

    /* First hash function: simply take the modul but prevent zero. */
    hval %= hsize;
    if (hval == 0) hval++;

    /* The first index tried. */
    idx = hval;

    if (htable[idx].used) {

        /* Further action might be required according to the action value. */

        if (htable[idx].used == hval &&
            strcmp(item.key, htable[idx].entry.key) == 0) {

            if (action == SCIENTER) 
	        htable[idx].entry.data = item.data;

	    return &htable[idx].entry;
        }

	/* Second hash function, as suggested in [Knuth] */
        hval2 = 1 + hval % (hsize-2);
	
        do {
	    /* 
	     * Because hsize is prime this guarantees to step through all
             * available indeces.
	     */
            if (idx <= hval2)
	        idx = hsize+idx-hval2;
	    else
	        idx -= hval2;

            /* If entry is found use it. */
            if (htable[idx].used == hval &&
                strcmp(item.key, htable[idx].entry.key) == 0) {

                if (action == SCIENTER) 
	            htable[idx].entry.data = item.data;

	        return &htable[idx].entry;
            }


	} while (htable[idx].used);
    }

    /* An empty bucket has been found. */
    if (action == SCIENTER) {
        htable[idx].used  = hval;
        htable[idx].entry = item;

	filled++;

        return &htable[idx].entry;
    } else
        return NULL;
}
/*-----------------------------------------------------------------------------------*/
