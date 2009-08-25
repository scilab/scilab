/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Simon LIPP
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MALLOC.h"
#include "BOOL.h"
#include "stack-c.h"
#include "sciprint.h"
#include "localization.h"
#include "doublecomplex.h"

#include <assert.h>

/*--------------------------------------------------------------------------*/
/* We can't have the_p - ref ~ 0, because this can lead to some bugs. For
 * example, see sci_comp.f:70 and getlin.f:84. The first one sets lin to offset
 * and excepts it to be != 2 (2 has a special meaning in the getlin function).
 * With a low offset, lin can be 2, thus leading the wrong branch being choosed
 * in getlin.
 * Why 1024 ? Well, it works and I like this number. And, for the superstitious
 * being, it's the size of a memory page... (well, kinda geeky superstition)
 */
#define SKIPSIZE 1024
/*--------------------------------------------------------------------------*/
static double *ref = NULL;
static int ssize = 0, gssize = 0, nextsize = 0;
/*--------------------------------------------------------------------------*/
extern void C2F(relocstack)(void *p, void *pi, void *ps, void *pc, void *pz,
                            int *n, int *ni, int *ns, int *nc, int *nz);
extern void C2F(adjustgstacksize)(int *mem, int *offset, int *l);
/*--------------------------------------------------------------------------*/
static void setref(double *p1, int allocsz)
{
	int ni = (allocsz * sizeof(double))/sizeof(int);
	int ns = (allocsz * sizeof(double))/sizeof(float);
	int nc = (allocsz * sizeof(double))/sizeof(char);
	int nz = (allocsz * sizeof(double))/sizeof(doublecomplex);
	
	C2F(stack).Stk = ref = p1;
	C2F(relocstack)(ref, ref, ref, ref, ref, &allocsz, &ni, &ns, &nc, &nz);
}

static void movegstack()
{
	int l, gptr;
	
	if(gssize) {
		/* Stack is like this:
		 * (internal) (reserved) (top) (unused) (bot) (gtop) (unused) (gbot)
		 * Since we have moved bot, we have to move gtop & gbot (global stack)
		 */
		gptr = ssize + 2 + SKIPSIZE;
		l = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1];
		C2F(adjustgstacksize)(&gssize, &gptr, &l);
	}
}

int scimem64(int *ptr, int newsize, BOOL isglobal)
{
	double *p1;
	int othersize, oldsize, allocsz;
	
	if(newsize++ < 0) /* Add 1 for alignment problems */
		return 0;
	
	if(isglobal == TRUE) {
		othersize = ssize;
		oldsize = gssize;
	}
	else {
		othersize = gssize;
		oldsize = ssize;
	}
	
	/* The real work for reducing stack size is done after the relocation,
	 * when freemem/freegmem is called. Right now, just return
	 */
	if(newsize < oldsize) {
		nextsize = newsize;
		if(isglobal == TRUE)
			*ptr = ssize + 2 + SKIPSIZE;
		else
			*ptr = 2 + SKIPSIZE;
		return 0;
	}

	/* Add 2 for internal stack */
	allocsz = newsize + othersize + 2 + SKIPSIZE;
	p1 = (double*)REALLOC(ref, sizeof(double) * allocsz);
	
	if(p1 != NULL) {
		setref(p1, allocsz);
		
		if(isglobal == TRUE) {
			*ptr = ssize + 2 + SKIPSIZE;
			gssize = newsize;
		}
		else {
			*ptr = 2 + SKIPSIZE;
			ssize = newsize;
			movegstack();
		}
	}
	else {
		if (ref == NULL) {
			sciprint(_("No space to allocate Scilab stack.\n"));
			exit(1); 
		}
		*ptr=0;
	}

	return 0;
}

void freemem64(BOOL isglobal) {
	double *p1;
	int allocsz;
	
	if(nextsize == 0)
		return;
	
	/* We have to reduce stack size */
	if(isglobal == TRUE) {
		gssize = nextsize;
	}
	else {
		ssize = nextsize;
		movegstack();
	}
	
	/* Add 2 for internal stack */
	allocsz = ssize + gssize + 2 + SKIPSIZE;
	p1 = (double*)REALLOC(ref, sizeof(double) * allocsz);
	
	if(p1 != NULL)
		setref(p1, allocsz);
	
	nextsize = 0;
}
