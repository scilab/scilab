/*********************************************************/
/* TAUCS                                                 */
/* Author: Sivan Toledo                                  */
/*********************************************************/

#define TAUCS_LOWER      1
#define TAUCS_UPPER      2
#define TAUCS_TRIANGULAR 4
#define TAUCS_SYMMETRIC  8

typedef struct
{
    int     n;    /* columns                      */
    int     m;    /* rows; don't use if symmetric   */
    int     flags;
    int*    colptr; /* pointers to where columns begin in rowind and values. */
    /* 0-based. Length is (n+1). */
    int*    rowind; /* row indices */
    double* values; /* numerical values */
} taucs_ccs_matrix;

taucs_ccs_matrix* taucs_ccs_create(int m, int n, int nnz);
void              taucs_ccs_free  (taucs_ccs_matrix* matrix);

void              taucs_ccs_genmmd(taucs_ccs_matrix* matrix,
                                   int** perm, int** invperm);

taucs_ccs_matrix* taucs_ccs_permute_symmetrically(taucs_ccs_matrix* A,
        int* perm, int* invperm);
void taucs_vec_permute(int n, double v[], double pv[], int p[]);
void taucs_vec_ipermute(int n, double pv[], double v[], int invp[]);

void* taucs_ccs_factor_llt_mf(taucs_ccs_matrix* A);

int   taucs_supernodal_solve_llt(void* vL,
                                 double* x, double* b);

void taucs_supernodal_factor_free(void* L);

taucs_ccs_matrix* taucs_supernodal_factor_to_ccs(void* L);

int taucs_get_nnz_from_supernodal_factor(void* vL);




