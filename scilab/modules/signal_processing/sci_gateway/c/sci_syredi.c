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
extern void C2F(syredi)(int *maxdeg, int *ityp, int *iapro,
			double *om, double *adelp, double *adels,
			/* outputs */
			int *deg_count, int *zeros_count,
			double *fact,
			double *b2, double *b1, double *b0,
			double *c1, double *c0,
			double *zzr, double *zzi,
			double *zpr, double *zpi,
			int *ierr,
			/* working buffers */
			double *spr, double *spi,
			double *pren, double *pimn,
			double *zm, double *sm, double *rom,
			/* v-- doutful types but whatever... */
			double *nzero, double *nze);
/****************************************************************/
enum filter_type {
  low_pass = 1,
  high_pass = 2,
  band_pass = 3,
  stop_band = 4};

enum design_type {
  butterworth = 1,
  elliptic = 2,
  chebytchev1 = 3,
  chebytchev2 = 4};

int syredi_buffered(enum filter_type filter, enum design_type design,
		    double cutoff_frequencies[4],
		    double ripple_passband, double ripple_stopband,
		    /* outputs */
		    int *deg_count, int *zeros_count, double *fact,
		    double b2[], double b1[], double b0[],
		    double c1[], double c0[],
		    double zzr[], double zzi[],
		    double zpr[], double zpi[]);

void questionable_copy(int count,
		       double input_real[], double input_imaginary[],
		       double output_real[], double output_imaginary[]);

int coherent_cutoffs(double cutoffs[], enum filter_type kind);

int is_sorted_ascending(double values[], int length);

double maximum(double values[], int count);
double minimum(double values[], int count);

int sci_syredi(char *fname, unsigned long fname_len)
{
  enum filter_type filter;
  enum design_type design;
  int rows, cols, one = 1;
  double *argument = NULL, *cutoff_frequencies = NULL;
  double ripple_passband, ripple_stopband;

  /* output variables */
  int deg_count, zeros_count, error;
  double *fact, *b0, *b1, *b2, *c0, *c1;
  double
    *zeros_real = NULL, *zeros_imaginary = NULL,
    *poles_real = NULL, *poles_imaginary = NULL;

  /* output buffers used for computation, ultimately copied to output variables */
  #define OUTPUT_BUFFERS_COUNT 9
  int output_buffers_count = OUTPUT_BUFFERS_COUNT;
  int output_buffers_sizes[OUTPUT_BUFFERS_COUNT] =
    {32, 32, 32, 32, 32, 64, 64, 64, 64};
  double *output_buffers[OUTPUT_BUFFERS_COUNT];
  int output_cursor;
  #undef OUTPUT_BUFFERS_COUNT
  
  CheckRhs(5,5);
  CheckLhs(8,8);

  /* arg1: filter kind */
  GetRhsVarMatrixDouble(1, &rows, &cols, &argument);
  filter = (int)argument[0];

  /* arg2: approximation kind */
  GetRhsVarMatrixDouble(2, &rows, &cols, &argument);
  design = (int)argument[0];

  /* arg3: cutoff frequencies */
  GetRhsVarMatrixDouble(3, &rows, &cols, &cutoff_frequencies);
  if (rows != 1 || cols != 4) {
    Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d array expected.\n"), fname, 3, 1, 4);
    return 1;
  }
  if (minimum(cutoff_frequencies, 4) < 0 || maximum(cutoff_frequencies, 4) > M_PI) {
    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, 4, "0", "%pi");
    return 1;
  }
  if (((filter == low_pass || filter == high_pass)
       && !is_sorted_ascending(cutoff_frequencies, 2)) ||
      ((filter == band_pass || filter == stop_band)
       && !is_sorted_ascending(cutoff_frequencies, 4))) {
    Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), fname, 3);
    return 1;
  }

  /* arg4: passband's ripple */
  GetRhsVarMatrixDouble(4, &rows, &cols, &argument);
  ripple_passband = argument[0];
  if (ripple_passband <= 0 || ripple_passband >= 1) {
    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, 4, "0", "1");
    return 1;
  }

  /* arg5: stopband's ripple */
  GetRhsVarMatrixDouble(5, &rows, &cols, &argument);
  ripple_stopband = argument[0];
  if (ripple_stopband <= 0 || ripple_stopband >= 1) {
    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), fname, 5, "0", "1");
    return 1;
  }

  for (output_cursor = 0 ; output_cursor < output_buffers_count ; ++output_cursor) {
    output_buffers[output_cursor] = (double *)MALLOC(output_buffers_sizes[output_cursor] * sizeof(double));
    if (output_buffers[output_cursor] == NULL) {
      Scierror(999, _("%s : Memory allocation error.\n"), fname);
      // TODO: free buffers (elegantly, possibly use an abstraction for this)
      return 1;
    }
  }
  
  iAllocMatrixOfDouble(Rhs + 1, 1, 1, &fact);

  error = syredi_buffered(/* inputs */
			  filter, design,
			  cutoff_frequencies, ripple_passband, ripple_stopband,
			  /* outputs */
			  &zeros_count, &deg_count, fact,
			  output_buffers[0], output_buffers[1],
			  output_buffers[2], output_buffers[3],
			  output_buffers[4], output_buffers[5],
			  output_buffers[6], output_buffers[7],
			  output_buffers[8]);
  if (error) {
    Scierror(999, _("%s : Memory allocation error.\n"), fname);
    return 1;
  }

  /* ret1: fact */
  LhsVar(1) = Rhs + 1;

  /* ret2: b2 */
  iAllocMatrixOfDouble(Rhs + 2, 1, deg_count, &b2);
  C2F(dcopy)(&deg_count, output_buffers[0], &one, b2, &one);
  LhsVar(2) = Rhs + 2;

  /* ret3: b1 */
  iAllocMatrixOfDouble(Rhs + 3, 1, deg_count, &b1);
  C2F(dcopy)(&deg_count, output_buffers[1], &one, b1, &one);
  LhsVar(3) = Rhs + 3;

  /* ret4: b0 */
  iAllocMatrixOfDouble(Rhs + 4, 1, deg_count, &b0);
  C2F(dcopy)(&deg_count, output_buffers[2], &one, b0, &one);
  LhsVar(4) = Rhs + 4;

  /* ret5: c1 */
  iAllocMatrixOfDouble(Rhs + 5, 1, deg_count, &c1);
  C2F(dcopy)(&deg_count, output_buffers[3], &one, c1, &one);
  LhsVar(5) = Rhs + 5;

  /* ret6: c0 */
  iAllocMatrixOfDouble(Rhs + 6, 1, deg_count, &c0);
  C2F(dcopy)(&deg_count, output_buffers[4], &one, c0, &one);
  LhsVar(6) = Rhs + 6;

  /* ret7: zeros */
  iAllocComplexMatrixOfDouble(Rhs + 7, 1, zeros_count, &zeros_real, &zeros_imaginary);
  questionable_copy(zeros_count, output_buffers[5], output_buffers[6],
		    zeros_real, zeros_imaginary);
  LhsVar(7) = Rhs + 7;

  /* ret8: poles */
  iAllocComplexMatrixOfDouble(Rhs + 8, 1, zeros_count, &poles_real, &poles_imaginary);
  questionable_copy(zeros_count, output_buffers[7], output_buffers[8], poles_real, poles_imaginary);
  LhsVar(8) = Rhs + 8;

  for (output_cursor-- ; output_cursor >= 0 ; output_cursor--) {
    FREE(output_buffers[output_cursor]);
    output_buffers[output_cursor] = NULL;
  }

  PutLhsVar();
  return error;
}

int syredi_buffered(/* inputs */
		    enum filter_type filter, enum design_type design,
		    double cutoff_frequencies[4], double ripple_passband, double ripple_stopband,
		    /* outputs */
		    int *zeros_count, int *deg_count, double *fact,
		    double b2[], double b1[], double b0[],
		    double c1[], double c0[],
		    double zzr[], double zzi[],
		    double zpr[], double zpi[])
{
  #define BUFFERS_COUNT 9
  const int buffers_count = BUFFERS_COUNT;
  int buffers_sizes[BUFFERS_COUNT] = {64, 64, 64, 64, 256, 256, 16, 64, 64};
  double *buffers[BUFFERS_COUNT];
  #undef BUFFERS_COUNT
  int maxdeg = 64, error = 1, i;

  for (i = 0 ; i < buffers_count ; ++i) {
    buffers[i] = (double *)MALLOC(buffers_sizes[i] * sizeof(double));
    if (buffers[i] == NULL) {
      break;
    }
  }
  if (i == buffers_count) {
    C2F(syredi)(/* inputs */
		&maxdeg, (int *) &filter, (int *) &design,
		cutoff_frequencies, &ripple_passband, &ripple_stopband,
		/* outputs */
		zeros_count, deg_count, fact,
		b2, b1, b0, c1, c0,
		zzr, zzi, zpr, zpi,
		&error,
		/* buffers */
		buffers[0], buffers[1], buffers[2], buffers[3],
		buffers[4], buffers[5],
		buffers[6],
		buffers[7], buffers[8]);
  }
  for (i-- ; i >= 0 ; i--) {
    FREE(buffers[i]);
    buffers[i] = NULL;
  }
  return error;
}

/* copies input to output adding conjugates */
/* questionable until the reason of input data loss is clarified */
/* translated from sci_syredi.f */
void questionable_copy(int length,
		       double input_real[], double input_imaginary[],
		       double output_real[], double output_imaginary[])
{
  int i, j;
  for (i = 0, j = 0 ; j < length ; ++i, ++j) {
    output_real[j] = input_real[i];
    output_imaginary[j] = input_imaginary[i];
    if (input_imaginary[i] != 0L) { /* as in fortran's source */
      ++j;
      output_real[j] = input_real[i];
      output_imaginary[j] = - input_imaginary[i];
    }
  }
}

/* checks whether the values array (with length elements) is ascendingly sorted */
int is_sorted_ascending(double values[], int length) {
  int i;
  for (i = 0 ; i < length - 1 ; ++i) {
    if (values[i] > values[i + 1]) {
      return 0;
    }
  }
  return 1;
}

/* returns the maximum of the values array (with length elements) */
double maximum(double values[], int length) {
  int i;
  double max;
  for (i = 0, max = values[0] ; i < length ; ++i) {
    max = Max(values[i], max);
  }
  return max;
}

/* returns the minimum of the values array (with length elements) */
double minimum(double values[], int length) {
  int i;
  double min;
  for (i = 0, min = values[0] ; i < length ; ++i) {
    min = Min(values[i], min);
  }
  return min;
}
