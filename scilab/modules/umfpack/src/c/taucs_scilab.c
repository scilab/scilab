/*
 *   This big file contains all the taucs routines needed for the
 *   scilab interface onto the taucs supernodal multi-frontal choleski
 *   solver :
 *        - internal structures used by the solver
 *        - genmmd interface
 *        - create / free taucs matrices routines
 *        - permutations routines
 *        - symbolic analysis routines
 *        - super nodal multi frontal factor routines
 *        - super nodal multi frontal solve routines
 *        - various utilities
 *
 *    All this code is from different files of the TAUCS solver
 *    lib from Sivan Toledo (see the TAUCS_license.txt file).
 *
 *    Very minor modifs have been done (Bruno) :
 *        - replace the _ for calling fortran routine by the
 *          scilab macro F2C  (call to genmmd and calls to blas
 *          routines)
 *        - the print messages that I have stayed are redirected
 *          to the scilab window with sciprint
 *        - I have added a small routine to get cnz
 *
 *    Thanks to Sivan Toledo
 */

/*************************************************************/
/*                                                           */
/*************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "taucs_scilab.h"
#include "machine.h"   /* F2C macro */
#include "sci_malloc.h"
#include "BOOL.h"
#include "sciprint.h"
#include "localization.h"

#ifdef _MSC_VER
#define alloca(x) MALLOC(x)
#endif

#define BLAS_FLOPS_CUTOFF  1000.0
#define SOLVE_DENSE_CUTOFF 5

extern int C2F(genmmd)();
extern int C2F(dpotrf)();
extern int C2F(dtrsm)();
extern int C2F(dsyrk)();
extern int C2F(dgemm)();
/*************************************************************/
/* structures                                                */
/*************************************************************/

typedef struct
{
    int     sn_size;
    int     n;
    int*    rowind;

    int     up_size;
    int*    sn_vertices;
    int*    up_vertices;
    double* f1;
    double* f2;
    double* u;
} supernodal_frontal_matrix;

typedef struct
{
    char    uplo;     /* 'u' for upper, 'l' for lower, ' ' don't know; prefer lower. */
    int     n;        /* size of matrix */
    int     n_sn;     /* number of supernodes */

    int* parent;      /* supernodal elimination tree */
    int* first_child;
    int* next_child;

    int* sn_size;     /* size of supernodes (diagonal block) */
    int* sn_up_size;  /* size of subdiagonal update blocks   */
    int** sn_struct;  /* row structure of supernodes         */

    int* sn_blocks_ld;  /* lda of supernode blocks */
    double** sn_blocks; /* supernode blocks        */

    int* up_blocks_ld;  /* lda of update blocks    */
    double** up_blocks; /* update blocks           */
} supernodal_factor_matrix;

/*************************************************************/
/* for qsort                                                 */
/*************************************************************/

static int* compare_indirect_map;
static int compare_indirect_ints(const void* vx, const void* vy)
{
    int* ix = (int*)vx;
    int* iy = (int*)vy;
    if (compare_indirect_map[*ix] < compare_indirect_map[*iy])
    {
        return -1;
    }
    if (compare_indirect_map[*ix] > compare_indirect_map[*iy])
    {
        return  1;
    }
    return 0;
}

/*********************************************************/
/* Interface to MMD                                      */
/*********************************************************/

void taucs_ccs_genmmd(taucs_ccs_matrix* m, int** perm, int** invperm)
{
    int  n, maxint, delta, nofsub;
    int* xadj;
    int* adjncy;
    int* invp;
    int* prm;
    int* dhead;
    int* qsize;
    int* llist;
    int* marker;

    int* len;
    int* next;

    int  nnz, i, j, ip;

    if (!(m->flags & TAUCS_SYMMETRIC))
    {
        sciprint("%s: %s", "taucs_ccs_genmmd", _("GENMMD ordering only works on symmetric matrices.\n"));
        *perm    = NULL;
        *invperm = NULL;
        return;
    }
    /* this routine may actually work on UPPER as well */
    if (!(m->flags & TAUCS_LOWER))
    {
        sciprint("%s: %s", "taucs_ccs_genmmd", _("The lower part of the matrix must be represented.\n"));
        *perm    = NULL;
        *invperm = NULL;
        return;
    }

    *perm    = NULL;
    *invperm = NULL;

    n   = m->n;
    nnz = (m->colptr)[n];

    /* I copied the value of delta and the size of */
    /* from SuperLU. Sivan                         */

    delta = 1; /* DELTA is a parameter to allow the choice of nodes
		whose degree <= min-degree + DELTA. */

    maxint = 32000;

    assert(sizeof(int) == 4);
    maxint = 2147483647; /* 2**31-1, for 32-bit only! */

    xadj   = (int*) MALLOC((n + 1)     * sizeof(int));
    adjncy = (int*) MALLOC((2 * nnz - n) * sizeof(int));
    invp   = (int*) MALLOC((n + 1)     * sizeof(int));
    prm    = (int*) MALLOC(n         * sizeof(int));
    dhead  = (int*) MALLOC((n + 1)     * sizeof(int));
    qsize  = (int*) MALLOC((n + 1)     * sizeof(int));
    llist  = (int*) MALLOC(n         * sizeof(int));
    marker = (int*) MALLOC(n         * sizeof(int));

    if (!xadj || !adjncy || !invp || !prm
            || !dhead || !qsize || !llist || !marker)
    {
        FREE(xadj  );
        FREE(adjncy);
        FREE(invp  );
        FREE(prm   );
        FREE(dhead );
        FREE(qsize );
        FREE(llist );
        FREE(marker);
        return;
    }

    len  = dhead; /* we reuse space */
    next = qsize; /* we reuse space */

    for (i = 0; i < n; i++)
    {
        len[i] = 0;
    }

    for (j = 0; j < n; j++)
    {
        for (ip = (m->colptr)[j]; ip < (m->colptr)[j + 1]; ip++)
        {
            i = (m->rowind)[ip];
            if (i != j)
            {
                len[i] ++;
                len[j] ++;
            }
        }
    }
    xadj[0] = 1;
    for (i = 1; i <= n; i++)
    {
        xadj[i] = xadj[i - 1] + len[i - 1];
    }


    /* use degree as a temporary */
    for (i = 0; i < n; i++)
    {
        next[i] = xadj[i] - 1;
    }

    for (j = 0; j < n; j++)
    {
        for (ip = (m->colptr)[j]; ip < (m->colptr)[j + 1]; ip++)
        {
            i = (m->rowind)[ip];
            assert( next[i] < 2 * nnz - n );
            assert( next[j] < 2 * nnz - n );
            if (i != j)
            {
                adjncy[ next[i] ] = j + 1;
                adjncy[ next[j] ] = i + 1;
                next[i] ++;
                next[j] ++;
            }
        }
    }

    F2C(genmmd)(&n,
                xadj, adjncy,
                invp, prm,
                &delta,
                dhead, qsize, llist, marker,
                &maxint, &nofsub);

    FREE(marker);
    FREE(llist );
    FREE(qsize );
    FREE(dhead );
    FREE(xadj  );
    FREE(adjncy);

    for (i = 0; i < n; i++)
    {
        prm[i] --;
    }
    for (i = 0; i < n; i++)
    {
        invp[ prm[i] ] = i;
    }

    *perm    = prm;
    *invperm = invp;
}

/*********************************************************/
/* CCS : create free taucs matrix routines               */
/*********************************************************/

taucs_ccs_matrix*
taucs_ccs_create(int m, int n, int nnz)
{
    taucs_ccs_matrix* matrix;

    matrix = (taucs_ccs_matrix*) MALLOC(sizeof(taucs_ccs_matrix));
    if (!matrix)
    {
        sciprint(_("%s: No more memory.\n"), "taucs_ccs_create");
        return NULL;
    }
    matrix->flags = 0;
    matrix->n = n;
    matrix->m = m;
    matrix->colptr = (int*)    MALLOC((n + 1) * sizeof(int));
    matrix->rowind = (int*)    MALLOC(nnz   * sizeof(int));
    matrix->values = (double*) MALLOC(nnz   * sizeof(double));
    if (!(matrix->colptr) || !(matrix->rowind))
    {
        sciprint(_("%s: No more memory (n=%d, nnz=%d).\n"), "taucs_ccs_create", n, nnz);
        FREE(matrix->colptr);
        FREE(matrix->rowind);
        FREE(matrix->values);
        FREE (matrix);
        return NULL;
    }

    return matrix;
}

void taucs_ccs_free(taucs_ccs_matrix* matrix)
{
    FREE(matrix->rowind);
    FREE(matrix->colptr);
    FREE(matrix->values);
    FREE(matrix);
}

/*********************************************************/
/* CCS : permutation routines                            */
/*********************************************************/

taucs_ccs_matrix*
taucs_ccs_permute_symmetrically(taucs_ccs_matrix* A, int* perm, int* invperm)
{
    taucs_ccs_matrix* PAPT;
    int n;
    int nnz;
    /* int* colptr; */
    int* len;
    int i, j, ip, I, J;
    double AIJ;

    /*assert(A->flags & TAUCS_SYMMETRIC);*/
    assert(A->flags & TAUCS_LOWER);

    n   = A->n;
    nnz = (A->colptr)[n];

    PAPT = taucs_ccs_create(n, n, nnz);
    /*PAPT->flags = TAUCS_SYMMETRIC | TAUCS_LOWER;*/
    PAPT->flags = A->flags;

    len    = (int*) MALLOC(n * sizeof(int));
    //colptr = (int*) MALLOC(n * sizeof(int));

    for (j = 0; j < n; j++)
    {
        len[j] = 0;
    }

    for (j = 0; j < n; j++)
    {
        for (ip = (A->colptr)[j]; ip < (A->colptr)[j + 1]; ip++)
        {
            /*i = (A->rowind)[ip] - (A->indshift);*/
            i = (A->rowind)[ip];

            I = invperm[i];
            J = invperm[j];

            if (I < J)
            {
                int T = I;
                J = T;
            }
            len[J] ++;
        }
    }

    (PAPT->colptr)[0] = 0;
    for (j = 1; j <= n; j++)
    {
        (PAPT->colptr)[j] = (PAPT->colptr)[j - 1] + len[j - 1];
    }

    for (j = 0; j < n; j++)
    {
        len[j] = (PAPT->colptr)[j];
    }

    for (j = 0; j < n; j++)
    {
        for (ip = (A->colptr)[j]; ip < (A->colptr)[j + 1]; ip++)
        {
            /*i   = (A->rowind)[ip] - (A->indshift);*/
            i   = (A->rowind)[ip];
            AIJ = (A->values)[ip];

            I = invperm[i];
            J = invperm[j];

            if (I < J)
            {
                int T = I;
                I = J;
                J = T;
            }

            /*(PAPT->rowind)[ len[J] ] = I + (PAPT->indshift);*/
            (PAPT->rowind)[ len[J] ] = I;
            (PAPT->values)[ len[J] ] = AIJ;

            len[J] ++;
        }
    }

    if (len)
    {
        FREE(len);
        len = NULL;
    }

    return PAPT;
}

void taucs_vec_permute(int n, double v[], double pv[], int p[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        pv[i] = v[p[i]];
    }
}

void taucs_vec_ipermute(int n, double pv[], double v[], int invp[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        v[invp[i]] = pv[i];
    }
}


/*************************************************************/
/* create and free the factor object                         */
/*************************************************************/

static supernodal_factor_matrix*
multifrontal_supernodal_create(void)
{
    supernodal_factor_matrix* L;

    L = (supernodal_factor_matrix*) MALLOC(sizeof(supernodal_factor_matrix));
    if (!L)
    {
        return NULL;
    }
    L->uplo      = 'l';
    L->n         = -1; /* unused */

    L->sn_struct   = NULL;
    L->sn_size     = NULL;
    L->sn_up_size  = NULL;
    L->parent      = NULL;
    L->first_child = NULL;
    L->next_child  = NULL;
    L->sn_blocks_ld  = NULL;
    L->sn_blocks     = NULL;
    L->up_blocks_ld  = NULL;
    L->up_blocks     = NULL;

    return L;
}

void taucs_supernodal_factor_free(void* vL)
{
    supernodal_factor_matrix* L = (supernodal_factor_matrix*) vL;
    int sn;

    FREE(L->parent);
    FREE(L->first_child);
    FREE(L->next_child);

    FREE(L->sn_size);
    FREE(L->sn_up_size);
    FREE(L->sn_blocks_ld);
    FREE(L->up_blocks_ld);
    if (L->sn_struct)
        for (sn = 0; sn < L->n_sn; sn++)
        {
            FREE(L->sn_struct[sn]);
        }

    if (L->sn_blocks)
        for (sn = 0; sn < L->n_sn; sn++)
        {
            FREE(L->sn_blocks[sn]);
        }

    if (L->up_blocks)
        for (sn = 0; sn < L->n_sn; sn++)
        {
            FREE(L->up_blocks[sn]);
        }
    /*
    for (sn=0; sn<L->n_sn; sn++)
    {
      FREE(L->sn_struct[sn]);
      FREE(L->sn_blocks[sn]);
      FREE(L->up_blocks[sn]);
    }*/

    FREE(L->sn_struct);
    FREE(L->sn_blocks);
    FREE(L->up_blocks);

    FREE(L);
}

/*************************************************************/
/* create and free frontal matrices                          */
/*************************************************************/

static supernodal_frontal_matrix*
supernodal_frontal_create(int* firstcol_in_supernode,
                          int sn_size,
                          int n,
                          int* rowind)
{
    supernodal_frontal_matrix* tmp;
    int iDontCheckf1 = 0;
    int iDontCheckf2 = 0;
    int iDontChecku = 0;

    tmp = (supernodal_frontal_matrix*)MALLOC(sizeof(supernodal_frontal_matrix));
    if (tmp == NULL)
    {
        return NULL;
    }

    tmp->sn_size = sn_size;
    tmp->n = n;

    tmp->rowind = rowind;

    tmp->n = n;
    tmp->sn_size = sn_size;
    tmp->up_size = n - sn_size;

    tmp->sn_vertices = rowind;
    tmp->up_vertices = rowind + sn_size;

    if (tmp->sn_size > 0)
    {
        tmp->f1 = (double*)CALLOC((tmp->sn_size) * (tmp->sn_size), sizeof(double));
    }
    else
    {
        iDontCheckf1 = 1;
        tmp->f1 = NULL;
    }

    if (tmp->sn_size  > 0 && tmp->up_size > 0)
    {
        tmp->f2 = (double*)CALLOC((tmp->up_size) * (tmp->sn_size), sizeof(double));
    }
    else
    {
        iDontCheckf2 = 1;
        tmp->f2 = NULL;
    }

    if (tmp->up_size > 0)
    {
        tmp->u  = (double*)CALLOC((tmp->up_size) * (tmp->up_size), sizeof(double));
    }
    else
    {
        iDontChecku = 1;
        tmp->u = NULL;
    }

    /*check allocation only if size is > 0 */
    if ( (tmp->f1 == NULL && iDontCheckf1 == 0) ||
            (tmp->f2 == NULL && iDontCheckf2 == 0) ||
            (tmp->u  == NULL && iDontChecku  == 0))
    {
        FREE(tmp->u);
        FREE(tmp->f1);
        FREE(tmp->f2);
        FREE(tmp);
        return NULL;
    }

    assert(tmp);
    return tmp;
}

static void supernodal_frontal_free(supernodal_frontal_matrix* to_del)
{
    /* f1 and f2 are moved to the factor */
    FREE(to_del->u);
    FREE(to_del);
}


/*************************************************************/
/* factor a frontal matrix                                   */
/*************************************************************/

static int
multifrontal_supernodal_front_factor(int sn,
                                     int* firstcol_in_supernode,
                                     int sn_size,
                                     taucs_ccs_matrix* A,
                                     supernodal_frontal_matrix* mtr,
                                     int* bitmap,
                                     supernodal_factor_matrix* snL)
{
    int i, j;
    int* ind;
    double* re;
    int INFO = -1;
    double done      =  1.0;
    double dminusone = -1.0;

    /* creating transform for real indices */
    for (i = 0; i < mtr->sn_size; i++)
    {
        bitmap[mtr->sn_vertices[i]] = i;
    }
    for (i = 0; i < mtr->up_size; i++)
    {
        bitmap[mtr->up_vertices[i]] = mtr->sn_size + i;
    }

    /* adding sn_size column of A to first sn_size column of frontal matrix */
    for (j = 0; j < (mtr->sn_size); j++)
    {
        ind = &(A->rowind[A->colptr[*(firstcol_in_supernode + j)]]);
        re  = &(A->values[A->colptr[*(firstcol_in_supernode + j)]]);
        for (i = 0; i < A->colptr[*(firstcol_in_supernode + j) + 1] - A->colptr[*(firstcol_in_supernode + j)];	i++)
        {
            if (bitmap[ind[i]] < mtr->sn_size)
            {
                mtr->f1[ (mtr->sn_size)*j + bitmap[ind[i]]] += re[i];
            }
            else
            {
                mtr->f2[ (mtr->up_size)*j + bitmap[ind[i]] - mtr->sn_size] += re[i];
            }
        }
    }

    /* we use the BLAS through the Fortran interface */

    /* solving of lower triangular system for L */
    if (mtr->sn_size)
    {
        F2C(dpotrf)("LOWER",
                    &(mtr->sn_size),
                    mtr->f1, &(mtr->sn_size),
                    &INFO);
    }

    if (INFO && INFO != -1)  /* INFO = -1 if not initialized */
    {
        sciprint(_("    CC^T Factorization: Matrix is not positive definite.\n"));
        sciprint(_("                        nonpositive pivot in column %d\n"), mtr->sn_vertices[INFO - 1]);
        return -1;
    }

    /* getting completion for found columns of L */
    if (mtr->up_size && mtr->sn_size)
    {
        F2C(dtrsm)("Right",
                   "Lower",
                   "Transpose",
                   "No unit diagonal",
                   &(mtr->up_size), &(mtr->sn_size),
                   &done,
                   mtr->f1, &(mtr->sn_size),
                   mtr->f2, &(mtr->up_size));
    }

    (snL->sn_blocks   )[sn] = mtr->f1;
    (snL->sn_blocks_ld)[sn] = mtr->sn_size;

    (snL->up_blocks   )[sn] = mtr->f2;
    (snL->up_blocks_ld)[sn] = mtr->up_size;

    /* computation of updated part of frontal matrix */
    if (mtr->up_size && mtr->sn_size)
    {
        F2C(dsyrk)("Lower",
                   "No Transpose",
                   &(mtr->up_size), &(mtr->sn_size),
                   &dminusone,
                   mtr->f2, &(mtr->up_size),
                   &done,
                   mtr->u, &(mtr->up_size));
    }

    return 0;
}

/*************************************************************/
/* extend-add                                                */
/*************************************************************/

static void
multifrontal_supernodal_front_extend_add(
    supernodal_frontal_matrix* parent_mtr,
    supernodal_frontal_matrix* my_mtr,
    int* bitmap)
{
    int j, i, parent_i, parent_j;
    double v;

    for (i = 0; i < parent_mtr->sn_size; i++)
    {
        bitmap[parent_mtr->sn_vertices[i]] = i;
    }
    for (i = 0; i < parent_mtr->up_size; i++)
    {
        bitmap[parent_mtr->up_vertices[i]] = (parent_mtr->sn_size) + i;
    }

    /* extend add operation for update matrix */
    for (j = 0; j < my_mtr->up_size; j++)
    {
        for (i = j; i < my_mtr->up_size; i++)
        {
            parent_j = bitmap[ my_mtr->up_vertices[j] ];
            parent_i = bitmap[ my_mtr->up_vertices[i] ];
            /* we could skip this if indices were sorted */
            if (parent_j > parent_i)
            {
                int tmp = parent_j;
                parent_j = parent_i;
                parent_i = tmp;
            }

            v = (my_mtr->u)[(my_mtr->up_size) * j + i];

            if (parent_j < parent_mtr->sn_size)
            {
                if (parent_i < parent_mtr->sn_size)
                {
                    (parent_mtr->f1)[ (parent_mtr->sn_size)*parent_j + parent_i] += v;
                }
                else
                {
                    (parent_mtr->f2)[ (parent_mtr->up_size)*parent_j + (parent_i - parent_mtr->sn_size)] += v;
                }
            }
            else
            {
                (parent_mtr->u)[ (parent_mtr->up_size) * (parent_j - parent_mtr->sn_size) + (parent_i - parent_mtr->sn_size)] += v;
            }
        }
    }
}

/*************************************************************/
/* symbolic elimination                                      */
/*************************************************************/

/* UNION FIND ROUTINES */

static int uf_makeset(int* uf, int i)
{
    uf[i] = i;
    return i;
}
static int uf_find   (int* uf, int i)
{
    if (uf[i] != i)
    {
        uf[i] = uf_find(uf, uf[i]);
    }
    return uf[i];
}

static int uf_union  (int* uf, int s, int t)
{

    if (uf_find(uf, s) < uf_find(uf, t))
    {
        uf[uf_find(uf, s)] = uf_find(uf, t);
        return (uf_find(uf, t));
    }
    else
    {
        uf[uf_find(uf, s)] = uf_find(uf, t);
        return (uf_find(uf, t));
    }
}

static
void recursive_postorder(int  j,
                         int  first_child[],
                         int  next_child[],
                         int  postorder[],
                         int  ipostorder[],
                         int* next)
{
    int c;
    for (c = first_child[j]; c != -1; c = next_child[c])
    {
        recursive_postorder(c, first_child, next_child,
                            postorder, ipostorder, next);
    }

    if (postorder)
    {
        postorder [*next] = j;
    }
    if (ipostorder)
    {
        ipostorder[j] = *next;
    }
    (*next)++;
}

#define GILBERT_NG_PEYTON_ANALYSIS_SUP

/* in a few tests the supernodal version seemed slower */
#undef GILBERT_NG_PEYTON_ANALYSIS_SUP

static int ordered_uf_makeset(int* uf, int i)
{
    uf[i] = i;
    return i;
}
static int ordered_uf_find   (int* uf, int i)
{
    if (uf[i] != i)
    {
        uf[i] = uf_find(uf, uf[i]);
    }
    return uf[i];
}
static int ordered_uf_union  (int* uf, int s, int t)
{
    assert(uf[t] == t);
    assert(uf[s] == s);
    assert(t > s);
    if (t > s)
    {
        uf[s] = t;
        return t;
    }
    else
    {
        uf[t] = s;
    }
    return s;
}

static void
tree_level(int j,
           int isroot,
           int first_child[],
           int next_child[],
           int level[],
           int level_j)
{
    int c;
    if (!isroot)
    {
        level[j] = level_j;
    }
    for (c = first_child[j]; c != -1; c = next_child[c])
    {
        tree_level(c,
                   FALSE,
                   first_child,
                   next_child,
                   level,
                   level_j + 1);
    }
}

static void
tree_first_descendant(int j,
                      int isroot,
                      int first_child[],
                      int next_child[],
                      int ipostorder[],
                      int first_descendant[])
{
    int c;
    int fd = ipostorder[j];
    for (c = first_child[j]; c != -1; c = next_child[c])
    {
        tree_first_descendant(c,
                              FALSE,
                              first_child,
                              next_child,
                              ipostorder,
                              first_descendant);
        if (first_descendant[c] < fd)
        {
            fd = first_descendant[c];
        }
    }
    if (!isroot)
    {
        first_descendant[j] = fd;
    }
}

int taucs_ccs_etree(taucs_ccs_matrix* A,
                    int* parent,
                    int* l_colcount,
                    int* l_rowcount,
                    int* l_nnz)
{
    int* prev_p;
    int* level;
    int* l_cc;
    int* l_rc;
    int* wt;

    int  n = A->n;
    int  u, pprime;
    int  ju;
    int* postorder;
    int* ipostorder;
    int  *first_child, *next_child;

    int i, j, k, ip, jp, kp;
    int nnz, jnnz;
    int* uf;
    int* rowptr;
    int* colind;
    int* rowcount;
    int* realroot;

    /* we need the row structures for the lower triangle */

    nnz = (A->colptr)[n];

    uf       = (int*)MALLOC(n     * sizeof(int));
    rowcount = (int*)MALLOC((n + 1) * sizeof(int));
    rowptr   = (int*)MALLOC((n + 1) * sizeof(int));
    colind   = (int*)MALLOC(nnz   * sizeof(int));

    for (i = 0; i <= n; i++)
    {
        rowcount[i] = 0;
    }
    for (j = 0; j < n; j++)
    {
        jnnz = (A->colptr)[j + 1] - (A->colptr)[j];
        for (ip = 0; ip < jnnz; ip++)
        {
            i = (A->rowind)[(A->colptr)[j] + ip];
            if (j < i)
            {
                rowcount[i]++;
            }
        }
    }

    ip = 0;
    for (i = 0; i <= n; i++)
    {
        int next_ip = ip + rowcount[i];
        rowcount[i] = ip;
        rowptr  [i] = ip;
        ip = next_ip;
    }

    for (j = 0; j < n; j++)
    {
        jnnz = (A->colptr)[j + 1] - (A->colptr)[j];
        for (ip = 0; ip < jnnz; ip++)
        {
            i = (A->rowind)[(A->colptr)[j] + ip];
            if (i == j)
            {
                continue;
            }
            assert( rowcount[i] < rowptr[i + 1] );
            colind[ rowcount[i] ] = j;
            rowcount[i]++;
        }
    }

    /* now compute the etree */

    {
        int u2, t, vroot;
        realroot = rowcount; /* reuse space */

        for (i = 0; i < n; i++)
        {
            uf_makeset(uf, i);
            realroot[i] = i;
            parent[i] = n;
            vroot = i;
            for (kp = rowptr[i]; kp < rowptr[i + 1]; kp++)
            {
                k = colind[kp];
                u2 = uf_find(uf, k);
                t = realroot[u2];
                if (parent[t] == n && t != i)
                {
                    parent[t] = i;
                    vroot = uf_union(uf, vroot, u2);
                    realroot[vroot] = i;
                }
            }
        }
    }

    FREE(colind);
    FREE(rowptr);
    FREE(rowcount);

    /* compute column counts */

    if (l_colcount || l_rowcount || l_nnz)
    {
        int* l_nz;
        int  tmp;
        int  u2, p, q;

        first_child = (int*)MALLOC((n + 1)     * sizeof(int));
        next_child  = (int*)MALLOC((n + 1)     * sizeof(int));
        postorder   = (int*)MALLOC(n     * sizeof(int));
        ipostorder  = (int*)MALLOC(n     * sizeof(int));
        wt  = (int*)MALLOC(n     * sizeof(int));
        level = (int*)MALLOC(n     * sizeof(int));
        prev_p  = (int*)MALLOC(n     * sizeof(int));

#ifdef GILBERT_NG_PEYTON_ANALYSIS_SUP
        prev_nbr  = MALLOC(n     * sizeof(int));
        first_descendant  = MALLOC(n     * sizeof(int));
#endif


        /* compute the postorder */

        for (j = 0; j <= n; j++)
        {
            first_child[j] = -1;
        }
        for (j = n - 1; j >= 0; j--)
        {
            next_child[j] = first_child[parent[j]];
            first_child[parent[j]] = j;
        }

        {
            int next = 0;
            recursive_postorder(n, first_child, next_child,
                                postorder,
                                ipostorder, &next);
        }

        /* we allocate scratch vectors to avoid conditionals */
        /* in the inner loop.                                */

        if (l_colcount)
        {
            l_cc = l_colcount;
        }
        else
        {
            l_cc = (int*) MALLOC(n * sizeof(int));
        }
        if (l_rowcount)
        {
            l_rc = l_rowcount;
        }
        else
        {
            l_rc = (int*) MALLOC(n * sizeof(int));
        }
        if (l_nnz)
        {
            l_nz = l_nnz;
        }
        else
        {
            l_nz = &tmp;
        }

        /* sort by postorder of etree */
        /* compute level, fst_desc */

        tree_level(n, TRUE, first_child, next_child,
                   level, -1);

        for (u = 0; u < n; u++)
        {
            prev_p  [u] = -1;
        }
        for (u = 0; u < n; u++)
        {
            l_rc    [u] =  1;
        }
        for (u = 0; u < n; u++)
        {
            ordered_uf_makeset(uf, u);
        }
        for (u = 0; u < n; u++)
        {
            if (first_child[u] == -1)
            {
                wt[u] = 1;    /* leaves     */
            }
            else
            {
                wt[u] =  0;    /* nonleaves */
            }
        }

#ifdef GILBERT_NG_PEYTON_ANALYSIS_SUP
        for (u = 0; u < n; u++)
        {
            prev_nbr[u] = -1;
        }

        tree_first_descendant(n, TRUE,
                              first_child, next_child, ipostorder,
                              first_descendant);
#endif

        FREE(first_child);
        FREE(next_child);

        for (p = 0; p < n; p++)
        {
            jp = postorder[p];
            if (parent[jp] != n)
            {
                wt[parent[jp]] --;
            }
            for (ip = (A->colptr)[jp]; ip < (A->colptr)[jp + 1]; ip++)
            {
                ju = (A->rowind)[ip];
                if (ju == jp)
                {
                    continue;    /* we only want proper neighbors */
                }
#ifdef GILBERT_NG_PEYTON_ANALYSIS_SUP
                if (first_descendant[jp] > prev_nbr[u])
                {
#else
                if (1)
                {
#endif
                    wt[jp] ++;
                    pprime = prev_p[ju];
                    if (pprime == -1)
                    {
                        l_rc[ju] += level[jp] - level[ju];
                    }
                    else
                    {
                        q = ordered_uf_find(uf, pprime);
                        l_rc[ju] += level[jp] - level[q];
                        wt[q] --;
                    }
                    prev_p[ju] = jp;
                }
#ifdef GILBERT_NG_PEYTON_ANALYSIS_SUP
                prev_nbr[u] = p;
#endif
            }
            if (parent[jp] != n)
            {
                if (!(ipostorder[parent[jp]] > ipostorder[jp]))
                {
                    sciprint("jp %d parent %d (ipo_j %d ipo_parent %d\n\r",
                             jp, parent[jp], ipostorder[jp], ipostorder[parent[jp]]);
                }
                assert(ipostorder[parent[jp]] > ipostorder[jp]);
                ordered_uf_union(uf, jp, parent[jp]);
            }
        }

        *l_nz = 0;
        for (u2 = 0; u2 < n; u2++)
        {
            l_cc[u2] = wt[u2];
            *l_nz += wt[u2];
        }
        for (u2 = 0; u2 < n; u2++)
        {
            if (parent[u2] != n)
            {
                l_cc[parent[u2]] += l_cc[u2];
                *l_nz += l_cc[u2];
            }
        }

        /* free scrtach vectors                              */

        if (!l_colcount)
        {
            FREE(l_cc);
        }
        if (!l_rowcount)
        {
            FREE(l_rc);
        }

        /* free other data structures */

        FREE(postorder);
        FREE(ipostorder);
        FREE(wt);
        FREE(level);
        FREE(prev_p);

#ifdef GILBERT_NG_PEYTON_ANALYSIS_SUP
        FREE(prev_nbr);
        FREE(first_descendant);
#endif
    }

    FREE(uf);
    return 0;
}


int
taucs_ccs_etree_liu(taucs_ccs_matrix* A,
                    int* parent,
                    int* l_colcount,
                    int* l_rowcount,
                    int* l_nnz)
{
    int n = A->n;
    int i, j, k, ip, kp;
    int nnz, jnnz;

    int* uf;
    int* rowptr;
    int* colind;

    int* rowcount;
    int* marker;
    int* realroot;

    int* l_cc;
    int* l_rc;

    /* we need the row structures for the lower triangle */

    nnz = (A->colptr)[n];

    uf       = (int*)MALLOC(n     * sizeof(int));
    rowcount = (int*)MALLOC((n + 1) * sizeof(int));
    rowptr   = (int*)MALLOC((n + 1) * sizeof(int));
    colind   = (int*)MALLOC(nnz   * sizeof(int));

    for (i = 0; i <= n; i++)
    {
        rowcount[i] = 0;
    }

    for (j = 0; j < n; j++)
    {

        jnnz = (A->colptr)[j + 1] - (A->colptr)[j];

        for (ip = 0; ip < jnnz; ip++)
        {
            i = (A->rowind)[(A->colptr)[j] + ip];
            if (j < i)
            {
                rowcount[i]++;
            }
        }
    }

    ip = 0;
    for (i = 0; i <= n; i++)
    {
        int next_ip = ip + rowcount[i];
        rowcount[i] = ip;
        rowptr  [i] = ip;
        ip = next_ip;
    }

    for (j = 0; j < n; j++)
    {
        jnnz = (A->colptr)[j + 1] - (A->colptr)[j];

        for (ip = 0; ip < jnnz; ip++)
        {
            i = (A->rowind)[(A->colptr)[j] + ip];
            if (i == j)
            {
                continue;
            }
            assert( rowcount[i] < rowptr[i + 1] );
            colind[ rowcount[i] ] = j;
            rowcount[i]++;
        }
    }

    /* now compute the etree */

    {
        int u, t, vroot;
        realroot = rowcount; /* reuse space */

        for (i = 0; i < n; i++)
        {
            uf_makeset(uf, i);
            realroot[i] = i;
            parent[i] = n;
            vroot = i;
            for (kp = rowptr[i]; kp < rowptr[i + 1]; kp++)
            {
                k = colind[kp];
                u = uf_find(uf, k);
                t = realroot[u];
                if (parent[t] == n && t != i)
                {
                    parent[t] = i;
                    vroot = uf_union(uf, vroot, u);
                    realroot[vroot] = i;
                }
            }
        }
    }

    /* compute column counts */

    if (l_colcount || l_rowcount || l_nnz)
    {
        int* l_nz;
        int  tmp;

        /* we allocate scratch vectors to avoid conditionals */
        /* in the inner loop.                                */

        if (l_colcount)
        {
            l_cc = l_colcount;
        }
        else
        {
            l_cc = (int*) MALLOC(n * sizeof(int));
        }
        if (l_rowcount)
        {
            l_rc = l_rowcount;
        }
        else
        {
            l_rc = (int*) MALLOC(n * sizeof(int));
        }
        if (l_nnz)
        {
            l_nz = l_nnz;
        }
        else
        {
            l_nz = &tmp;
        }

        marker = rowcount; /* we reuse the space */

        for (j = 0; j < n; j++)
        {
            l_cc[j] = 1;
        }
        *l_nz = n;

        for (i = 0; i < n; i++)
        {
            marker[i] = n;    /* clear the array */
        }

        for (i = 0; i < n; i++)
        {
            l_rc[i] = 1;
            marker[ i ] = i;
            for (kp = rowptr[i]; kp < rowptr[i + 1]; kp++)
            {
                k = colind[kp];
                j = k;
                while (marker[j] != i)
                {
                    l_cc[j]++;
                    l_rc[i]++;
                    (*l_nz)++;
                    marker[j] = i;
                    j = parent[j];
                }
            }
        }

        /* free scrtach vectors                              */

        if (!l_colcount)
        {
            FREE(l_cc);
        }
        if (!l_rowcount)
        {
            FREE(l_rc);
        }
    }

    FREE(colind);
    FREE(rowptr);
    FREE(rowcount);
    FREE(uf);
    return 0;
}

static void
recursive_symbolic_elimination(int            j,
                               taucs_ccs_matrix* A,
                               int            first_child[],
                               int            next_child[],
                               int*           n_sn,
                               int            sn_size[],
                               int            sn_up_size[],
                               int*           sn_rowind[],
                               int            sn_first_child[],
                               int            sn_next_child[],
                               int            rowind[],
                               int            column_to_sn_map[],
                               int            map[],
                               int            do_order,
                               int            ipostorder[]
                              )
{
    int  i, ip, c, c_sn;
    int  in_previous_sn;
    int  nnz;

    for (c = first_child[j]; c != -1; c = next_child[c])
    {
        recursive_symbolic_elimination(c, A,
                                       first_child, next_child,
                                       n_sn,
                                       sn_size, sn_up_size, sn_rowind,
                                       sn_first_child, sn_next_child,
                                       rowind, /* temporary */
                                       column_to_sn_map,
                                       map,
                                       do_order, ipostorder
                                      );
    }

    in_previous_sn = 1;
    if (j == A->n)
    {
        in_previous_sn = 0;    /* this is not a real column */
    }
    else if (first_child[j] == -1)
    {
        in_previous_sn = 0;    /* this is a leaf */
    }
    else if (next_child[first_child[j]] != -1)
    {
        in_previous_sn = 0;    /* more than 1 child */
    }
    else
    {
        /* check that the structure is nested */
        /* map contains child markers         */

        c = first_child[j];
        for (ip = (A->colptr)[j]; ip < (A->colptr)[j + 1]; ip++)
        {
            i = (A->rowind)[ip];
            in_previous_sn = in_previous_sn && (map[i] == c);
        }
    }

    if (in_previous_sn)
    {
        c = first_child[j];
        c_sn = column_to_sn_map[c];
        column_to_sn_map[j] = c_sn;

        /* swap row indices so j is at the end of the */
        /* supernode, not in the update indices       */
        for (ip = sn_size[c_sn]; ip < sn_up_size[c_sn]; ip++)
            if (sn_rowind[c_sn][ip] == j)
            {
                break;
            }
        assert(ip < sn_up_size[c_sn]);
        sn_rowind[c_sn][ip] = sn_rowind[c_sn][sn_size[c_sn]];
        sn_rowind[c_sn][sn_size[c_sn]] = j;

        /* mark the nonzeros in the map */
        for (ip = sn_size[c_sn]; ip < sn_up_size[c_sn]; ip++)
        {
            map[ sn_rowind[c_sn][ip] ] = j;
        }

        sn_size   [c_sn]++;

        return;
    }

    /* we are in a new supernode */

    if (j < A->n)
    {
        nnz = 1;
        rowind[0] = j;
        map[j]    = j;

        for (c = first_child[j]; c != -1; c = next_child[c])
        {
            c_sn = column_to_sn_map[c];
            for (ip = sn_size[c_sn]; ip < sn_up_size[c_sn]; ip++)
            {
                i = sn_rowind[c_sn][ip];
                if (i > j && map[i] != j)   /* new row index */
                {
                    map[i] = j;
                    rowind[nnz] = i;
                    nnz++;
                }
            }
        }

        for (ip = (A->colptr)[j]; ip < (A->colptr)[j + 1]; ip++)
        {
            i = (A->rowind)[ip];
            if (map[i] != j)   /* new row index */
            {
                map[i] = j;
                rowind[nnz] = i;
                nnz++;
            }
        }
    }

    /*printf("children of sn %d: ",*n_sn);*/
    for (c = first_child[j]; c != -1; c = next_child[c])
    {
        c_sn = column_to_sn_map[c];
        /*printf("%d ",c_sn);*/
        if (c == first_child[j])
        {
            sn_first_child[*n_sn] = c_sn;
        }
        else
        {
            sn_next_child[ c_sn ] = sn_first_child[*n_sn];
            sn_first_child[*n_sn] = c_sn;
        }
    }
    /*printf("\n");*/

    if (j < A->n)
    {
        column_to_sn_map[j] = *n_sn;
        sn_size   [*n_sn] = 1;
        sn_up_size[*n_sn] = nnz;
        sn_rowind [*n_sn] = (int*) MALLOC(nnz * sizeof(int));
        for (ip = 0; ip < nnz; ip++)
        {
            sn_rowind[*n_sn][ip] = rowind[ip];
        }
        if (do_order)
        {
            /* Sivan and Vladimir: we think that we can sort in */
            /* column order, not only in etree postorder.       */
            /*
            radix_sort(sn_rowind [*n_sn],nnz);
            qsort(sn_rowind [*n_sn],nnz,sizeof(int),compare_ints);
                 */
            compare_indirect_map = ipostorder;
            qsort(sn_rowind [*n_sn], nnz, sizeof(int), compare_indirect_ints);
        }
        assert(sn_rowind [*n_sn][0] == j);
        (*n_sn)++;
    }

    return;
}

/* count zeros and nonzeros in a supernode to compute the */
/* utility of merging fundamental supernodes.             */

typedef struct
{
    double zeros;
    double nonzeros;
} znz;

static znz
recursive_amalgamate_supernodes(int           sn,
                                int*           n_sn,
                                int            sn_size[],
                                int            sn_up_size[],
                                int*           sn_rowind[],
                                int            sn_first_child[],
                                int            sn_next_child[],
                                int            rowind[],
                                int            column_to_sn_map[],
                                int            map[],
                                int            do_order,
                                int            ipostorder[]
                               )
{
    int  i, ip, c_sn, gc_sn;
    int  nnz;
    int  nchildren; /* number of children, child index */
    znz* c_znz;
    znz  sn_znz, merged_znz;

    int new_sn_size, new_sn_up_size;

    sn_znz.zeros    = 0.0;
    sn_znz.nonzeros = (double) (((sn_up_size[sn] - sn_size[sn]) * sn_size[sn])
                                + (sn_size[sn] * (sn_size[sn] + 1)) / 2);

    if (sn_first_child[sn] == -1)   /* leaf */
    {
        return sn_znz;
    }

    nchildren = 0;
    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        nchildren++;
    }

    c_znz = (znz*) alloca(nchildren * sizeof(znz));

    //printf("supernode %d out of %d\n",sn,*n_sn);

    /* merge the supernode with its children! */

    i = 0;
    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        c_znz[i] =
            recursive_amalgamate_supernodes(c_sn,
                                            n_sn,
                                            sn_size, sn_up_size, sn_rowind,
                                            sn_first_child, sn_next_child,
                                            rowind, /* temporary */
                                            column_to_sn_map,
                                            map,
                                            do_order, ipostorder
                                           );
        assert(c_znz[i].zeros + c_znz[i].nonzeros ==
               (double) (((sn_up_size[c_sn] - sn_size[c_sn]) * sn_size[c_sn])
                         + (sn_size[c_sn] * (sn_size[c_sn] + 1)) / 2 ));
        i++;
    }

    merged_znz.nonzeros = sn_znz.nonzeros;
    merged_znz.zeros    = sn_znz.zeros;

    for (i = 0; i < nchildren; i++)
    {
        merged_znz.nonzeros += (c_znz[i]).nonzeros;
        merged_znz.zeros    += (c_znz[i]).zeros;
    }

    /*  printf("supernode %d out of %d (continuing)\n",sn,*n_sn);*/

    /* should we merge the supernode with its children? */

    nnz = 0;
    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        for (ip = 0; ip < sn_size[c_sn]; ip++)
        {
            i = sn_rowind[c_sn][ip];
            assert( map[i] != sn );
            map[i] = sn;
            rowind[nnz] = i;
            nnz++;
        }
    }

    for (ip = 0; ip < sn_size[sn]; ip++)
    {
        i = sn_rowind[sn][ip];
        assert( map[i] != sn );
        map[i] = sn;
        rowind[nnz] = i;
        nnz++;
    }

    new_sn_size = nnz;

    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        for (ip = sn_size[c_sn]; ip < sn_up_size[c_sn]; ip++)
        {
            i = sn_rowind[c_sn][ip];
            if (map[i] != sn)   /* new row index */
            {
                map[i] = sn;
                rowind[nnz] = i;
                nnz++;
            }
        }
    }

    for (ip = sn_size[sn]; ip < sn_up_size[sn]; ip++)
    {
        i = sn_rowind[sn][ip];
        if (map[i] != sn)   /* new row index */
        {
            map[i] = sn;
            rowind[nnz] = i;
            nnz++;
        }
    }

    new_sn_up_size = nnz;

    if (do_order)
    {
        compare_indirect_map = ipostorder;
        qsort(rowind, nnz, sizeof(int), compare_indirect_ints);
    }

    /* determine whether we should merge the supernode and its children */

    {
        int n;
        double* zcount;

        n = 0;
        for (ip = 0; ip < nnz; ip++)
        {
            i = rowind[ip];
            if (i >= n)
            {
                n = i + 1;
            }
        }

        zcount = (double*) malloc(n * sizeof(double));
        assert(zcount);

        for (ip = 0; ip < new_sn_size; ip++)
        {
            i = rowind[ip];
            assert(i < n);
            zcount[i] = (double) (ip + 1);
        }
        for (ip = new_sn_size; ip < new_sn_up_size; ip++)
        {
            i = rowind[ip];
            assert(i < n);
            zcount[i] = (double) new_sn_size;
        }

        /*
        for (ip=0; ip<new_sn_up_size; ip++)
          printf("row %d zcount = %.0f\n",rowind[ip],zcount[rowind[ip]]);
        */

        for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
        {
            for (ip = 0; ip < sn_size[c_sn]; ip++)
            {
                i = sn_rowind[c_sn][ip];
                assert(i < n);
                zcount[i] -= (double) (ip + 1);
            }
            for (ip = sn_size[c_sn]; ip < sn_up_size[c_sn]; ip++)
            {
                i = sn_rowind[c_sn][ip];
                assert(i < n);
                zcount[i] -= (double) sn_size[c_sn];
            }
        }

        for (ip = 0; ip < sn_size[sn]; ip++)
        {
            i = sn_rowind[sn][ip];
            assert(i < n);
            zcount[i] -= (double) (ip + 1);
        }
        for (ip = sn_size[sn]; ip < sn_up_size[sn]; ip++)
        {
            i = sn_rowind[sn][ip];
            assert(i < n);
            zcount[i] -= (double) sn_size[sn];
        }

        /*
        for (ip=0; ip<new_sn_up_size; ip++)
          printf("ROW %d zcount = %.0f\n",rowind[ip],zcount[rowind[ip]]);
        printf("zeros before merging %.0f\n",merged_znz.zeros);
        */

        for (ip = 0; ip < new_sn_up_size; ip++)
        {
            i = rowind[ip];
            assert(i < n);
            assert(zcount[i] >= 0.0);
            merged_znz.zeros += zcount[i];
        }
        FREE( zcount );
        /*printf("zeros after merging %.0f\n",merged_znz.zeros);*/

        /* voodoo constants (need some kind of a utility function */
        if ((new_sn_size < 16)
                ||
                ((sn_size[sn] < 50) && (merged_znz.zeros < 0.5 * merged_znz.nonzeros))
                ||
                ((sn_size[sn] < 250) && (merged_znz.zeros < 0.25 * merged_znz.nonzeros))
                ||
                ((sn_size[sn] < 500) && (merged_znz.zeros < 0.10 * merged_znz.nonzeros))
                ||
                (merged_znz.zeros < 0.05 * merged_znz.nonzeros)
           )
        {
            /*
            taucs_printf("merging sn %d, zeros (%f) vs nonzeros (%f)\n",
               sn,merged_znz.zeros,merged_znz.nonzeros);
            */
        }
        else
        {
            /*
            taucs_printf("sn %d, too many zeros (%f) vs nonzeros (%f)\n",
               sn,merged_znz.zeros,merged_znz.nonzeros);
            printf("returning without merging\n");
            */
            return sn_znz;
        }
    }

    /* now merge the children lists */

    sn_size[sn]    = new_sn_size;
    sn_up_size[sn] = new_sn_up_size;
    sn_rowind[sn]  = (int*) REALLOC(sn_rowind[sn],
                                    new_sn_up_size * sizeof(int));
    for (ip = 0; ip < new_sn_up_size; ip++)
    {
        sn_rowind[sn][ip] = rowind[ip];
    }

    //  printf("supernode %d out of %d (merging)\n",sn,*n_sn);

    nchildren = 0;
    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        for (ip = 0; ip < sn_size[c_sn]; ip++)
        {
            i = (sn_rowind[c_sn])[ip];
            assert(column_to_sn_map[i] == c_sn);
            column_to_sn_map[i] = sn;
        }

        for (gc_sn = sn_first_child[c_sn]; gc_sn != -1; gc_sn = sn_next_child[gc_sn])
        {
            rowind[nchildren] = gc_sn;
            nchildren++;
        }
    }

    /* free the children's rowind vectors */
    for (c_sn = sn_first_child[sn]; c_sn != -1; c_sn = sn_next_child[c_sn])
    {
        FREE( sn_rowind[c_sn] );
        sn_rowind[c_sn]  = NULL;
        sn_size[c_sn]    = 0;
        sn_up_size[c_sn] = 0;
    }

    sn_first_child[sn] = -1;
    for (i = 0; i < nchildren; i++)
    {
        sn_next_child[ rowind[i] ] = sn_first_child[sn];
        sn_first_child[sn] = rowind[i];
    }

    /*
    printf("supernode %d out of %d (done)\n",sn,*n_sn);
    printf("returning, merging\n");
    */
    return merged_znz;
}

int
taucs_ccs_symbolic_elimination(taucs_ccs_matrix* A,
                               void* vL,
                               int do_order
                              )
{
    supernodal_factor_matrix* L = (supernodal_factor_matrix*) vL;
    int* first_child;
    int* next_child;
    int j;
    int* column_to_sn_map;
    int* map;
    int* rowind;
    int* parent;
    int* ipostorder;

    L->n         = A->n;
    L->sn_struct = (int**)MALLOC((A->n  ) * sizeof(int*));
    L->sn_size   = (int*) MALLOC((A->n + 1) * sizeof(int));

    L->sn_up_size   = (int*) MALLOC((A->n + 1) * sizeof(int));
    L->first_child = (int*) MALLOC((A->n + 1) * sizeof(int));
    L->next_child  = (int*) MALLOC((A->n + 1) * sizeof(int));

    column_to_sn_map = (int*)MALLOC((A->n + 1) * sizeof(int));
    map              = (int*) MALLOC((A->n + 1) * sizeof(int));

    first_child = (int*) MALLOC(((A->n) + 1) * sizeof(int));
    next_child  = (int*) MALLOC(((A->n) + 1) * sizeof(int));

    rowind      = (int*) MALLOC((A->n) * sizeof(int));

    /* compute the vertex elimination tree */
    parent      = (int*)MALLOC((A->n + 1) * sizeof(int));

    taucs_ccs_etree(A, parent, NULL, NULL, NULL);

    if (0)
    {
        int *cc1, *cc2, *rc1, *rc2;
        int *p1;
        int nnz1, nnz2;

        cc1 = (int*)MALLOC((A->n) * sizeof(int));
        cc2 = (int*)MALLOC((A->n) * sizeof(int));
        rc1 = (int*)MALLOC((A->n) * sizeof(int));
        rc2 = (int*)MALLOC((A->n) * sizeof(int));
        p1 = (int*)MALLOC((A->n) * sizeof(int));

        taucs_ccs_etree_liu(A, parent, cc1, rc1, &nnz1);

        taucs_ccs_etree(A, p1, cc2, rc2, &nnz2);

        for (j = 0; j < (A->n); j++)
        {
            assert(parent[j] == p1[j]);
        }
        for (j = 0; j < (A->n); j++)
        {
            if (cc1[j] != cc2[j])
            {
                sciprint("j=%d cc1=%d cc2=%d\n", j, cc1[j], cc2[j]);
            }
            assert(cc1[j] == cc2[j]);
        }

        for (j = 0; j < (A->n); j++)
        {
            if (rc1[j] != rc2[j])
            {
                sciprint("j=%d rc1=%d rc2=%d\n", j, rc1[j], rc2[j]);
            }
            assert(rc1[j] == rc2[j]);
        }

        if (nnz1 != nnz2)
        {
            sciprint("nnz1=%d nnz2=%d\n", nnz1, nnz2);
        }

        FREE(cc1);
        FREE(cc2);
        FREE(rc1);
        FREE(rc2);
    }

    for (j = 0; j <= (A->n); j++)
    {
        first_child[j] = -1;
    }
    for (j = (A->n) - 1; j >= 0; j--)
    {
        int p = parent[j];
        next_child[j] = first_child[p];
        first_child[p] = j;
    }
    FREE(parent);

    ipostorder = (int*)MALLOC((A->n + 1) * sizeof(int));
    {
        int next = 0;
        /*int* postorder = (int*)MALLOC((A->n+1)*sizeof(int));*/
        recursive_postorder(A->n, first_child, next_child,
                            NULL,
                            ipostorder, &next);
        /*
        printf("ipostorder ");
        for (j=0; j <= (A->n); j++) printf("%d ",ipostorder[j]);
        printf("\n");
        printf(" postorder ");
        for (j=0; j <= (A->n); j++) printf("%d ",postorder[j]);
        printf("\n");
        */
    }

    L->n_sn = 0;
    for (j = 0; j < (A->n); j++)
    {
        map[j] = -1;
    }
    for (j = 0; j <= (A->n); j++)
    {
        (L->first_child)[j] = (L->next_child)[j] = -1;
    }

    recursive_symbolic_elimination(A->n,
                                   A,
                                   first_child, next_child,
                                   &(L->n_sn),
                                   L->sn_size, L->sn_up_size, L->sn_struct,
                                   L->first_child, L->next_child,
                                   rowind,
                                   column_to_sn_map,
                                   map,
                                   do_order, ipostorder
                                  );

    {
        double nnz   = 0.0;
        double flops = 0.0;
        int sn, i, colnnz;
        for (sn = 0; sn < (L->n_sn); sn++)
        {
            for (i = 0, colnnz = (L->sn_up_size)[sn];
                    i < (L->sn_size)[sn];
                    i++, colnnz--)
            {
                flops += ((double)(colnnz) - 1.0) * ((double)(colnnz) + 2.0) / 2.0;
                nnz   += (double) (colnnz);
            }
        }
        /*sciprint("\t\tSymbolic Analysis of CC^T: %.2e nonzeros, %.2e flops\n",
          nnz, flops); */
    }

    for (j = 0; j < (A->n); j++)
    {
        map[j] = -1;
    }

    (void) recursive_amalgamate_supernodes((L->n_sn) - 1,
                                           &(L->n_sn),
                                           L->sn_size, L->sn_up_size, L->sn_struct,
                                           L->first_child, L->next_child,
                                           rowind,
                                           column_to_sn_map,
                                           map,
                                           do_order, ipostorder
                                          );


    {
        double nnz   = 0.0;
        double flops = 0.0;
        int sn, i, colnnz;
        for (sn = 0; sn < (L->n_sn); sn++)
        {
            for (i = 0, colnnz = (L->sn_up_size)[sn];
                    i < (L->sn_size)[sn];
                    i++, colnnz--)
            {
                flops += ((double)(colnnz) - 1.0) * ((double)(colnnz) + 2.0) / 2.0;
                nnz   += (double) (colnnz);
            }
        }
        /*sciprint("\t\tRelaxed  Analysis of LL^T: %.2e nonzeros, %.2e flops\n",
          nnz, flops); */
    }

    /*
    {
      int i;
      printf("c2sn: ");
      for (i=0; i<A->n; i++) printf("%d ",column_to_sn_map[i]);
      printf("\n");
    }
    */



    L->sn_blocks_ld  = (int*)MALLOC((L->n_sn) * sizeof(int));
    L->sn_blocks     = (double**)CALLOC((L->n_sn), sizeof(double*)); /* so we can free before allocation */

    L->up_blocks_ld  = (int*)MALLOC((L->n_sn) * sizeof(int));
    L->up_blocks     = (double**)CALLOC((L->n_sn), sizeof(double*));

    FREE(rowind);
    FREE(map);
    FREE(column_to_sn_map);
    FREE(next_child);
    FREE(first_child);
    FREE(ipostorder);

    return 0;
}


/*************************************************************/
/* factor routines                                           */
/*************************************************************/

static supernodal_frontal_matrix*
recursive_multifrontal_supernodal_factor_llt(int sn,       /* this supernode */
        int is_root,  /* is v the root? */
        int* bitmap,
        taucs_ccs_matrix* A,
        supernodal_factor_matrix* snL,
        int* fail)
{
    supernodal_frontal_matrix* my_matrix = NULL;
    supernodal_frontal_matrix* child_matrix = NULL;
    int child;
    int* v;
    int  sn_size;
    int* first_child   = snL->first_child;
    int* next_child    = snL->next_child;

    v = &( snL->sn_struct[sn][0] );

    if (!is_root)
    {
        sn_size = snL->sn_size[sn];
    }
    else
    {
        sn_size = -1;
    }

    for (child = first_child[sn]; child != -1; child = next_child[child])
    {
        child_matrix =
            recursive_multifrontal_supernodal_factor_llt(child,
                    FALSE,
                    bitmap,
                    A, snL, fail);
        if (*fail)
        {
            if (child_matrix)
            {
                supernodal_frontal_free(child_matrix);
            }
            return NULL;
        }

        if (!is_root)
        {
            if (!my_matrix)
            {
                my_matrix =  supernodal_frontal_create(v, sn_size,
                                                       snL->sn_up_size[sn],
                                                       snL->sn_struct[sn]);
                if (!my_matrix)
                {
                    *fail = TRUE;
                    supernodal_frontal_free(child_matrix);
                    return NULL;
                }
            }

            multifrontal_supernodal_front_extend_add(my_matrix, child_matrix, bitmap);
            supernodal_frontal_free(child_matrix);
        }
    }

    /* in case we have no children, we allocate now */
    if (!is_root && !my_matrix)
        my_matrix =  supernodal_frontal_create(v, sn_size,
                                               snL->sn_up_size[sn],
                                               snL->sn_struct[sn]);

    if (!is_root)
    {
        if (multifrontal_supernodal_front_factor(sn,
                v, sn_size,
                A,
                my_matrix,
                bitmap,
                snL))
        {
            /* nonpositive pivot */
            *fail = TRUE;
            supernodal_frontal_free(my_matrix);
            return NULL;
        }
    }
    return my_matrix;
}

void* taucs_ccs_factor_llt_mf(taucs_ccs_matrix* A)
{
    supernodal_factor_matrix* L;
    int* map;
    int fail;

    L = multifrontal_supernodal_create();

    taucs_ccs_symbolic_elimination(A, L,
                                   FALSE /* don't sort row indices */ );

    map = (int*)MALLOC((A->n + 1) * sizeof(int));

    fail = FALSE;
    recursive_multifrontal_supernodal_factor_llt((L->n_sn),
            TRUE,
            map,
            A, L, &fail);

    FREE(map);

    if (fail)
    {
        taucs_supernodal_factor_free(L);
        return NULL;
    }

    return (void*) L;
}


/*************************************************************/
/* supernodal solve routines                                 */
/*************************************************************/

static void
recursive_supernodal_solve_l(int sn,       /* this supernode */
                             int is_root,  /* is v the root? */
                             int* first_child, int* next_child,
                             int** sn_struct, int* sn_sizes, int* sn_up_sizes,
                             int* sn_blocks_ld, double* sn_blocks[],
                             int* up_blocks_ld, double* up_blocks[],
                             double x[], double b[],
                             double t[])
{
    int child;
    int  sn_size; /* number of rows/columns in the supernode    */
    int  up_size; /* number of rows that this supernode updates */
    int    ione = 1;
    double done = 1.0;
    double dzero = 0.0;
    double* xdense;
    double* bdense;
    double  flops;
    int i, j, ip, jp;

    for (child = first_child[sn]; child != -1; child = next_child[child])
    {
        recursive_supernodal_solve_l(child,
                                     FALSE,
                                     first_child, next_child,
                                     sn_struct, sn_sizes, sn_up_sizes,
                                     sn_blocks_ld, sn_blocks,
                                     up_blocks_ld, up_blocks,
                                     x, b, t);
    }

    if (!is_root)
    {

        sn_size = sn_sizes[sn];
        up_size = sn_up_sizes[sn] - sn_sizes[sn];

        flops = ((double)sn_size) * ((double)sn_size)
                + 2.0 * ((double)sn_size) * ((double)up_size);

        if (flops > BLAS_FLOPS_CUTOFF)
        {
            xdense = t;
            bdense = t + sn_size;

            for (i = 0; i < sn_size; i++)
            {
                xdense[i] = b[ sn_struct[ sn ][ i ] ];
            }
            for (i = 0; i < up_size; i++)
            {
                bdense[i] = 0.0;
            }

            F2C(dtrsm)("Left",
                       "Lower",
                       "No Transpose",
                       "No unit diagonal",
                       &sn_size, &ione,
                       &done,
                       sn_blocks[sn], &(sn_blocks_ld[sn]),
                       xdense       , &sn_size);

            if ((up_size > 0) & (sn_size > 0))
            {
                F2C(dgemm)("No Transpose", "No Transpose",
                           &up_size, &ione, &sn_size,
                           &done,
                           up_blocks[sn], &(up_blocks_ld[sn]),
                           xdense       , &sn_size,
                           &dzero,
                           bdense       , &up_size);
            }

            for (i = 0; i < sn_size; i++)
            {
                x[ sn_struct[ sn][ i ] ]  = xdense[i];
            }
            for (i = 0; i < up_size; i++)
            {
                b[ sn_struct[ sn ][ sn_size + i ] ] -= bdense[i];
            }

        }
        else if (sn_size > SOLVE_DENSE_CUTOFF)
        {

            xdense = t;
            bdense = t + sn_size;

            for (i = 0; i < sn_size; i++)
            {
                xdense[i] = b[ sn_struct[ sn ][ i ] ];
            }
            for (i = 0; i < up_size; i++)
            {
                bdense[i] = 0.0;
            }

            for (jp = 0; jp < sn_size; jp++)
            {
                xdense[jp] = xdense[jp] / sn_blocks[sn][ sn_blocks_ld[sn] * jp + jp];

                for (ip = jp + 1; ip < sn_size; ip++)
                {
                    xdense[ip] -= xdense[jp] * sn_blocks[sn][ sn_blocks_ld[sn] * jp + ip];
                }
            }

            for (jp = 0; jp < sn_size; jp++)
            {
                for (ip = 0; ip < up_size; ip++)
                {
                    bdense[ip] += xdense[jp] * up_blocks[sn][ up_blocks_ld[sn] * jp + ip];
                }
            }

            for (i = 0; i < sn_size; i++)
            {
                x[ sn_struct[ sn][ i ] ]  = xdense[i];
            }
            for (i = 0; i < up_size; i++)
            {
                b[ sn_struct[ sn ][ sn_size + i ] ] -= bdense[i];
            }

        }
        else
        {

            for (jp = 0; jp < sn_size; jp++)
            {
                j = sn_struct[sn][jp];
                x[j] = b[j] / sn_blocks[sn][ sn_blocks_ld[sn] * jp + jp];
                for (ip = jp + 1; ip < sn_size; ip++)
                {
                    i = sn_struct[sn][ip];
                    b[i] -= x[j] * sn_blocks[sn][ sn_blocks_ld[sn] * jp + ip];
                }

                for (ip = 0; ip < up_size; ip++)
                {
                    i = sn_struct[sn][sn_size + ip];
                    b[i] -= x[j] * up_blocks[sn][ up_blocks_ld[sn] * jp + ip];
                }
            }

        }
    }
}

static void
recursive_supernodal_solve_lt(int sn,       /* this supernode */
                              int is_root,  /* is v the root? */
                              int* first_child, int* next_child,
                              int** sn_struct, int* sn_sizes, int* sn_up_sizes,
                              int* sn_blocks_ld, double* sn_blocks[],
                              int* up_blocks_ld, double* up_blocks[],
                              double x[], double b[],
                              double t[])
{
    int child;
    int  sn_size; /* number of rows/columns in the supernode    */
    int  up_size; /* number of rows that this supernode updates */
    int    ione = 1;
    double done = 1.0;
    double dminusone = -1.0;
    double* xdense;
    double* bdense;
    double  flops;
    int i, j, ip, jp;

    if (!is_root)
    {

        sn_size = sn_sizes[sn];
        up_size = sn_up_sizes[sn] - sn_sizes[sn];

        flops = ((double)sn_size) * ((double)sn_size)
                + 2.0 * ((double)sn_size) * ((double)up_size);

        if (flops > BLAS_FLOPS_CUTOFF)
        {

            bdense = t;
            xdense = t + sn_size;

            for (i = 0; i < sn_size; i++)
            {
                bdense[i] = b[ sn_struct[ sn][ i ] ];
            }
            for (i = 0; i < up_size; i++)
            {
                xdense[i] = x[ sn_struct[sn][sn_size + i] ];
            }

            if ((up_size > 0) & (sn_size > 0))
                F2C(dgemm)("Transpose", "No Transpose",
                           &sn_size, &ione, &up_size,
                           &dminusone,
                           up_blocks[sn], &(up_blocks_ld[sn]),
                           xdense       , &up_size,
                           &done,
                           bdense       , &sn_size);

            F2C(dtrsm)("Left",
                       "Lower",
                       "Transpose",
                       "No unit diagonal",
                       &sn_size, &ione,
                       &done,
                       sn_blocks[sn], &(sn_blocks_ld[sn]),
                       bdense       , &sn_size);

            for (i = 0; i < sn_size; i++)
            {
                x[ sn_struct[ sn][ i ] ]  = bdense[i];
            }

        }
        else if (sn_size > SOLVE_DENSE_CUTOFF)
        {

            bdense = t;
            xdense = t + sn_size;

            for (i = 0; i < sn_size; i++)
            {
                bdense[i] = b[ sn_struct[ sn][ i ] ];
            }
            for (i = 0; i < up_size; i++)
            {
                xdense[i] = x[ sn_struct[sn][sn_size + i] ];
            }

            for (ip = sn_size - 1; ip >= 0; ip--)
            {
                for (jp = 0; jp < up_size; jp++)
                {
                    bdense[ip] -= xdense[jp] * up_blocks[sn][ up_blocks_ld[sn] * ip + jp];
                }
            }

            for (ip = sn_size - 1; ip >= 0; ip--)
            {
                for (jp = sn_size - 1; jp > ip; jp--)
                {
                    bdense[ip] -= bdense[jp] * sn_blocks[sn][ sn_blocks_ld[sn] * ip + jp];
                }
                bdense[ip] = bdense[ip] / sn_blocks[sn][ sn_blocks_ld[sn] * ip + ip];
            }

            for (i = 0; i < sn_size; i++)
            {
                x[ sn_struct[ sn][ i ] ]  = bdense[i];
            }

        }
        else
        {

            for (ip = sn_size - 1; ip >= 0; ip--)
            {
                i = sn_struct[sn][ip];

                for (jp = 0; jp < up_size; jp++)
                {
                    j = sn_struct[sn][sn_size + jp];
                    b[i] -= x[j] * up_blocks[sn][ up_blocks_ld[sn] * ip + jp];
                }

                for (jp = sn_size - 1; jp > ip; jp--)
                {
                    j = sn_struct[sn][jp];
                    b[i] -= x[j] * sn_blocks[sn][ sn_blocks_ld[sn] * ip + jp];
                }
                x[i] = b[i] / sn_blocks[sn][ sn_blocks_ld[sn] * ip + ip];

            }

        }
    }

    for (child = first_child[sn]; child != -1; child = next_child[child])
    {
        recursive_supernodal_solve_lt(child,
                                      FALSE,
                                      first_child, next_child,
                                      sn_struct, sn_sizes, sn_up_sizes,
                                      sn_blocks_ld, sn_blocks,
                                      up_blocks_ld, up_blocks,
                                      x, b, t);
    }
}

int taucs_supernodal_solve_llt(void* vL,
                               double* x, double* b)
{
    supernodal_factor_matrix* L = (supernodal_factor_matrix*) vL;
    double* y;
    double* t; /* temporary vector */
    int     i;

    y = MALLOC((L->n) * sizeof(double));
    t = MALLOC((L->n) * sizeof(double));
    if (!y || !t)
    {
        FREE(y);
        FREE(t);
        sciprint(_("%s: No more memory.\n"), "multifrontal_supernodal_solve_llt");
        return -1;
    }

    for (i = 0; i < L->n; i++)
    {
        x[i] = b[i];
    }

    recursive_supernodal_solve_l (L->n_sn,
                                  TRUE,  /* this is the root */
                                  L->first_child, L->next_child,
                                  L->sn_struct, L->sn_size, L->sn_up_size,
                                  L->sn_blocks_ld, L->sn_blocks,
                                  L->up_blocks_ld, L->up_blocks,
                                  y, x, t);

    recursive_supernodal_solve_lt(L->n_sn,
                                  TRUE,  /* this is the root */
                                  L->first_child, L->next_child,
                                  L->sn_struct, L->sn_size, L->sn_up_size,
                                  L->sn_blocks_ld, L->sn_blocks,
                                  L->up_blocks_ld, L->up_blocks,
                                  x, y, t);

    FREE(y);
    FREE(t);

    return 0;
}

/*****************************************************
 *  some utility routines                            *
 *****************************************************/

taucs_ccs_matrix*
taucs_supernodal_factor_to_ccs(void* vL)
{
    supernodal_factor_matrix* L = (supernodal_factor_matrix*) vL;
    taucs_ccs_matrix* C;
    int n, nnz;
    int i, j, ip, jp, sn, next;
    double v;
    int* len;

    n = L->n;

    len = (int*) MALLOC(n * sizeof(int));
    if (!len)
    {
        return NULL;
    }

    nnz = 0;

    for (sn = 0; sn < L->n_sn; sn++)
    {
        for (jp = 0; jp < (L->sn_size)[sn]; jp++)
        {
            j = (L->sn_struct)[sn][jp];
            len[j] = 0;

            for (ip = jp; ip < (L->sn_size)[sn]; ip++)
            {
                v = (L->sn_blocks)[sn][ jp * (L->sn_blocks_ld)[sn] + ip ];

                if (v)
                {
                    len[j] ++;
                    nnz ++;
                }
            }
            for (ip = (L->sn_size)[sn]; ip < (L->sn_up_size)[sn]; ip++)
            {
                v = (L->up_blocks)[sn][ jp * (L->up_blocks_ld)[sn] + (ip - (L->sn_size)[sn]) ];

                if (v)
                {
                    len[j] ++;
                    nnz ++;
                }
            }
        }
    }


    C = taucs_ccs_create(n, n, nnz);
    if (!C)
    {
        FREE(len);
        return NULL;
    }
    C->flags = TAUCS_TRIANGULAR | TAUCS_LOWER;

    (C->colptr)[0] = 0;
    for (j = 1; j <= n; j++)
    {
        (C->colptr)[j] = (C->colptr)[j - 1] + len[j - 1];
    }

    FREE(len);

    for (sn = 0; sn < L->n_sn; sn++)
    {
        for (jp = 0; jp < (L->sn_size)[sn]; jp++)
        {
            j = (L->sn_struct)[sn][jp];

            next = (C->colptr)[j];

            for (ip = jp; ip < (L->sn_size)[sn]; ip++)
            {
                i = (L->sn_struct)[sn][ ip ];
                v = (L->sn_blocks)[sn][ jp * (L->sn_blocks_ld)[sn] + ip ];

                if (v == 0.0)
                {
                    continue;
                }

                (C->rowind)[next] = i;
                (C->values)[next] = v;
                next++;
            }
            for (ip = (L->sn_size)[sn]; ip < (L->sn_up_size)[sn]; ip++)
            {
                i = (L->sn_struct)[sn][ ip ];
                v = (L->up_blocks)[sn][ jp * (L->up_blocks_ld)[sn] + (ip - (L->sn_size)[sn]) ];

                if (v == 0.0)
                {
                    continue;
                }

                (C->rowind)[next] = i;
                (C->values)[next] = v;
                next++;
            }
        }
    }

    return C;
}

/*
 *   from the preeeding routine here is a small one to get only
 *   the number of non zero elements of the factor (Bruno le 29/11/2001)
 *   In fact the supernodal struct keeps on some exact zeros and so the
 *   nnz get here is representative of the memory used by the supernodal
 *   struc.
 */

int taucs_get_nnz_from_supernodal_factor(void* vL)
{
    supernodal_factor_matrix* L = (supernodal_factor_matrix*) vL;
    int nnz = 0;
    int jp = 0, sn = 0;

    for (sn = 0; sn < L->n_sn; sn++)
        for (jp = 0; jp < (L->sn_size)[sn]; jp++)
        {
            nnz += (L->sn_up_size)[sn] - jp;
        }

    return ( nnz );
}



