/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "CdfBase.h"

#include "api_scilab.h"
#include "stack-c.h" /* TODO: remove as soon as Rhs is linked in api_scilab.h */
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "sci_string_matrix.h"
#include <string.h>

int mod(int a, int b);
int rotate(int i, int step, int length);
char *cdf_options(struct cdf_descriptor const * const desc);
void cdf_error(char const * const fname, int status, double bound);
int CdfBase(char const * const fname, int* _piKey, int inarg, int oarg, int shift, int which,
	    int (*fun)(int *, ...));

int
cdf_generic(char *fname, int* _piKey, struct cdf_descriptor *cdf)
{
  struct cdf_item const * it;
  char *option;

  CheckRhs(cdf->minrhs, cdf->maxrhs);
  CheckLhs(cdf->minlhs, cdf->maxlhs);
  option = create_string(_piKey, 1);
  for (it = cdf->items; it != cdf->end_item; ++it)
    if (strcmp(option, it->option) == 0) {
      /* "which" argument (5th) inferred from position in item list */
      CdfBase(fname, _piKey, it->inarg, it->oarg, it->shift, it - cdf->items + 1, cdf->fun);
      break;
    }
  destroy_string(option);
  if (it == cdf->end_item) { /* no target found */
    char *optlist;
    optlist = cdf_options(cdf);
    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, optlist);
    FREE(optlist);
    return 1;
  }
  return 0;
}

/**
 * Remainder r > 0 of a div b.
 */
int
mod(int a, int b)
{
  int m = a % b;
  return (m >= 0) ? m : m + b;
}

/**
 * Get index of T's ith element after T has been rotated step elts to the left.
 * Formally, with r denoting table's rotation to the left,
 *   (r(T, step))[rotate(i, step, length)] = T[i]
 * e.g.  T = {0, 1, 2, 3, 4, 5, 6, 7, 8}
 * r(T, 3) = {3, 4, 5, 6, 7, 8, 0, 1, 2}
 * hence rotate(7,3,9) = 4
 */
int
rotate(int i, int step, int length)
{
  return mod((i - step), length);
}

/**
 * Collect options from a cdf_descriptor in a user readable string.
 * Allocates space for its return value. To be freed afterwards.
 */
char *
cdf_options(struct cdf_descriptor const * const cdf)
{
  char *ret;
  size_t len = 0;
  char const * const spc = ", ";
  struct cdf_item const * it;

  for (it = cdf->items; it != cdf->end_item; ++it)
    len += strlen(spc) + strlen(it->option);
  ret = (char *) MALLOC(len * sizeof(char));
  ret[0] = '\0';
  for (it = cdf->items; it != cdf->end_item; ++it) {
    strcat(ret, it->option);
    if (it + 1 != cdf->end_item)
      strcat(ret, spc);
  }
  return ret;
}

/**
 * Interpret and signal DCDFLIB's error return values.
 */
void
cdf_error(char const * const fname, int status, double bound)
{
  switch (status) {
  case 1:
    Scierror(999, _("%s: Answer appears to be lower than lowest search bound %f\n"), fname, bound);
    break;
  case 2:
    Scierror(999, _("%s: Answer appears to be higher than greatest search bound %f\n"), fname, bound);
    break;
  case 3:
    Scierror(999, "%s: P + Q ≠ 1\n", fname);
    break;
  case 4:
    if (strcmp(fname, "cdfbet") == 0)
      Scierror(999, "%s: X + Y ≠ 1", fname);
    else if (strcmp(fname, "cdfbin") == 0 ||
	     strcmp(fname, "cdfnbn") == 0)
      Scierror(999, "%s: Pr + Ompr ≠ 1\n", fname);
      else if (strcmp(fname, "cdfnor") == 0)
      Scierror(999, _("%s: Std must not be zero\n"), fname);
    break;
  case 10:
    if (strcmp(fname, "cdfchi") == 0)
      Scierror(999, _("%s: cumgam returned an error\n"), fname);
    else if (strcmp(fname, "cdfchi") == 0)
      Scierror(999, _("%s: gamma or inverse gamma routine failed\n"), fname);
    break;
  default:
    Scierror(999, _("%s: Argument #%d out of range. Bound exceeded: %f.\n"),
	     fname, - status, bound);
  }
}

/**
 * Older generic interface to DCDLIB's cdf functions.
 * @param fname scilab caller's function name
 * @param inarg number of inputs to the scilab call
 * @param oarg number of outputs begged by the scilab call
 * @param shift tells how much arglist has to be shifted in DCDFLIB funcall
 * @param which of the arguments is to be deduced from the others
 * @param fun actual computation function
 * @return error code
 */
int
CdfBase(char const * const fname, int* _piKey, int inarg, int oarg, int shift, int which,
	int (*fun)(int*, ...))
{
  #define MAXARG 6
  double *data[MAXARG];
  int rows[MAXARG], cols[MAXARG];
  #undef MAXARG

  double bound;
  int errlevel;
  int i;
  int *p;

  if ( Rhs != inarg + 1 ) {
    Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, inarg + 1);
    return 1;
  }
  for (i = 0; i < inarg; ++i) {
    getVarAddressFromPosition(_piKey, i + 2, &p);
    getMatrixOfDouble(_piKey, p, &rows[i], &cols[i], &data[i]);
  }
  for (i = 1; i < inarg ; ++i)
    if (rows[i] != rows[i-1] || cols[i] != cols[i-1]) {
      Scierror(999, _("%s: Incompatible input arguments #%d and #%d': Same sizes expected.\n"), fname, i + 1, i + 2);
      return 1;
    }
  for (i = 0; i < oarg; ++i)
    allocMatrixOfDouble(_piKey, Rhs + i + 1, rows[0], cols[0], &data[i + inarg]);
#define callpos(i) rotate(i, shift, inarg + oarg)
  for (i = 0; i < rows[0] * cols[0]; ++i) {
    switch (inarg + oarg) {
    case 4: /* cdfchi, cdfpoi, cdft */
      (*fun)(&which,
	     &(data[callpos(0)][i]),
	     &(data[callpos(1)][i]),
	     &(data[callpos(2)][i]),
	     &(data[callpos(3)][i]),
	     &errlevel, &bound);
      break;
    case 5: /* cdfchn, cdff, cdfgam, cdfnor */
      (*fun)(&which,
	     &(data[callpos(0)][i]),
	     &(data[callpos(1)][i]),
	     &(data[callpos(2)][i]),
	     &(data[callpos(3)][i]),
	     &(data[callpos(4)][i]),
	     &errlevel, &bound);
      break;
    case 6: /* cdfbet, cdfbin, cdffnc, cdfnbn, */
      (*fun)(&which,
	     &(data[callpos(0)][i]),
	     &(data[callpos(1)][i]),
	     &(data[callpos(2)][i]),
	     &(data[callpos(3)][i]),
	     &(data[callpos(4)][i]),
	     &(data[callpos(5)][i]),
	     &errlevel, &bound);
      break;
    }
    if (errlevel != 0) {
      cdf_error(fname, errlevel, bound);
      return 1;
    }
  }
#undef callpos
  for (i = 0; i < oarg; ++i)
    LhsVar(i + 1) = Rhs + i + 1;
  PutLhsVar();
  return 0;
}
