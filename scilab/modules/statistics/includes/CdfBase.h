/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __CDFBASE_H__
#define __CDFBASE_H__

#include "dynlib_statistics.h"
/* see cdff.f line 97 */
#define INFINITY_FOR_CDF 1.0e300
#define ZERO_FOR_CDF 1.0e-300

/**
 * Describe a way of calling a particular cdf function.
 * @see cdf_descriptor
 */
struct cdf_item
{
    const char *option;
    const int inarg; /* number of inputs taken */
    const int oarg;  /* number of outputs expected */
    const int shift; /* how much arglist has to be shifted in DCDFLIB funcall @see CdfBase */
};

/**
 * Type of a cdf generic function.
 */
typedef int (*const cdf_fptr)(int *, ...);

/**
 * Holds all information required to manipulate DCDFLIB's cdf functions.
 * @warning "items" element order is used in cdf_generic to infer "which" value
 * @note use mkcdf macro to instanciate cdf_descriptors
 */
struct cdf_descriptor
{
    const int minrhs;
    const int maxrhs;
    const int minlhs;
    const int maxlhs;
    cdf_fptr fun;				 /* actual DCDFLIB's function */
    const struct cdf_item *items;	 /* ways of calling this function */
    const struct cdf_item *end_item; /* last item, 1 passed */
};

/**
 * Generic interface to DCDFLIB's cdf functions.
 * @param[in] fname scilab caller function name
 * @param[in] cdf's description, @see struct cdf_descriptor
 */
STATISTICS_IMPEXP int cdf_generic(char *fname, void* pvApiCtx, struct cdf_descriptor *cdf);


/**
 * Number of elements of an elttype-typed array
 */
#define	nelem(array, elttype) (sizeof(array) / sizeof(elttype))

/**
 * Address of end (ie last past one) element of a cdf_item array
 */
#define	end_item(array) (array + nelem(array, struct cdf_item))

/**
 * Format a cdf_descriptor
 * @param ffname fortran function name (with no underscore)
 * @param minrhs, maxrhs, minlhs, maxlhs rhs (resp. lhs) minimum (resp. max)
 * @param itvar variable holing the cdf_item array describing cdf options
 * @warning itvar must be an array so that sizeof will work on it significantly
 */
#define	mkcdf(ffname, minrhs, maxrhs, minlhs, maxlhs, itvar)		\
  {minrhs, maxrhs, minlhs, maxlhs,					\
      (cdf_fptr)C2F(ffname), itvar, end_item(itvar)}

/**
 * @remark There is a need for cleverer macros in this module. Indeed,
 * every sci_cdf file follows the same pattern. However, I won't go
 * any further on this way since:
 *
 * 1. I frown upon encapsulating returns in macros (beacause it cheats
 * on standard C interpreting). And I am reluctant to declare unscoped
 * variables in macros to be used out of the macro's body (because of
 * side effects and code obfuscating). But C89 doesn't allow
 * expressions like (int t, &t). Hence, I see no (clean) way out.
 *
 * 2. No comma inside a macro arguments is allowed. i.e. cpp reads 3
 * arguments in such a macro call as mac({1, 2, 3}). On the other
 * hand, I won't write bracing macros (two complementary macros
 * starting and ending a block) because it's a pain to write neatly,
 * it has unexpected side effets w.r.t. standard C interpretation.
 */

#endif /* __CDFBASE_H__ */
