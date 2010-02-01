/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project 
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do 
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 */
/*
 * First here are some utilities for the interface : 
 *
 *   1) When a user computes an LU fact with umf_lufact or a Choleski
 *      fact with taucs_chfact he/she get at the scilab level only a 
 *      pointer on to the factorization : the memory for this factorization 
 *      is outside scilab memory. In order to avoid problem with scilab 
 *      pointer coming from other usage I manage in this interface a 
 *      linked list of all the valid pointers onto "umfpack numerical 
 *      factorisation handle" and the same for the Choleski factorizations. 
 *      These 2 lists are called 
 *
 *         ListNumeric
 *         ListCholFactor
 *
 *      And so you will find here 3 routines to deal with :
 *
 *         AddAdrToList, RetrieveAdrFromList, IsAdrInList
 *
 *   2) some others  utility routines are used :
 *
 *         sci_sparse_to_ccs_sparse: scilab sparse format -> CCS format (for umfpack)
 *
 *         spd_sci_sparse_to_taucs_sparse: transform a scilab sparse (supposed to be spd)
 *                                         in the format wait by taucs 
 *
 *         TransposeMatrix: used only to solve x = b/A  (umfpack(b,"/",A)
 *
 *         UmfErrorMes : to deal with some few messages from umfpack
 *
 *         test_size_for_sparse : to see if stk have enough places to 
 *                                store a sparse (see comments)
 *         test_size_for_mat : the same for classic matrix
 */

#include "MALLOC.h"
#include "stack-c.h"
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"

int AddAdrToList(Adr adr, int it_flag, CellAdr **L)
{
  CellAdr *NewCell;
  if ( (NewCell = MALLOC(sizeof(CellAdr))) == NULL )
    return 0;
  else  
    {
      NewCell->adr = adr;
      NewCell->it = it_flag;
      NewCell->next = *L;
      *L = NewCell;
      return 1;
    }
}

int RetrieveAdrFromList(Adr adr, CellAdr **L, int *it_flag)
{
  /* teste si l'adresse adr est presente ds la liste L, si oui
     on la retire et la fonction renvoie 1, sinon 0  */
  CellAdr * Cell;

  if ( *L == NULL )
    return 0;
  else if ( (*L)->adr == adr ) 
    {
      Cell = *L;
      *it_flag = Cell->it;
      *L = (*L)->next;
      FREE(Cell);
      return 1;
    }
  else
    return ( RetrieveAdrFromList(adr, &((*L)->next), it_flag));
}

int IsAdrInList(Adr adr, CellAdr *L, int *it_flag)
{
  /* teste si l'adresse adr est presente ds la liste L, si oui
     la fonction renvoie 1, sinon 0. On renvoit aussi it */

  if ( L == NULL )
    return 0;
  else if ( L->adr == adr ) 
    {
      *it_flag = L->it;
      return 1;
    }
  else
    return ( IsAdrInList(adr, L->next, it_flag) );
}


void TransposeMatrix(double A[], int ma, int na, double At[])
{
  /*  compute At the (na,ma) matrix gotten by the
   *  transposition of the (ma, na) matrix A.  A and
   *  At are in fact simple 1-d arrays with the fortran 
   *  storage convention :
   *     A(i,j) = A[i + ma*j] , At(i,j) = At[i + na*j]
   *
   *     At(i,j) = A(j,i) = A[j + ma*i]
   */
  int i, j;
  for ( j = 0 ; j < ma ; j++ )
    for ( i = 0 ; i < na ; i++ )
      At[i + na*j] = A[j + ma*i];
}

int sci_sparse_to_ccs_sparse(int num, SciSparse *A, CcsSparse *B)
{
  int taille, one=1, nel = A->nel, m = A->m, n = A->n, it = A->it;
  int l0, l1, l2, k, kb, i, j, count;

  taille = ((2*it+3)*nel + n+1 )/2 + 2;

  CreateVar(num, MATRIX_OF_DOUBLE_DATATYPE, &taille, &one, &l0);  /* return 0 in case of failure (not enough memory in stk) */

  B->m = m; B->n = n; B->nel = nel; B->it = it;
  B->R = stk(l0);
  if ( it == 1 )
    {
      B->I = stk(l0 + nel); l1 = l0 + 2*nel;
    }
  else
    {
      B->I = NULL; l1 = l0 + nel;
    }
  B->p = (int *) stk(l1); l2 = l1 + (n+1)/2 + 1;
  B->irow = (int *) stk(l2);

  for ( i = 0 ; i <= n ; i++ )
    B->p[i] = 0;

  for ( k = 0 ; k < nel ; k++ )
    B->p[A->icol[k]]++;   /* this is because  A->icol[k] is 1-based (and not 0-based) */

  for ( i = 2 ; i <= n ; i++ )
    B->p[i] += B->p[i-1];

  k = 0;
  for ( i = 0 ; i < m ; i++ )
    for ( count = 0 ; count < A->mnel[i] ; count++ )
      {
	j = A->icol[k]-1;
	kb = B->p[j];  /* "pointeur" actuel sur la colonne j */
	B->irow[kb] = i;
	B->R[kb] = A->R[k];
	if (it == 1) B->I[kb] = A->I[k];
	B->p[j]++;
	k++;
      }

  for ( i = n-1 ; i > 0 ; i-- )
    B->p[i] = B->p[i-1];
  B->p[0] = 0;

  return 1;
}

char *UmfErrorMes(int num_error)
{
  /* to deal with various umfpack error indicator */
  char *mes1 = _("singular matrix");
  char *mes2 = _("not enough memory");
  char *mes3 = _("internal error");
  char *mes4 = _("invalid matrix");
  /*  normallly with the different controls in the interface
   *  the others errors may not occured but we put anyway
   *  this last one :
   */
  char *mes5 = "unidentified error";

  switch (num_error) {
  case UMFPACK_WARNING_singular_matrix: return(mes1);
  case UMFPACK_ERROR_out_of_memory:   return(mes2);
  case UMFPACK_ERROR_internal_error:  return(mes3);
  case UMFPACK_ERROR_invalid_matrix:  return(mes4);
  default:                            return(mes5);
  };
}

int is_sparse_upper_triangular(SciSparse *A)
{
  int i, k=0, nb_elem_row_i;
  for ( i = 0 ; i < A->m ; i++ )
    {
      nb_elem_row_i = A->mnel[i];
      if (nb_elem_row_i > 0  &&  A->icol[k] <= i)
	return 0; 
      k += nb_elem_row_i;
    }
  return 1;
}

int spd_sci_sparse_to_taucs_sparse(int num, SciSparse *A, taucs_ccs_matrix *B)
{
  /*  
   *  this function create a taucs sparse symmetric matrix (with only the lower
   *  triangle part) from a supposed symmetric spd scilab sparse one's.
   *  This is to put a sci sparse in the format wait by taucs routines
   *
   *  The scilab sparse may be only upper triangular
   */
  int taille, one=1, B_nel, n = A->n;
  int l0, l1, l2, i, k, l, p, nnz;

  if ( A->m != A->n  ||  A->m <= 0  ||  A->it == 1 )
    return ( MAT_IS_NOT_SPD );

  if ( is_sparse_upper_triangular(A) )
    B_nel = A->nel;
  else
    B_nel = n + (A->nel - n)/2;
  taille = (3*B_nel + n+1 )/2 + 2;

  CreateVar(num, MATRIX_OF_DOUBLE_DATATYPE, &taille, &one, &l0);  /* return 0 in case of failure (not enough memory in stk) */

  /* form the taucs sparse matrix struct */
  B->m = n; B->n = n;
  B->flags =  TAUCS_SYMMETRIC | TAUCS_LOWER;

  B->values = stk(l0); l1 = l0 + B_nel;
  B->colptr = (int *) stk(l1); l2 = l1 + (n+1)/2 + 1;
  B->rowind = (int *) stk(l2);

  nnz = 0;
  k = 0;
  for ( i = 0 ; i < n ; i++ )
    {
      if ( A->mnel[i] > 0 )
	{
	  l = 0;
	  while ( l < A->mnel[i]  &&  A->icol[k+l] < i+1 )   /* go to the diagonal element */ 
	    l++;
	  if ( l >= A->mnel[i] )          /* no element A_ij with j >= i => A_ii = 0  */
	    return ( MAT_IS_NOT_SPD );
	  else if ( A->icol[k+l] > i+1 )  /* A_ii = 0 */
	    return ( MAT_IS_NOT_SPD );
	  else if ( A->R[k+l] <= 0 )      /* A_ii <= 0 */
	    return ( MAT_IS_NOT_SPD );
	  else                            /* normal case A_ii > 0 */
	    {        
	      if ( nnz + A->mnel[i] - l > B_nel )    
		return ( MAT_IS_NOT_SPD );
	      B->colptr[i] = nnz;
	      for ( p = l ; p < A->mnel[i] ; p++)
		{
		  B->values[nnz] = A->R[k+p];
		  B->rowind[nnz] = A->icol[k+p]-1;
		  nnz++;
		}
	      k = k + A->mnel[i];
	    }
	}
      else
	return ( MAT_IS_NOT_SPD );
    }
  if ( nnz != B_nel )
    return ( MAT_IS_NOT_SPD );

  B->colptr[n] = nnz;
  
  return ( A_PRIORI_OK );
}


/*------------------------------------------------------*
 * an utility to test if we can create a sparse matrix  *
 * in the scilab stack                                  *
 *------------------------------------------------------*/

int test_size_for_sparse(int pos, int m, int it, int nel, int * pl_miss) 
{
  /*  test if the scilab stack can currently store at the
   *  position pos a sparse matrix with m rows and nel 
   *  non nul elements (Bruno le 17/12/2001 with the help
   *  of jpc). This function is required because with a failure
   *  in a CreateVarFromPtr(pos, "s", ...) the control is then
   *  passed (via Scierror) to the intepretor and we can lose
   *  the pointer and so don't be able to free the associated
   *  memory to this pointer 
   */
 
  int lw = pos + Top - Rhs, il;

  if (lw + 1 >= Bot) return FALSE; /* even no place for a supplementary var */

  /* 5 + m + nel : number of "integers" cases required for the sparse */
  
  il = iadr(*Lstk(lw )) +  5 + m + nel;
  *pl_miss =  (sadr(il) + nel*(it+1)) - *Lstk(Bot); 

  if ( *pl_miss > 0 )
    return FALSE;
  else
    return TRUE;
}

int test_size_for_mat(int pos, int m, int n, int it, int * pl_miss) 
{
  /* the same for classic matrix (trick given par jpc) */
  int lw = pos + Top - Rhs, il;

  if (lw + 1 >= Bot) return FALSE;

  /* 4 is the number of int "cases" required for a classic matrix
   * (type , m, n, it)
   */
  il = iadr(*Lstk(lw )) + 4;

  *pl_miss =  (sadr(il) + m*n*(it+1)) - *Lstk(Bot); 

  if ( *pl_miss > 0 )
    return FALSE;
  else
    return TRUE;
}


void residu_with_prec(SciSparse *A, double x[], double b[], double r[], double *rn)
{
  /*  un essai de calcul de residu : r = Ax - b, en precision double etendue */
  int i, j, k, l;
  long double temp, norm2;

  norm2 = 0.0;
  k = 0;
  for ( i = 0 ; i < A->m ; i++ )
    {
      temp = 0.0;
      for ( l = 0 ; l < A->mnel[i] ; l++ )
	{
	  j = A->icol[k] - 1;
	  temp += (long double) A->R[k]  *  (long double) x[j];
	  k++;
	}
      temp -=  (long double) b[i];
      r[i] = (double) temp;
      norm2 += temp*temp;
    }
  *rn = (double) sqrt(norm2);
  return;
}

void residu_with_prec_for_chol(SciSparse *A, double x[], double b[], double r[], 
				      double *rn, int A_is_upper_triangular, long double wk[])
{
  /*  the same than the previous routine but this one take care of the fact that 
   *  when A_is_upper_triangular=1 only the upper triangle part of A is stored */
  int i, j, k, l;
  long double norm2 = 0.0;

  if ( ! A_is_upper_triangular )
    residu_with_prec(A, x, b, r, rn);
  else
    {   /* A*x-b but only the upper triangle of A is stored */
      for ( i = 0 ; i < A->m ; i++ ) 
	wk[i] = -(long double) b[i];
      k = 0;
      for ( i = 0 ; i < A->m ; i++ )
	{
	  for ( l = 0 ; l < A->mnel[i] ; l++ )
	    {
	      j = A->icol[k] - 1;
	      wk[i] += (long double) A->R[k]  *  (long double) x[j];
	      if ( j != i )
		wk[j] += (long double) A->R[k]  *  (long double) x[i];
	      k++;
	    }
	}
      for ( i = 0 ; i < A->m ; i++ ) 
	{
	  r[i] = (double) wk[i];
	  norm2 += wk[i]*wk[i];
	}
      *rn = (double) sqrt(norm2);
    }
  return;
}


void cmplx_residu_with_prec(SciSparse *A, 
				   double xr[], double xi[],
				   double br[], double bi[], 
				   double rr[], double ri[],
				   double *rn)
{
  /*  the same for complex system */
  int i, j, k, l;
  long double tempr, tempi, norm2;

  norm2 = 0.0;
  k = 0;
  for ( i = 0 ; i < A->m ; i++ )
    {
      tempr = 0.0; tempi = 0.0;
      for ( l = 0 ; l < A->mnel[i] ; l++ )
	{
	  j = A->icol[k] - 1;
	  tempr +=   (long double) A->R[k]  *  (long double) xr[j]
                   - (long double) A->I[k]  *  (long double) xi[j];
	  tempi +=   (long double) A->I[k]  *  (long double) xr[j]
                   + (long double) A->R[k]  *  (long double) xi[j];
	  k++;
	}
      tempr -=  (long double) br[i];
      tempi -=  (long double) bi[i];
      rr[i] = (double) tempr;
      ri[i] = (double) tempi;
      norm2 += tempr*tempr + tempi*tempi;
    }
  *rn = (double) sqrt(norm2);
  return;
}
