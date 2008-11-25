#ifndef __SCISPARSE_H__
#define __SCISPARSE_H__

/**
 * Structure used for sparse matrix
 */
typedef struct scisparse {
	int m;
	int n;
	int it;
	int nel; /**< number of non nul elements */
	int *mnel;/**< mnel[i]: number of non nul elements of row i, size m */
	int *icol; /**< icol[j]: column of the j-th non nul element, size nel */
	double *R; /**< R[j]: real value of the j-th non nul element, size nel */
	double *I ; /**< I[j]: imag value of the j-th non nul element, size nel */
} SciSparse ;

#endif /* __SCISPARSE_H__ */