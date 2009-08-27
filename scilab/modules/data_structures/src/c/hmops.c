/*
 *  hmdops.c
 *
 *  PURPOSE
 *     codes to accelerate scilab hypermatrices operations
 *     like extraction / insertion / creation
 *
 *  AUTHOR
 *     Bruno Pincon (Bruno.Pincon@iecn.u-nancy.fr)
 *
 *  exportation de C2F(ishm)
 *                 C2F(intehm)
 *                 C2F(intihm)
 *  modifications pour le champs dim en int32 maintenant
 */


#include <math.h>
#include <stdlib.h>
#include "stack-c.h"
#include "stack2.h"
#include "hmops.h"
#include "Scierror.h"
#include "core_math.h"
#include "localization.h"

#define sign(a) ((a) < 0 ? -1 : 1)

/* SCI_IMPLICIT_POLY is not used elsewhere in C code ... maybe fortran */
#define SCI_IMPLICIT_POLY 129 
#define NOT_REAL_or_CMPLX_or_BOOL_or_INT -1
#define OLD_HYPERMAT -2


typedef struct hypermat {
  int type;           /* type of the (elements of the) hyper matrix */
  int it;             /* sub type (in case of type=1 or type=8) */
  int dimsize;        /* number of dimensions of the hyper matrix */
  int size;           /* total number of elements : size = dims[0]x... x dims[dimsize-1] */
  int *dims;          /* number of elements in each dimension */
  double *R, *I;      /* in case of type=1 points to the elements (I being used if it=1)  */
  void *P;            /* in case of type=4 or 8 points to the elements */
} HyperMat;

typedef struct sci_bool_sparse {
  int m;
  int n;
  int nel;
  int *mnel;
  int *jcol;
} SciBoolSparse;



extern int C2F(ddmpev)();
#ifdef _MSC_VER
int C2F(createlistcvarfrom)();
#endif

static int get_length(int num)
{
  int il;
  il = iadr(*Lstk( num + Top - Rhs ));
  return(*istk(il+1));
}

static void get_length_and_pointer(int num, int *n, int **t)
{
  int il;
  il = iadr(*Lstk( num + Top - Rhs ));
  *n = *istk(il+1);
  *t = istk(il+4);
}

#define GetHMat(pos,H) if (! get_hmat(pos,H)) { return 0;}

static int get_hmat(int num, HyperMat *H)
{
  int il, il1, il2, il3,/* it,*/ lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw ));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  if ( *istk(il) != sci_mlist )
    return 0;
  else if ( *istk(il+1) != 3 )  /* a hm mlist must have 3 fields */
    return 0;

  /*  get the pointers for the 3 fields */
  il1 = sadr(il+6);
  il2 = il1 + *istk(il+3) - 1;
  il3 = il1 + *istk(il+4) - 1;
  il1 = iadr(il1); il2 = iadr(il2); il3 = iadr(il3);

  /*  test if the first field is a matrix string with 3 components
   *  and that the first is "hm" (ie 17 22  in scilab char code)
   */
  if ( (*istk(il1) != sci_strings)  |  ((*istk(il1+1))*(*istk(il1+2)) != 3)  )
    return 0;
  else if ( *istk(il1+5)-1 != 2 )  /* 1 str must have 2 chars */
    return 0;
  else if ( *istk(il1+8) != 17  || *istk(il1+9) != 22 )
    return 0;


  /*  get the 2d field */
  if ( *istk(il2) == sci_matrix  &&  *istk(il2+3) == 0 )
    {
      /* this is an old hypermat (the dim field is an array of doubles) */
      H->type = OLD_HYPERMAT;
      H->it = -1; H->size = -1;
      H->P = (void *) istk(il3);
      return 2;
    }

  if ( (*istk(il2) != sci_ints)  |  (*istk(il2+3) != I_INT32) )
    return 0;


  H->dimsize = (*istk(il2+1))*(*istk(il2+2));
  H->dims = istk(il2+4);

  /* needed for Jpc stuff (putlhsvar) */
  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;  /* a voir ? */

  /*  get the 3d field */
  switch ( *istk(il3) )
    {
    case (sci_matrix):
      H->size = (*istk(il3+1))*(*istk(il3+2));
      H->type = sci_matrix;
      H->it = *istk(il3+3);
      H->R = stk(sadr(il3+4));
      if ( H->it == 1 )
	H->I = H->R + H->size;
      return 1;

    case (sci_boolean):
      H->size = (*istk(il3+1))*(*istk(il3+2));
      H->type = sci_boolean;
      H->it = 0;   /* not used */
      H->P = (void *) istk(il3+3);
      return 1;

    case (sci_ints):
      H->size = (*istk(il3+1))*(*istk(il3+2));
      H->type = sci_ints;
      H->it = *istk(il3+3);
      H->P = (void *) istk(il3+4);
      return 1;

    default:
      H->type = NOT_REAL_or_CMPLX_or_BOOL_or_INT;
      H->it = -1; H->size = -1;
      H->P = (void *) istk(il3);
      return 2;
    }
}

int C2F(ishm)()
{
  /* teste si l'argument en Top est une hypermatrice */
  int il, il1, il2;
  il = iadr(*Lstk( Top ));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  if ( *istk(il) != sci_mlist )
    return 0;
  else if ( *istk(il+1) != 3 )  /* a hm mlist must have 3 fields */
    return 0;

  /*  get the pointer of the first and second fields */
  il1 = sadr(il+6);
  il2 = il1 + *istk(il+3) - 1;
  il1 = iadr(il1); il2 = iadr(il2);

  /*  test if the first field is a matrix string with 3 components
   *  and that the first is "hm" (ie 17 22  in scilab char code)
   */
  if ( (*istk(il1) != sci_strings)  |  ((*istk(il1+1))*(*istk(il1+2)) != 3)  )
    return 0;
  else if ( *istk(il1+5)-1 != 2 )  /* 1 str must have 2 chars */
    return 0;
  else if ( *istk(il1+8) != 17  || *istk(il1+9) != 22 )
    return 0;

  return 1;
}

static int get_mat_as_hmat(int num, HyperMat *H)
{
  int il, type, lw;
  static int dims[2];

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw ));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  type = *istk(il);

  if (type == sci_matrix || type == sci_boolean || type == sci_ints)
    {

      /* needed for Jpc stuff (putlhsvar) ? */
      Nbvars = Max(Nbvars,num);
      C2F(intersci).ntypes[num-1] = '$';
      C2F(intersci).iwhere[num-1] = *Lstk(lw);
      C2F(intersci).lad[num-1] = 0;  /* a voir ? */

      H->type = type;
      H->dimsize = 2;
      dims[0] = *istk(il+1);
      dims[1] = *istk(il+2);
      H->size = dims[0]*dims[1];
      H->dims = dims;
      if (type == sci_matrix)
	{
	  H->it = *istk(il+3);
	  H->R = stk(sadr(il+4));
	  if (H->it == 1)
	    H->I = H->R + H->size;
	}
      else if (type == sci_boolean)
	{
	  H->it = 0;
	  H->P = (void *) istk(il+3);
	}
      else /* type = sci_ints */
	{
	  H->it = *istk(il+3);
	  H->P = (void *) istk(il+4);
	}
      return 1;
    }
    else
      return 0;
}

#define CreateHMat(pos,H) if (! cre_hmat(pos,H)) { return 0;}

static int cre_hmat(int pos, HyperMat *H)
{
  /*  dans cette version, seuls les champs dimsize, size et it sont definis
   *  et on alloue alors la memoire des champs dims, R (et I si it=1) dans
   *  la pile scilab (juste � la place occupee par la variable).
   */
  static char *Str[]= { "hm","dims","entries"}; int m1=1,n1=3;
  int mL=3,nL=1,lL, one=1, lr, lc, lar, lac;
  CreateVar(pos,MATRIX_ORIENTED_TYPED_LIST_DATATYPE, &mL, &nL, &lL);
  CreateListVarFromPtr(pos,1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, Str);
  lr = 4; lar = -1;
  CreateListVarFrom(pos,2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &one, &H->dimsize, &lr, &lar);
  H->dims = istk(lr);

  lar = -1; lac = -1;

  switch (H->type)
    {
    case (sci_matrix):
      CreateListCVarFrom(pos,3,MATRIX_OF_DOUBLE_DATATYPE, &H->it, &H->size, &one , &lr, &lc, &lar, &lac);
      H->R = stk(lr);
      if ( H->it == 1)
	H->I = stk(lc);
      return 1;

    case (sci_boolean):
      CreateListVarFrom(pos, 3,MATRIX_OF_BOOLEAN_DATATYPE, &H->size, &one, &lr, &lar);
      H->P = (void *) istk(lr);
      return 1;

    case (sci_ints):
      lr = H->it;
      CreateListVarFrom(pos, 3,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &H->size, &one, &lr, &lar);
      H->P = (void *) istk(lr);
      return 1;
    }

	/* Ajout Allan CORNET Correction Warning */
	/* warning C4715: 'cre_hmat' : not all control paths return a value */
	return 1;


}


#define GetSciBoolSparse(pos,M) if (! get_sci_bool_sparse(pos,M)) { return 0;}

static int get_sci_bool_sparse(int num, SciBoolSparse *M)
{
  int il, lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk(lw));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  if ( *istk(il) != sci_boolean_sparse )
    return 0;

  /* needed for Jpc stuff (putlhsvar) */
  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;  /* a voir ? */

  M->m  = *istk(il+1);
  M->n  = *istk(il+2);
  M->nel  = *istk(il+4);
  M->mnel = istk(il+5);
  M->jcol = istk(il+5+M->m);
  return 1;
}


#define ReshapeHMat(pos,H,new_dimsize) if (! reshape_hmat(pos,H,new_dimsize)) { return 0;}

static int reshape_hmat(int pos, HyperMat *H, int new_dimsize)
{
  /*
   *   This utility routine is used when an hypermatrix H
   *   is indexed with fewer indices vectors than its dimsize
   *   (for instance the profil of H is n1 x n2 x n3 but
   *    an expression like H(v1,v2) is used). So we have to
   *    reconsidered the profil of H for this operation (in
   *    my example H is then considered with the profil
   *    n1 x (n2*n3) ). For that (as H is passed by reference)
   *    we create a new variable at position pos, recompute
   *    the new profil in this var and then H->dims will points to it.
   *
   */
  int *new_dims;
  int k, one=1, l;

  l = I_INT32; CreateVar(pos,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &new_dimsize, &one, &l);
  new_dims = istk(l);
  for ( k = 0 ; k < new_dimsize ; k++)
    new_dims[k] = H->dims[k];
  for ( k = new_dimsize ; k < H->dimsize ; k++ )
    new_dims[new_dimsize-1] *= H->dims[k];
  H->dimsize = new_dimsize;
  H->dims = new_dims;
  return 1;
}

static int cmpint(const void *pn1, const void *pn2)
{
  int *n1 = (int *)pn1, *n2 = (int *)pn2;
  return (*n1 - *n2);
}

static int index_convert(double *td, int * ti, int mn, int *ind_max)
{
  /*  convert a scilab vector of indices (which are integers but
   *  stored as double) in an int vector together with
   *  detecting the max index
   */
  int k, val;
  *ind_max = 0;
  for ( k = 0 ; k < mn ; k++ )
    {
      val = (int) td[k];
      if ( val <= 0 )
	   return 0;
      if ( val > *ind_max )
	*ind_max = val;
      ti[k] = val - 1;
    }
  return 1;
}

static int create_index_vector(int pos, int pos_ind, int *mn,
			       int nmax, int *ind_max)
{
  /*
   *   converti une "structure" scilab d'indicage en un vecteur d'indices
   *
   *      pos     : position de la variable initiale
   *      pos_ind : position de la variable resultante (le vecteur d'indice)
   *      mn      : taille du vecteur d'indice resultant
   *      ind_max : max de ce vecteur
   *      nmax    : utilise pour les descriptions implicites, aussi ind_max ne
   *                doit pas lui etre superieur
   */
      /* code based on SCI/modules/core/src/fortran/indxg.f */

  int m, n, l, li, one=1, trois=3, *ti,/* val,*/ il, k, i, j, ideb, ipas, ifin, *P;
  double *td, px[3], x;
  HyperMat H;
  SciBoolSparse B;
  SciIntMat IV;

  switch ( GetType(pos) )
    {
    case (sci_matrix):

      GetRhsVar(pos,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);
      if ( m == -1 )      /* implicit index : */
	{
	  *mn = nmax; *ind_max = nmax;
	  li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
	  for ( k = 0 ; k < *mn ; k++ )
	    ti[k] = k;
	  return 1;
	}
      else if ( m == 0 )  /* index is the void matrix [] */
	{
	  *mn = 0; *ind_max = 0;
	  return 1;
	}
      else                /* "normal" index */
	{
	  td = stk(l); *mn = m*n; *ind_max = 0;
	  li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
	  return ( index_convert(td, ti, *mn, ind_max) );
	}

    case (sci_ints):

      GetRhsVar(pos,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m, &n, (int *)&IV);

      if ( m <= 0 )      /* normaly not possible */
	return 0;
      else               /* "normal" index */
	{
	  *mn = m*n; *ind_max = 0;
	  li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
	  li = 4; C2F(tpconv)(&(IV.it), &li, mn, IV.D, &one, (void *) ti, &one); /* convert to usual int */

	  for ( i = 0 ; i < *mn ; i++ )
	    {
	      if ( ti[i] <= 0 ) return 0;
	      if ( ti[i] > *ind_max ) *ind_max = ti[i];
	      ti[i]--;
	    }
	  return 1;
	}

    case (sci_poly):

      il = iadr( *Lstk( pos + Top - Rhs ) );
      if ( *istk(il) < 0 ) il = iadr( *istk(il+1) );
      m = *istk(il+1); n = *istk(il+2);
      if ( *istk(il+3) != 0 )
	return 0;
      *mn = m*n;
      l = sadr(il+9+*mn);
      CreateVar( pos_ind,MATRIX_OF_DOUBLE_DATATYPE, mn, &one, &li); td = stk(li);
      x = (double) nmax;
      C2F(ddmpev)( stk(l), istk(il+8), &one, &x, td, &one, &one, mn);
      ti = (int *)td;
      return ( index_convert(td, ti, *mn, ind_max) );

    case (sci_implicit_poly ):         /* p1:p2:p3 */

      il = iadr( *Lstk( pos + Top - Rhs ) );
      if ( *istk(il) < 0 ) il = iadr( *istk(il+1) );
      l = sadr( il+12 );
      x = (double) nmax;
      C2F(ddmpev)( stk(l), istk(il+8), &one, &x, px, &one, &one, &trois);
      ideb = (int) px[0]; ipas = (int) px[1]; ifin = (int) px[2];
      if ( ipas == 0  ||  (ifin-ideb)*sign(ipas) < 0 )   /* index is finaly [] */
	{
	  *mn = 0; *ind_max = -1;
	  return 1;
	}
      else if ( (ipas < 0  &&  ifin <= 0)   ||  (ipas > 0  &&  ideb <= 0) )
	{
	  return 0;    /* at least one index will be <= 0 => error */
	}
      else
	{
	  int ind_min;
	  *mn = (abs(ifin-ideb))/abs(ipas)+1;
	  if ( ipas > 0 )
            {
              ind_min = ideb; *ind_max = ideb + (*mn-1)*ipas;
            }
          else /* ipas < 0 (the case ipas==0 is treated before) */
            {
              *ind_max = ideb; ind_min = ideb  + (*mn-1)*ipas; /* Bug 4015 fix: flip left to right for hypermatrices */
            }
	  if ( ind_min <= 0 )
            return 0;    /* at least one index is <= 0 => error */

	  li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
	  ti[0] = ideb-1;  /* -1 to get 0-based indices */
	  for ( k = 1 ; k < *mn ; k++ ) ti[k] = ti[k-1] + ipas;
	  return 1;
	}

    case (sci_boolean) :

      GetRhsVar(pos,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l);
      if ( m*n != nmax )
	return 0;
      *mn = 0;
      for ( k = 0 ; k < nmax ; k++ )
	if ( *istk(l+k) != 0 )
	  (*mn)++;
      if ( *mn == 0 )
	{
	  *ind_max = 0; return 1;
	}
      li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
      i = 0;
      for ( k = 0 ; k < nmax ; k++ )
	if ( *istk(l+k) != 0 )
	  {
	    ti[i] = k; i++;
	  }
      *ind_max = ti[*mn-1] + 1;
      return 1;

    case (sci_mlist) :         /* Try if it is an hypermat of BOOLEANS */

      GetHMat(pos, &H);
      if ( H.type != sci_boolean ||  H.size != nmax)
	return 0;
      P = (int *) H.P;
      *ind_max = 0;
      *mn = 0;
      for ( k = 0 ; k < nmax ; k++ )
	if ( P[k] != 0 )
	  (*mn)++;
      if ( *mn == 0 )
	{
	  *ind_max = 0; return 1;
	}
      li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
      i = 0;
      for ( k = 0 ; k < nmax ; k++ )
	if ( P[k] != 0 )
	  {
	    ti[i] = k; i++;
	  }
      *ind_max = ti[*mn-1] + 1;
      return 1;


    case (sci_boolean_sparse) :

      GetSciBoolSparse(pos, &B);
      if ( B.m*B.n != nmax )
	return 0;

      if ( B.nel == 0 )  /* false sparse matrix => index is [] */
	{
	  *mn = 0; *ind_max = 0;
	  return 1;
	}

      *mn = B.nel;
      li = 4; CreateVar(pos_ind,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, mn,   &one,   &li); ti = istk(li);
      if ( B.m == 1 )
	{
	  for ( k = 0 ; k < B.nel ; k++ )
	    ti[k] = B.jcol[k] - 1;
	}
      else if ( B.n == 1 )
	{
	  i = 0;
	  for ( k = 0 ; k < B.m ; k++ )
	    if ( B.mnel[k] != 0 )
	      {
		ti[i] = k; i++;
	      }
	}
      else
	{
	  k = 0;
	  for ( i = 0 ; i < B.m ; i++ )
	    for ( l = 0 ; l < B.mnel[i] ; l++ )
	      {
		j = B.jcol[k] - 1;
		ti[k] = j*B.m + i;
		k++;
	      }
	  qsort((void *)ti, (size_t) B.nel, sizeof(int), cmpint);
	}
      *ind_max = ti[*mn-1] + 1;
      return 1;

    default :
      return 0;
    }
}


static void compute_indices(int dec, int dimsize, int dims[], int j[])
{
  /*
   *   from an indexing (i0,i1,i2,...) of an hypermatrix of size
   *   dims[0] x dims[1] x dims[2] x....  computes the "real" one
   *   dimensionnal indices (hypermatrices have the fortran order).
   */

  int nd, i, k, K, Knew, m, p, temp;
  int *id;

  get_length_and_pointer(dec+dimsize, &nd, &id);
  K = nd;
  for ( k = 0 ; k < K ; k++ )
    j[k] = id[k];

  for ( i = dimsize-1 ; i > 0 ; i-- )
    {
      get_length_and_pointer(dec+i, &nd, &id);
      Knew = K * nd;
      m = Knew-1;
      for ( k = K-1 ; k >= 0 ; k--)
	{
	  temp = dims[i-1] * j[k];
	  for ( p = nd-1 ; p >= 0 ; p-- )
	    {
	      j[m] = id[p] + temp;
	      m--;
	    }
	}
      K = Knew;
    }
}


int C2F(intehm)()
{
  /*
   *  Extraction routine for an hypermatrix of type REAL_OR_COMPLEX, BOOLEAN
   *  and INTEGER (the 6 types of scilab ints)
   *
   *    He = ehm ( v_1, v_2, ..., v_nb_iv, H )
   *
   */
  HyperMat H, He;
  int dec, i, k, l, m, n, mn, ntot, ind_max;
  int *j, ier, one=1, zero=0, ltot, nb_index_vectors, final_dimsize, lr, lc;
  int *P, *Pe;
  short int *siP, *siPe;
  char  *cP, *cPe;

/*   CheckLhs(minlhs,maxlhs); */

  if ( Rhs < 2 )
    {
      Scierror(999,_(" An hypermatrix extraction must have at least 2 arguments. "));
      return(0);
    };

  if ( ! get_hmat(Rhs, &H) )
    {
      Scierror(999,_(" Argument is not an hypermatrix. "));
      return 0;
    }
  else if ( H.type == NOT_REAL_or_CMPLX_or_BOOL_or_INT  || H.type == OLD_HYPERMAT )
    {
      /*  do the extraction with the macro %hm_e  */
      Fin = -Fin;
      return 0;
    }

  nb_index_vectors = Rhs-1;
  if ( H.dimsize <  nb_index_vectors )
    {
      Scierror(999,_(" Incompatible hypermatrix extraction. "));
      return 0;
    }
  else if ( H.dimsize > nb_index_vectors )  /* reshape H */
    {
      ReshapeHMat(Rhs+1, &H, nb_index_vectors );
      dec = Rhs+1;
    }
  else
    dec = Rhs;

  if ( H.size == 0 )   /* the hypermat is empty => return an empty matrix ? */
    {
      CreateVar(dec+1,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &l);
      LhsVar(1) = dec+1;
      PutLhsVar();
      return 0;
    }


  ntot = 1;   /* will be the nb of elts of the extracted hmat or mat */
  for ( i = 1 ; i <= nb_index_vectors ; i++ )
    {
      ier = create_index_vector(i, dec+i, &mn, H.dims[i-1], &ind_max);
      if ( ier == 0  ||  ind_max > H.dims[i-1] )
	{
	  Scierror(999,_("Bad (%d th) index in hypermatrix extraction. "),i); return 0;
	}
      if ( mn == 0 )   /* the vector index is [] => we return an empty matrix */
	{
	  CreateVar(dec+i+1,MATRIX_OF_DOUBLE_DATATYPE, &zero, &zero, &l);
	  LhsVar(1) = dec+i+1;
	  PutLhsVar();
	  return 0;
	}
      ntot *= mn;
    }

  /*  For the Matlab compatibility : an hypermatrix of profil n1 x ... x nj x ... x nk
   *  with  nj > 1 and nj+1 = ... = nk = 1 becomes an hypermatrix of profil n1 x ... x nj
   *  Moreover, in scilab, if nj <= 2, we get in fact a matrix.
   */
  final_dimsize = nb_index_vectors;
  while (final_dimsize > 1 && get_length(dec + final_dimsize) == 1)
    final_dimsize--;
  if ( final_dimsize > 2 )   /* we create an hypermatrix for the extraction result */
    {
      He.dimsize = final_dimsize;
      He.size = ntot;
      He.it = H.it;
      He.type = H.type;
      CreateHMat(dec+Rhs, &He);
      for ( k = 0 ; k < final_dimsize ; k++ )
	He.dims[k] = get_length(dec+k+1);
    }
  else                /* we create a matrix  for the extraction result */
    {
      m = get_length(dec+1);
      if (final_dimsize > 1)
	n = get_length(dec+2);
      else
	n = 1;
      switch (H.type)
	{
	case (sci_matrix):
	  CreateCVar(dec+Rhs,MATRIX_OF_DOUBLE_DATATYPE, &(H.it), &m, &n, &lr, &lc);
	  He.R = stk(lr);
	  if ( H.it == 1 ) He.I = stk(lc);
	  break;
	case (sci_boolean):
	  CreateVar(dec+Rhs,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &lr);
	  He.P = (void *) istk(lr);
	  break;
	case (sci_ints):
	  lr = H.it;
	  CreateVar(dec+Rhs,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m, &n, &lr);
	  He.P = (void *) istk(lr);
	  break;
	}
    }

  /* indices computing */
  ltot = 4; CreateVar(dec+Rhs+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &ntot, &one, &ltot); j = istk(ltot);
  compute_indices(dec, nb_index_vectors, H.dims, j);

  /*  fill the resulting hypermatrix or matrix  */
  switch ( H.type )
    {
    case (sci_matrix) :
      for ( k = 0 ; k < ntot ; k++ )
	He.R[k] = H.R[j[k]];
      if (H.it == 1)
	for ( k = 0 ; k < ntot ; k++ )
	  He.I[k] = H.I[j[k]];
      break;

    case (sci_boolean) :     /* (sci_boolean stored with 4 bytes) */
      Pe = (int *) He.P ; P = (int *) H.P;
      for ( k = 0 ; k < ntot ; k++ )
	Pe[k] = P[j[k]];
      break;

    case (sci_ints) :
      if ( H.it == I_INT32  ||  H.it == I_UINT32 )
	{
	  Pe = (int *) He.P; P = (int *) H.P;
	  for ( k = 0 ; k < ntot ; k++ )
	    Pe[k] = P[j[k]];
	}
      else if ( H.it == I_INT16  ||  H.it == I_UINT16 )
	{
	  siPe = (short int *) He.P; siP = (short int *) H.P;
	  for ( k = 0 ; k < ntot ; k++ )
	    siPe[k] = siP[j[k]];
	}
      else    /* SCI_INT8 and SCI_UINT8 : 1 Byte int */
	{
	  cPe = (char *) He.P; cP = (char *) H.P;
	  for ( k = 0 ; k < ntot ; k++ )
	    cPe[k] = cP[j[k]];
	}
      break;
    }

  LhsVar(1) = dec+Rhs;
  PutLhsVar();
  return 0;
}


int C2F(intihm)()
{
  /*
      une routine d'insertion pour hypermatrice : cas le plus
      simple :   A( vi1, ..., vik ) = B

        ihm ( vi1, vi2, ..., vik, B, A )

      avec des vecteurs d'indices classiques vi1, vi2, ....
      et B une hypermatrice ou bien une matrice
   */

  HyperMat A, B;
  int i, k,/* l, li, m, n,*/ ntot, mn,/* err_neg,*/ iconf, ind_max;
  int nb_index_vectors, B_is_scalar;
  int *j,/* nd,*/ one=1, ltot, il, dec/*, Top_save*/;
  int *PA, *PB;
  short int *siPA, *siPB;
  char *cPA, *cPB;
  int ilp, topk;

/*   CheckLhs(minlhs,maxlhs); */

  if ( Rhs < 3 )
    {
      Scierror(999,_(" An hypermatrix insertion must have at least 3 arguments "));
      return 0;
    };
  nb_index_vectors = Rhs - 2;

  if ( ! get_hmat(Rhs, &A) )
    {
      Scierror(999,_(" Argument is not an hypermatrix "));
      return 0;
    }
  else if ( A.type == NOT_REAL_or_CMPLX_or_BOOL_or_INT  || A.type == OLD_HYPERMAT )
    {
      /* do the job by the %x_i_hm macro family */
      Fin = -Fin;
      return 0;
    }

  if ( ! get_hmat(Rhs-1, &B) )   /* B is not an hypermat => try if it is a matrix */
    if ( ! get_mat_as_hmat(Rhs-1, &B) )  /* it is not a matrix of type 1, 4 or 8 */
      {
	/* it stays some authorized possibilities like A(....) = B with B a polynomial
         * matrix and A a real hypermatrix => try the %x_i_hm macro family
         */
	Fin = -Fin;
	return 0;
      }


  if ( A.type !=  B.type || A.it != B.it || B.size == 0  || A.dimsize <  nb_index_vectors )
    {
      /*  do the job by the %x_i_hm macro family */
      Fin = -Fin;
      return 0;
    }

  if ( B.size == 1 )
    B_is_scalar = 1;
  else
    B_is_scalar = 0;


  if ( A.dimsize > nb_index_vectors )
    {
      ReshapeHMat(Rhs+1, &A, nb_index_vectors);
      dec = Rhs+1;
    }
  else
    dec = Rhs;


  /* get the index vectors */
  ntot = 1;
  iconf = 0;
  for ( i = 1 ; i <= nb_index_vectors ; i++ )
    {
      if (! create_index_vector(i, dec+i, &mn, A.dims[i-1], &ind_max)) return 0;
      if ( mn == 0 )   /* the i th index vector is [] */
	{
	  if ( B_is_scalar )
	    /* nothing append (strange but reproduces the Matlab behavior) */
	    goto the_end;
	  else   /* B have at least 2 elts */
	    {
	      Scierror(999,_(" Bad hypermatrix insertion. ")); return 0;
	    }
	}
      else if ( ind_max > A.dims[i-1] )
	{
	  /* we have to enlarge the hypermat : do the job by the %x_i_hm macro family */
	  Fin = -Fin;
	  return 0;
	}
      else if ( !B_is_scalar  &&  mn != 1 )  /* do the conformity test */
	{
	  while ( iconf < B.dimsize  &&  B.dims[iconf] == 1 )
	    iconf++;
	  if ( iconf >= B.dimsize  ||  B.dims[iconf] != mn )
	    {
	      Scierror(999,_(" Bad hypermatrix insertion. "));
	      return 0;
	    }
	  iconf++;
	}
      ntot *= mn;
    }
  /* to finish the conformity test */
  if ( !B_is_scalar &&  ntot != B.size )
    {
      Scierror(999,_(" Bad hypermatrix insertion. "));
      return 0;
    }

  /* indices computing */
  ltot = 4; CreateVar(dec+Rhs-1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &ntot, &one, &ltot); j = istk(ltot);
  compute_indices(dec, nb_index_vectors, A.dims, j);


  /*   modify in place the hypermatrix A  */
  switch ( A.type )
    {
    case (sci_matrix) :
      if ( B_is_scalar )
	{
	  for ( k = 0 ; k < ntot ; k++ ) A.R[j[k]] = B.R[0];
	  if (A.it == 1)
	    for ( k = 0 ; k < ntot ; k++ ) A.I[j[k]] = B.I[0];
	}
      else
	{
	  for ( k = 0 ; k < ntot ; k++ ) A.R[j[k]] = B.R[k];
	  if (A.it == 1)
	    for ( k = 0 ; k < ntot ; k++ ) A.I[j[k]] = B.I[k];
	}
      break;

    case (sci_boolean) :
      PA = (int *) A.P ; PB = (int *) B.P;
      if ( B_is_scalar )
	for ( k = 0 ; k < ntot ; k++ ) PA[j[k]] = PB[0];
      else
	for ( k = 0 ; k < ntot ; k++ ) PA[j[k]] = PB[k];
      break;

    case (sci_ints) :
      if ( A.it == I_INT32  ||  A.it == I_UINT32 )
	{
	  PA = (int *) A.P ; PB = (int *) B.P;
	  if ( B_is_scalar )
	    for ( k = 0 ; k < ntot ; k++ ) PA[j[k]] = PB[0];
	  else
	    for ( k = 0 ; k < ntot ; k++ ) PA[j[k]] = PB[k];
	}
      else if ( A.it == I_INT16  ||  A.it == I_UINT16 )
	{
	  siPA = (short int *) A.P; siPB = (short int *) B.P;
	  if ( B_is_scalar )
	    for ( k = 0 ; k < ntot ; k++ ) siPA[j[k]] = siPB[0];
	  else
	    for ( k = 0 ; k < ntot ; k++ ) siPA[j[k]] = siPB[k];
	}
      else   /* 1 Byte int */
	{
	  cPA = (char *) A.P; cPB = (char *) B.P;
	  if ( B_is_scalar )
	    for ( k = 0 ; k < ntot ; k++ ) cPA[j[k]] = cPB[0];
	  else
	    for ( k = 0 ; k < ntot ; k++ ) cPA[j[k]] = cPB[k];
	}
      break;
    }

/*
 *  ici j'essaie de faire le boulot de putlhsvar
 *  le code se base sur  setref (SCI/system/createref.f)
 *  on met une variable speciale "en Top" (le nouveau
 *  Top = Top-Rhs+1) qui indique en fait que l'on a
 *  modifi� "en place" la variable topk.
 *  Les instructions  LhsVar(1) = 0; et Nbvars = 0;
 *  permettent a priori de sortir "convenablement"
 *  de putlhsvar.
 */
 the_end:
  il = iadr(*Lstk(Top));
  topk = *istk(il + 2);
  Top = Top - Rhs + 1;
  ilp = iadr(*Lstk(Top));
  *istk(ilp) = -1;
  *istk(ilp+1) = -1;
  *istk(ilp+2) = topk;
  if ( topk > 0 )
    *istk(ilp+3) = *Lstk(topk+1) - *Lstk(topk);
  else
    *istk(ilp+3) = 0;
  *Lstk(Top+1) = sadr(ilp+4);

  LhsVar(1) = 0;
  Nbvars = 0;

  return 0;
}
