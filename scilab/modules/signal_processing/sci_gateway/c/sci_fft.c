/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>

#include "gw_signal.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

/****************************************************************/
extern void C2F(fft842)(int *inverse, int *signal_length,
			double *signal_real, double *signal_imaginary,
			int *error);
extern void C2F(dfft2)(double *signal_real, double *signal_imaginary,
		       int *nseg, int *n, int *nspn,
		       int *inverse, int *error,
		       int *buffer_data, int *buffer_size);
/****************************************************************/
int POW2_15 = 32768;

int my_log2(int n);
int my_pow2(int n);
int is_pow2(int n);
int maxfactor(int n);
int fft_1dim(double *signal_real, double *signal_imaginary, int signal_length,
	     int inverse, int *buffer_data, int buffer_size);
int fft_2dim(double *signal_real, double *signal_imaginary,
	     int signal_rows, int signal_cols,
	     int inverse, int *buffer_data, int buffer_size);
int fft_ndim(double *signal_real, double *signal_imaginary, int signal_length,
	     int dimensions_length, int dimension_stride,
	     int inverse, int *buffer_data, int buffer_size);

int sci_fft(char *fname, unsigned long fname_len)
{
  double
    *signal_real, *signal_imaginary, /* input signal */
    *frequencies_real, *frequencies_imaginary, /* freq domain output */
    *argument;
  int
    inverse = -1, /* -1 for forward transform, 1 for inverse */
    /* [4213] should likely hold an array */
    dimensions_length = 0, /* length of each dimension */
    /* [4213] should likely hold an array */
    /* assert: dimension_stride(1) = product(dimensions_length(j),j=1..i-1) */
    dimension_stride = 0, /* stride for one step along each dimension */
    dimension_count = 0; /* number of dimensions */
  int signal_rows = 0, signal_cols = 0, rows = 0, cols = 0;

  int *buffer_data = NULL;
  int signal_length = 0, error = 0, buffer_size = 0;
  int one = 1;

  CheckRhs(1,4); /* but RHS=3 invalid too: checked afterwars */
  CheckLhs(1,1);

  /* collecting and checking arguments */
  switch (Rhs) {   /* no breaks to collect all arguments */
  case 4:
    GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
    dimension_stride = (int)argument[0];
    GetRhsVarMatrixDouble(3, &rows, &cols, &argument);
    dimensions_length = (int)argument[0];
    dimension_count = 3; /* any value > 2 (used as a flag) */
  case 2:
    GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
    inverse = argument[0];
    if (rows != 1 || cols != 1) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
      return 1;
    }
    if (inverse != 1 && inverse != -1) {
      Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 2, "-1 1");
      return 1;
    }
  case 1:
    GetVarDimension(1, &signal_rows, &signal_cols);
    dimension_count = Max(dimension_count,
			  (signal_rows == 1 || signal_cols == 1) ? 1 : 2);
    signal_length = signal_rows * signal_cols;    
    if (signal_length <= 1) {
      Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 1);
      return 1;
    }
    iAllocComplexMatrixOfDouble(Rhs + 1, signal_rows, signal_cols,
				&frequencies_real, &frequencies_imaginary);
    if (iIsComplex(1)) {
      GetRhsVarMatrixComplex(1, &signal_rows, &signal_cols,
			     &signal_real, &signal_imaginary);
      C2F(dcopy)(&signal_length, signal_real,
		 &one, frequencies_real, &one);
      C2F(dcopy)(&signal_length, signal_imaginary,
		 &one, frequencies_imaginary, &one);
    }
    else {
      double zero = 0L;
      GetRhsVarMatrixDouble(1, &signal_rows, &signal_cols, &signal_real);
      C2F(dcopy)(&signal_length, signal_real,
		 &one, frequencies_real, &one);
      C2F(dset)(&signal_length, &zero, frequencies_imaginary, &one);
    }
    break;
  default:
    Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 4);
    return 1;
  }

  /* upper bound on the workspace required by dfft2 */
  buffer_size = 8 * maxfactor(signal_length) + 24;
  buffer_data = (int *)CALLOC(buffer_size, sizeof(int));
  if (buffer_data == NULL) {
    Scierror(999, _("%s : Memory allocation error.\n"), fname);
    return 1;
  }

  /* how to compute depends on the dimension */
  switch (dimension_count) {
  case 1:
    error = fft_1dim(frequencies_real, frequencies_imaginary,
		     signal_length, inverse, buffer_data, buffer_size);
    break;
  case 2:
    error = fft_2dim(frequencies_real, frequencies_imaginary,
		     signal_rows, signal_cols, inverse,
		     buffer_data, buffer_size);
    if (error == 1) {
      Scierror(999, _("%s : Memory allocation error.\n"), fname);
    }
    break;
  default:
    error = fft_ndim(frequencies_real, frequencies_imaginary,
		     signal_length, dimensions_length, dimension_stride,
		     inverse, buffer_data, buffer_size);
    break;
  }

  /* preparing the return value */
  LhsVar(1) = Rhs + 1;
  PutLhsVar();
  free(buffer_data);
  buffer_data = NULL;
  return 0;
}
/****************************************************************/
int fft_1dim(double *signal_real, double *signal_imaginary,
	     int signal_length, int inverse,
	     int *buffer_data, int buffer_size) {
  int error = 0, one = 1;
  if (is_pow2(signal_length) && signal_length < POW2_15) {
    C2F(fft842)(&inverse, &signal_length,
		signal_real, signal_imaginary, &error);
  }
  else {
    C2F(dfft2)(signal_real, signal_imaginary,
	       &one, &signal_length, &one,
	       &inverse, &error,
	       buffer_data, &buffer_size);
  }
  return error;
}

int fft_2dim(double *signal_real, double *signal_imaginary,
		    int signal_rows, int signal_cols,
		    int inverse,
		    int *buffer_data, int buffer_size) {
  int error = 0, i, one = 1;
  if (is_pow2(signal_rows) && signal_rows < POW2_15) {
    for (i = 0 ; i < signal_cols ; i++) {
      C2F(fft842)(&inverse, &signal_rows,
		  &(signal_real[signal_rows * i]),
		  &(signal_imaginary[signal_rows * i]), &error);
    }
  }
  else {
    C2F(dfft2)(signal_real, signal_imaginary,
	       &signal_cols, &signal_rows, &one,
	       &inverse, &error, buffer_data, &buffer_size);
  }
  if (is_pow2(signal_cols) && signal_cols < POW2_15) {
    double *temp_real, *temp_imaginary;
    temp_real = (double *)MALLOC(signal_cols * sizeof(double));
    temp_imaginary = (double *)MALLOC(signal_cols * sizeof(double));
    if (temp_real == NULL || temp_imaginary == NULL) {
      return 1;
    }
    for (i = 0 ; i < signal_rows ; i++) {
      C2F(dcopy)(&signal_cols, &(signal_real[i]),
		 &signal_rows, temp_real, &one);
      C2F(dcopy)(&signal_cols, &(signal_imaginary[i]),
		 &signal_rows, temp_imaginary, &one);
      C2F(fft842)(&inverse, &signal_cols,
		  temp_real, temp_imaginary, &error);
      C2F(dcopy)(&signal_cols, temp_real, &one,
		 &(signal_real[i]), &signal_rows);
      C2F(dcopy)(&signal_cols, temp_imaginary, &one,
		 &(signal_imaginary[i]), &signal_rows);
    }
    free(temp_imaginary);
    temp_imaginary = NULL;
    free(temp_real);
    temp_real = NULL;
  }
  else { /* erroneous implementation suspected */
    C2F(dfft2)(signal_real, signal_imaginary,
	       &one, &signal_cols, &signal_rows,
	       &inverse, &error, buffer_data, &buffer_size);
  }
  return error;
}

int fft_ndim(double *signal_real, double *signal_imaginary,
	     int signal_length, int dimensions_length, int dimension_stride,
	     int inverse, int *buffer_data, int buffer_size) {
  /* translated litterally from Fortran... but... wtf ?! */
  int error = 0;
  int nseg = signal_length / dimensions_length / dimension_stride;
  C2F(dfft2)(signal_real, signal_imaginary, &nseg,
	     &dimensions_length, &dimension_stride, &inverse, &error,
	     buffer_data, &buffer_size);
  return error;
}

/* maximum factor of the prime decomposition of n
   - extracted and translated from dfftbi.f
   - pityfully needed to allocate the appropriate mem
   - translated litterally to make sure the result is the same
   TODO: remove once allocation moved to algorithm
 */
int maxfactor(int n) {
  int nfac[15];
  int m = 0, j = 0, jj = 0, k = 0, kt = 0, max = 0;

  for (k = abs(n) ; k % 16 == 0 ; k /= 16) {
    m++;
    nfac[m-1] = 4;
  }
  for (j = 3, jj = 9 ; jj <= k ; j += 2, jj = j*j) {
    if (k % jj != 0) continue;
    m++;
    nfac[m-1] = j;
    k /= jj;
  }
  if (k <= 4) {
    kt = m;
    nfac[m] = k;
    if (k != 1)
      m++;
  }
  else {
    if (k % 4 == 0) {
      m++;
      nfac[m-1] = 2;
      k /= 4;
    }
    kt = m;
    for (j = 2 ; j <= k ; j = ((j + 1) / 2) * 2 + 1) {
      if (k % j != 0)
	continue;
      m++;
      nfac[m-1] = j;
      k /= j;
    }
  }
  if (kt != 0) {
    for (j = kt ; j > 0 ; j--) {
      m++;
      nfac[m-1] = nfac[j-1];
    }
  }
  /* get nfac maximum */
  for (j = 0, max = nfac[0]; j < m ; j++) {
    max = (nfac[j] > max) ? nfac[j] : max;
  }
  return max;
}

/* because log2 is only C99... */
int my_log2(int n) {
  return (int)(log(n)/log(2));
}

int my_pow2(int n) {
  return 1 << n;
}

int is_pow2(int n) {
  return my_pow2(my_log2(n)) == n;
}
