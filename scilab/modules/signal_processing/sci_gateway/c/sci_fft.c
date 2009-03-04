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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gw_signal.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"

// switch to either c or fortran gateway...
int sci_fft(char *fname,unsigned long fname_len)
{
  return sci_fftc(fname, fname_len);
}

/*--------------------------------------------------------------------------*/
extern int C2F(scifft)(char *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_fftf(char *fname,unsigned long fname_len)
{
	C2F(scifft)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
/****************************************************************/
/* maximum factor of the prime decomposition of n (as bundled into the dfft2 algorithm) */
int maxfactor(int n);

/* is n a power of 2 ? */
int is_pow2(int n) {
  return (int)pow(2,(int)log2((double)n)) == n;
}

void print_array(double *array, int elements_nb) {
  int i;
  for (i = 0 ; i < elements_nb ; i++) {
    printf(" %f ", array[i]);
  }
}

int sci_fftc(char *fname, unsigned long fname_len)
{
  double
    *signal_real, *signal_imaginary, /* input signal */
    *frequencies_real, *frequencies_imaginary, /* frequency domain output */
    *argument;
  int
    inverse = -1, /* -1 for forward transform, 1 for inverse */
    dimensions_depth = 0, /* depth of each dimension */
    increment = 0, /* should hold the sequence (indexed by i) product(dimensions_depth(j),j=1..i-1)*/
/*     *dimensions_depth = NULL, /\* depth of each dimension *\/ */
/*     *increment = NULL, /\* should hold the sequence (indexed by i) product(dimensions_depth(j),j=1..i-1)*\/ */
    dimension_count = 0; /* number of dimensions */
  int signal_rows = 0, signal_cols = 0, rows = 0, cols = 0;

  int *buffer_data = NULL;
  int signal_length = 0, error = 0, buffer_size = 0;
  int zero = 0, one = 1, i = 0;

  printf("entering my sci_fft C gateway 2009-02-27\n");

  CheckRhs(1,4); /* but RHS=3 invalid */
  CheckLhs(1,1);

  /* collecting and checking arguments */
  /* intentionally no breaks between cases to collect all arguments */
  switch (Rhs) {
  case 4:
    /* */
    GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
/*     if (rows != 1 || cols != 1) { */
/*       Error(60); */
/*       return 1; */
/*     } */
    increment = (int)argument[0];
    printf("arg4 : %d\n", increment);
    GetRhsVarMatrixDouble(3, &rows, &cols, &argument);
/*     if (rows != 1 || cols != 1) { */
/*       Error(60); */
/*       return 1; */
/*     } */
    dimensions_depth = (int)argument[0];
    printf("arg3 : %d\n", dimensions_depth);
    // dimension_count = cols;
    dimension_count = 3;
  case 2:
    GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
    inverse = argument[0];
    if (rows != 1 || cols != 1) {
      Error(60);
      return 1;
    }
    if (inverse != 1 && inverse != -1) {
      Error(5);
      return 1;
    }
  case 1:
    GetVarDimension(1, &signal_rows, &signal_cols);
    dimension_count = Max(dimension_count, (signal_rows == 1 || signal_cols == 1) ? 1 : 2);
    signal_length = signal_rows * signal_cols;    
    if (signal_length <= 1) {
      Error(60);
      return 1;
    }
    iAllocComplexMatrixOfDouble(Rhs + 1, signal_rows, signal_cols, &frequencies_real, &frequencies_imaginary);
    if (iIsComplex(1)) {
      printf("Complex vector\n");
      GetRhsVarMatrixComplex(1, &signal_rows, &signal_cols, &signal_real, &signal_imaginary);
      C2F(dcopy)(&signal_length, signal_real, &one, frequencies_real, &one);
      C2F(dcopy)(&signal_length, signal_imaginary, &one, frequencies_imaginary, &one);
    }
    else {
      printf("Real vector\n");
      GetRhsVarMatrixDouble(1, &signal_rows, &signal_cols, &signal_real);
      C2F(dcopy)(&signal_length, signal_real, &one, frequencies_real, &one);
      C2F(dset)(&signal_length, &zero, frequencies_imaginary, &one);
    }
    break;
  default:
    Error(39);
    return 1;
  }

  /* upper bound on the required workspace for dfft2 */
  buffer_size = 8 * maxfactor(signal_length) + 24;
  buffer_data = (int *)calloc(buffer_size, sizeof(int));
  if (buffer_data == NULL) {
    printf("mem allocation error !\n");
    return 1;
  }

  /* dimension ? */
  switch (dimension_count) {
  case 1:
    printf("1-dimensional case...\n");
    if (is_pow2(signal_length) && signal_length < pow(2, 15)) {
      printf("length is a power of 2...\n");
      C2F(fft842)(&inverse, &signal_length, frequencies_real, frequencies_imaginary, &error);
    }
    else {
      printf("length is NOT a power of 2...\n");
      C2F(dfft2)(frequencies_real, frequencies_imaginary, &one, &signal_length, &one,
		 &inverse, &error, buffer_data, &buffer_size);
    }
    break;
  case 2:
    printf("2-dimensional case...\n");
    if (is_pow2(signal_rows) && signal_rows < pow(2, 15)) {
      printf("signal_rows IS power of 2\n");
      for (i = 0 ; i < signal_cols ; i++) {
	C2F(fft842)(&inverse, &signal_rows, &(frequencies_real[signal_rows * i]),
		    &(frequencies_imaginary[signal_rows * i]), &error);
      }
    }
    else {
      printf("signal_rows NOT power of 2\n");
      C2F(dfft2)(frequencies_real, frequencies_imaginary, &signal_cols, &signal_rows, &one,
		 &inverse, &error, buffer_data, &buffer_size);
    }
    if (is_pow2(signal_cols) && signal_cols < pow(2, 15)) {
      printf("signal_cols IS a power of 2\n");
      double *temp_real, *temp_imaginary;
      temp_real = (double *)malloc(signal_cols * sizeof(double));
      temp_imaginary = (double *)malloc(signal_cols * sizeof(double));
      if (temp_real == NULL || temp_imaginary == NULL) {
	Scierror(999, _("%s : Memory allocation error.\n"), fname);
	return 1;
      }
      for (i = 0 ; i < signal_rows ; i++) {
	C2F(dcopy)(&signal_cols, &(frequencies_real[i]), &signal_rows, temp_real, &one);
	C2F(dcopy)(&signal_cols, &(frequencies_imaginary[i]), &signal_rows, temp_imaginary, &one);
	C2F(fft842)(&inverse, &signal_cols, temp_real, temp_imaginary, &error);
	C2F(dcopy)(&signal_cols, temp_real, &one, &(frequencies_real[i]), &signal_rows);
	C2F(dcopy)(&signal_cols, temp_imaginary, &one, &(frequencies_imaginary[i]), &signal_rows);
      }
      free(temp_imaginary);
      free(temp_real);
    }
    else {
      printf("signal_cols NOT a power of 2\n");
      C2F(dfft2)(frequencies_real, frequencies_imaginary, &one, &signal_cols, &signal_rows,
		 &inverse, &error, buffer_data, &buffer_size);
    }
    break;
  default:
    printf("multi-dimensional case...\n");
    /* pending [ */
    /* TODO: translate code from Fortran */
    {
      int nspn, nseg, n;
      n = dimensions_depth;
      nspn = increment;
      nseg = signal_length / n / nspn; /* translated litterally from Fortran... but... wtf ?! */
      printf("nspn = %d, nseg = %d, n= %d\n", nspn, nseg, n);
      C2F(dfft2)(frequencies_real, frequencies_imaginary, &nseg, &n, &nspn, &inverse, &error, buffer_data, &buffer_size);
    }
    /* ] pending */
    break;
  }

  printf("errlevel = %d\n", error);

  /* preparing the return value */
  LhsVar(1) = Rhs + 1;
  PutLhsVar();
  free(buffer_data);
  buffer_data = NULL;
  printf("leaving my sci_fft\n");

/*   printf("signal : "); */
/*   print_array(frequencies_real, 10); */
/*   printf("\nimag part : "); */
/*   print_array(frequencies_imaginary, 10); */
/*   printf("\n"); */

  return 0;
}
/****************************************************************/
/* VL 26/01/2009
   translated extract from dfftbi.f
   pityfully needed to allocate the appropriate mem
   translated litterally to make sure the result is the same
 */
int maxfactor(int n) {
  int nfac[15];
  int m, j, jj, k, kt, max;

  m = 0;
  k = abs(n);
  while (k-(k/16)*16 == 0)
    {
      m++;
      nfac[m-1] = 4;
      k = k/16;
    }
  j = 3;
  jj = 9;
  goto r40;
 r30:
  m++;
  nfac[m-1] = j;
  k = k/jj;
 r40:
  if (k % jj == 0)
    goto r30;
  j += 2;
  jj = j*j;
  if (jj <= k)
    goto r40;
  if (k > 4)
    goto r50;
  kt = m;
  nfac[m] = k;
  if (k != 1)
    m++;
  goto r90;
 r50:
  if (k-(k/4)*4 != 0)
    goto r60;
  m++;
  nfac[m-1] = 2;
  k /= 4;

 r60:
  kt = m;
  j = 2;
 r70:
  if (k%j != 0)
    goto r80;
  m++;
  nfac[m-1] = j;
  k = k/j;
 r80:
  j = ((j+1)/2)*2 + 1;
  if (j <= k)
    goto r70;
 r90:
  if (kt == 0) goto r110;
  j = kt;
  do {
    m = m + 1;
    nfac[m-1] = nfac[j-1];
    j--;
  } while (j != 0);
 r110:
  // find the maximum factor
  max = nfac[0];
  for (j = 0 ; j<m ; j++) {
    // printf("nfac[%d] == %d\n", j, nfac[j]);
    max = (nfac[j] > max) ? nfac[j] : max;
  }
  return max;
}
