#include <math.h>
#include "../machine.h"
#include "Math.h"


static int nnuees __PARAMS((double *x, double *y, double *z, integer *n, double *bx, double *by, double *bz, integer *nbbary, integer *chaine, integer *ierr));

static int heapi2 __PARAMS((integer *criter, integer *record, integer *n));
static int bblocs __PARAMS((double *x, double *y, double *z, integer *n, double *bx, double *by, double *bz, integer *nbbary, integer *chaine, integer *ierr));

/*------------------------------------------------------
 *------------------------------------------------------*/


int C2F(nues1)(double *xyz, integer *n, double *bxyz, integer *nbbary, integer *chaine, integer *ierr)
{
  bblocs(xyz, xyz+(*n), xyz+2*(*n), n, bxyz, bxyz+(*nbbary), 
	 bxyz+2*(*nbbary), nbbary, chaine, ierr); 
  nnuees(xyz, xyz+(*n), xyz+2*(*n), n, bxyz, bxyz+(*nbbary), 
	 bxyz+2*(*nbbary), nbbary, chaine, ierr); 
  return 0;
}

/*------------------------------------------------------
 * entry : n points x,y,z 
 * nbbary : initial bary_centers bx,by,bz 
 * out : new nbbary bary_centers  bx,by,bz 
 * chaine(i) point on the barycenter whom  x(i),y(i),z(i)
 * belongs 
 *------------------------------------------------------*/

static int nnuees(double *x, double *y, double *z, integer *n, double *bx, double *by, double *bz, integer *nbbary, integer *chaine, integer *ierr)
{
  integer i1, i2;
  double d1, d2, d3;
  static double d;
  static integer i, k, l, m, iclas;
  static int vvide;
  static double id;
  static integer im;
  static double ax[256], ay[256], az[256];
  static integer nn[256];
  static double ix;
  static integer nbiter;
  static double idx;
  static integer nnn;

    /* Parameter adjustments */
  --chaine;
  --z;
  --y;
  --x;
  --bz;
  --by;
  --bx;

  *ierr = 0;
  if (*nbbary > 256) 
    {
      /*     too many bary_center */
      *ierr = 1;
      return 0;
    } 
  else if (*n <= *nbbary) 
    {
      /*     number of bary_centers is greater than number of points */
      i1 = *n;
      for (i = 1; i <= i1; ++i) {
	bx[i] = x[i];
	by[i] = y[i];
	bz[i] = z[i];
	chaine[i] = i;
      }
      i1 = *nbbary;
      for (i = *n + 1; i <= i1; ++i) {
	bx[i] = (float)0.;
	by[i] = (float)0.;
	bz[i] = (float)0.;
	chaine[i] = 0;
      }
      return 0;
  }
  /*     init des ancients bary_centres */
  i1 = *nbbary;
  for (i = 1; i <= i1; ++i) {
    ax[i - 1] = bx[i];
    ay[i - 1] = by[i];
    az[i - 1] = bz[i];
  }
  /*     grande boucle d'iteration */
  nbiter = 0;
 L9999:
  ++nbiter;
  /*     raz des nouveaux bary_centres */
  i1 = *nbbary;
  for (i = 1; i <= i1; ++i) {
    bx[i] = (float)0.;
    by[i] = (float)0.;
    bz[i] = (float)0.;
    nn[i - 1] = 0;
  }

  i1 = *n;
  for (i = 1; i <= i1; ++i) {
    idx = 1e30;
    l = 0;
    /*     pour tous les ancients bary_centres */
    i2 = *nbbary;
    for (k = 1; k <= i2; ++k) {
      /* Computing 2nd power */
      d1 = x[i] - ax[k - 1];
      /* Computing 2nd power */
      d2 = y[i] - ay[k - 1];
      /* Computing 2nd power */
      d3 = z[i] - az[k - 1];
      ix = d1 * d1 + d2 * d2 + d3 * d3;
      /*     on garde le bary_centre le plus proche du point */
      if (ix < idx) {
	l = k;
	idx = ix;
      }
    }

    if (l == 0) {
      /*     can not find any bary center for that point */
      chaine[i] = 0;
      *ierr = 2;
    }
    /*     on affecte le point a la classe l des nouveaux bary_centres */
    /*     ( le plus proche) */
    /*     le ieme point de l'image index les points par l */
    chaine[i] = l;
    bx[l] += x[i];
    by[l] += y[i];
    bz[l] += z[i];
    ++nn[l - 1];
  }

  i1 = *nbbary;
  for (k = 1; k <= i1; ++k) {
    if (nn[k - 1] != 0) {
      bx[k] /= nn[k - 1];
      by[k] /= nn[k - 1];
      bz[k] /= nn[k - 1];
    }
  }

  vvide = 0;
  i1 = *nbbary;
  for (k = 1; k <= i1; ++k) {
    if (nn[k - 1] == 0) {
    L7:
      /*     k est une classe vide */
      vvide = 1;
      /*     on choisi la classe de plus grand nombre d'elements */
      im = 0;
      l = 1;
      i2 = *nbbary;
      for (m = 1; m <= i2; ++m) {
	if (nn[m - 1] >= im) {
	  l = m;
	  im = nn[l - 1];
	}
      }
      if (im <= 2) {
	/*     on ne peut couper la classe */
	bx[k] = (float)0.;
	by[k] = (float)0.;
	bz[k] = (float)0.;
	vvide = 0;
	/*     empty cluster */
	goto L6;
      }
      /*     empty cluster ,k, :we cut in two the cluster ,l, with , nn(l), */
      /*     elements */
      nnn = nn[l - 1] / 2;
      nn[l - 1] = nnn;
      nn[k - 1] = nnn;
      iclas = 1;
      i2 = *n;
      for (i = 1; i <= i2; ++i) {
	if (chaine[i] == l) {
	  if (iclas == 1) {
	    bx[k] = x[i];
	    by[k] = y[i];
	    bz[k] = z[i];
	    chaine[i] = k;
	    ++iclas;
	  } else {
	    chaine[i] = 0;
	    bx[l] = x[i];
	    by[l] = y[i];
	    bz[l] = z[i];
	  }
	  idx = (d1 = bx[k] - bx[l], Abs(d1)) + (d2 = by[k] - 
						 by[l], Abs(d2)) + (d3 = bz[k] - bz[l], Abs(
											    d3));
	  idx /= (d1 = bx[l], Abs(d1)) + (d2 = by[l], Abs(
							  d2)) + (d3 = bz[l], Abs(d3));
	  if (idx > (float)1e-6) {
	    goto L6;
	  }
	}
      }
      /*     cluster',l,'has not been cut */
      nn[l - 1] = 1;
      nn[k - 1] = 0;
      goto L7;
    }
  L6:
    ;
  }

  idx = (float)0.;
  i1 = *nbbary;
  for (k = 1; k <= i1; ++k) {
    /* Computing 2nd power */
    d1 = ax[k - 1] - bx[k];
    /* Computing 2nd power */
    d2 = ay[k - 1] - by[k];
    /* Computing 2nd power */
    d3 = az[k - 1] - bz[k];
    id = d1 * d1 + d2 * d2 + d3 * d3;
    idx = Max(idx,id);
    ax[k - 1] = bx[k];
    ay[k - 1] = by[k];
    az[k - 1] = bz[k];
  }

  d = sqrt(idx);
  /*     iteration suivante */
  /*     teste pour iteration suivante */
  if (d > (sqrt(nbiter) + (float)1.) || vvide) {
    if (vvide) {
      /*     empty cluster : we iterate */
    }
    /*     d maximum displacement of the bary centers */
    goto L9999;
  }
  /*     d,'maximum displacement of the bary centers */
  return 0;
} 

/*------------------------------------------------------
 *     en entree n points x,y,z 
 *     et nbbary = nombre de bary_centres desires 
 *     en sortie les nbbary bary_centres trouves  bx,by,bz 
 *     chaine(i) pointe sur le bary_centre auquel 
 *     le point x(i),y(i),z(i) appartient 
 *------------------------------------------------------*/

static int bblocs(double *x, double *y, double *z, integer *n, double *bx, double *by, double *bz, integer *nbbary, integer *chaine, integer *ierr)
{
  /* System generated locals */
  integer i1, i2;
  double d1, d2;

  /* Local variables */
  static integer bmin[256], bmax[256], rang, dmax, kmin, tete[256], 
    rmin[256], rmax[256], vmin[256], vmax[256], next, b, i, j, k, m, r,
    v, w, itmax, histo[256];
  static integer bi, ri, nbbloc, vi, diagon[256], nbbouc;
  static int manque;
  static integer weight, clasvo[256], icompt;
  static double xma, yma, zma;
  static integer var;
  static double xmi, ymi, zmi;

  --chaine;
  --z;
  --y;
  --x;
  --bz;
  --by;
  --bx;

  /* Function Body */
  *ierr = 0;
  if (*nbbary > 256) {
    /*     le nombre de barycentres demande est trop grand */
    *ierr = 1;
    return 0;
  } else if (*n <= *nbbary) {
    i1 = *n;
    for (i = 1; i <= i1; ++i) {
      bx[i] = x[i];
      by[i] = y[i];
      bz[i] = z[i];
      chaine[i] = i;
      /* L204: */
    }
    i1 = *nbbary;
    for (i = *n + 1; i <= i1; ++i) {
      bx[i] = (float)0.;
      by[i] = (float)0.;
      bz[i] = (float)0.;
      chaine[i] = i;
    }
    return 0;
  }

  /*     initialisations */
  xmi = x[1];
  xma = x[1];
  ymi = y[1];
  yma = y[1];
  zmi = z[1];
  zma = z[1];

  i1 = *n;
  for (i = 1; i <= i1; ++i) {
    /* Computing MIN */
    d1 = xmi, d2 = x[i];
    xmi = Min(d1,d2);
    /* Computing MAX */
    d1 = xma, d2 = x[i];
    xma = Max(d1,d2);
    /* Computing MIN */
    d1 = ymi, d2 = y[i];
    ymi = Min(d1,d2);
    /* Computing MAX */
    d1 = yma, d2 = y[i];
    yma = Max(d1,d2);
    /* Computing MIN */
    d1 = zmi, d2 = z[i];
    zmi = Min(d1,d2);
    /* Computing MAX */
    d1 = zma, d2 = z[i];
    zma = Max(d1,d2);
    chaine[i] = i + 1;
  }
  if (xmi == xma) {
    xma += (float)1.;
  }
  if (ymi == yma) {
    yma += (float)1.;
  }
  if (zmi == zma) {
    zma += (float)1.;
  }
  chaine[*n] = 0;
  i1 = *nbbary;
  for (i = 1; i <= i1; ++i) {
    rmin[i - 1] = 256;
    vmin[i - 1] = 256;
    bmin[i - 1] = 256;
    rmax[i - 1] = 0;
    vmax[i - 1] = 0;
    bmax[i - 1] = 0;
    tete[i - 1] = 0;
    bx[i] = 0.;
    by[i] = 0.;
    bz[i] = 0.;
  }
  tete[0] = 1;

/*     recherche et concentration du premier bloc */
/*     initialisation du chainage */

  rmin[0] = 0;
  vmin[0] = 0;
  bmin[0] = 0;
  rmax[0] = 256;
  vmax[0] = 256;
  bmax[0] = 256;

/*     recherche du nombre d'iterations a effectuer */

  itmax = 1;
  for (i = 1; i <= 9; ++i) {
    itmax <<= 1;
    if (itmax > *nbbary) {
      itmax = i - 1;
      goto L31;
    }
  }
  /*     division des blocs en blocs plus petits */
  /*     initialisation */
L31:
  manque = 1;
  nbbloc = 1;
  rang = 1;
  /*     boucle de division */
  i1 = itmax + 1;
  for (nbbouc = 1; nbbouc <= i1; ++nbbouc) {
    /*     ajustement du nombre de boucles / a une puissance de 2 */
  L32:
    if (nbbouc > itmax) {
      rang = *nbbary - nbbloc;
      if (rang <= 0) {
	goto L100;
      }
      /*     classement des rang bary manquants */
      i2 = nbbloc;
      for (j = 1; j <= i2; ++j) {
	diagon[j - 1] = rmax[j - 1] - rmin[j - 1] + (vmax[j - 1] - 
						     vmin[j - 1]) + (bmax[j - 1] - bmin[j - 1]);
	clasvo[j - 1] = j;
      }
      heapi2(diagon, clasvo, &nbbloc);
      manque = 0;
    }
    m = nbbloc;
    i = 0;
    icompt = 1;
  L60:
    if (icompt <= rang) {
      if (manque==1) {
	++i;
      } else {
	if (diagon[m - 1] == 0) {
	  goto L32;
	}
	i = clasvo[m - 1];
	--m;
      }
      /*     division du bloc 1 */
      /*     recherche de la plus grande dimention */
      r = rmax[i - 1] - rmin[i - 1];
      v = vmax[i - 1] - vmin[i - 1];
      b = bmax[i - 1] - bmin[i - 1];
      if (r == 0 && v == 0 && b == 0) {
	goto L110;
      }
      if (r >= v) {
	if (r >= b) {
	  dmax = 1;
	} else {
	  dmax = 3;
	}
      } else {
	if (v >= b) {
	  dmax = 2;
	} else {
	  dmax = 3;
	}
      }
      weight = 0;
      /*     calcul de l'histogramme suivant cette dimention */
      for (j = 1; j <= 256; ++j) {
	histo[j - 1] = 0;
      }
      j = tete[i - 1];
      switch ((int)dmax) {
      case 1:  goto L1;
      case 2:  goto L2;
      case 3:  goto L3;
      }
    L1:
      if (j != 0) {
	ri = (integer) ((x[j] - xmi) * 255 / (xma - xmi) + 1);
	++histo[ri - 1];
	++weight;
	j = chaine[j];
	goto L1;
      }
      goto L4;
    L2:
      if (j != 0) {
	vi = (integer) ((y[j] - ymi) * 255 / (yma - ymi) + 1);
	++histo[vi - 1];
	++weight;
	j = chaine[j];
	goto L2;
      }
      goto L4;
    L3:
      if (j != 0) {
	bi = (integer) ((z[j] - zmi) * 255 / (zma - zmi) + 1);
	++histo[bi - 1];
	++weight;
	j = chaine[j];
	goto L3;
      }
    L4:
      /*     division du bloc suivant l'histogramme */
      weight /= 2;
      switch ((int)dmax) {
      case 1:  goto L13;
      case 2:  goto L14;
      case 3:  goto L15;
      }
    L13:
      k = rmin[i - 1];
      goto L16;
    L14:
      k = vmin[i - 1];
      goto L16;
    L15:
      k = bmin[i - 1];
    L16:
      kmin = k + 1;
      w = 0;
    L5:
      if (w <= weight) {
	++k;
	w += histo[k - 1];
	goto L5;
      }
      if (k <= kmin) {
	++k;
      }
      /*     reinitialisation des parametres du bloc divise */
      rmax[i - 1] = 0;
      vmax[i - 1] = 0;
      bmax[i - 1] = 0;
      rmin[i - 1] = 256;
      vmin[i - 1] = 256;
      bmin[i - 1] = 256;
      ++nbbloc;
      /*     mise a jour des chainages */
      /*     recompactage des blocs eclates */
      j = tete[i - 1];
      tete[i - 1] = 0;
      tete[nbbloc - 1] = 0;
    L6:
      if (j != 0) {
	next = chaine[j];
	ri = (integer) ((x[j] - xmi) * 255 / (xma - xmi));
	vi = (integer) ((y[j] - ymi) * 255 / (yma - ymi));
	bi = (integer) ((z[j] - zmi) * 255 / (zma - zmi));
	switch ((int)dmax) {
	case 1:  goto L7;
	case 2:  goto L8;
	case 3:  goto L9;
	}
      L7:
	var = ri + 1;
	goto L11;
      L8:
	var = vi + 1;
	goto L11;
      L9:
	var = bi + 1;
      L11:
	if (var >= k) {
	  chaine[j] = tete[nbbloc - 1];
	  tete[nbbloc - 1] = j;
	  /* Computing MAX */
	  i2 = rmax[nbbloc - 1];
	  rmax[nbbloc - 1] = Max(i2,ri);
	  /* Computing MAX */
	  i2 = vmax[nbbloc - 1];
	  vmax[nbbloc - 1] = Max(i2,vi);
	  /* Computing MAX */
	  i2 = bmax[nbbloc - 1];
	  bmax[nbbloc - 1] = Max(i2,bi);
	  /* Computing MIN */
	  i2 = rmin[nbbloc - 1];
	  rmin[nbbloc - 1] = Min(i2,ri);
	  /* Computing MIN */
	  i2 = vmin[nbbloc - 1];
	  vmin[nbbloc - 1] = Min(i2,vi);
	  /* Computing MIN */
	  i2 = bmin[nbbloc - 1];
	  bmin[nbbloc - 1] = Min(i2,bi);
	} else {
	  chaine[j] = tete[i - 1];
	  tete[i - 1] = j;
	  /* Computing MAX */
	  i2 = rmax[i - 1];
	  rmax[i - 1] = Max(i2,ri);
	  /* Computing MAX */
	  i2 = vmax[i - 1];
	  vmax[i - 1] = Max(i2,vi);
	  /* Computing MAX */
	  i2 = bmax[i - 1];
	  bmax[i - 1] = Max(i2,bi);
	  /* Computing MIN */
	  i2 = rmin[i - 1];
	  rmin[i - 1] = Min(i2,ri);
	  /* Computing MIN */
	  i2 = vmin[i - 1];
	  vmin[i - 1] = Min(i2,vi);
	  /* Computing MIN */
	  i2 = bmin[i - 1];
	  bmin[i - 1] = Min(i2,bi);
	}
	j = next;
	goto L6;
      }
    L110:
      ++icompt;
      goto L60;
    }
    rang <<= 1;
  }

  /*     calcul des bary_centres */

L100:
  i1 = *nbbary;
  for (i = 1; i <= i1; ++i) {
    j = tete[i - 1];
    weight = 0;
  L12:
    if (j != 0) {
      bx[i] += x[j];
      by[i] += y[j];
      bz[i] += z[j];
      ++weight;
      k = j;
      j = chaine[j];
      chaine[k] = i;
      goto L12;
    }
    bx[i] /= weight;
    by[i] /= weight;
    bz[i] /= weight;
  }
  return 0;
} 




/*------------------------------------------------------
 *     trie selon les valeurs de criter croissantes 
 *     record suit le reordonnancement 
 *------------------------------------------------------*/

static int heapi2(integer *criter, integer *record, integer *n)
{
  static integer crit, i, j, l, r, rec;
  --record;
  --criter;
  if (*n <= 1) return 0;

  l = *n / 2 + 1;
  r = *n;
 L2:
  if (l <= 1)  goto L20;
  --l;
  rec = record[l];
  crit = criter[l];
  goto L3;
L20:
  rec = record[r];
  crit = criter[r];
  record[r] = record[1];
  criter[r] = criter[1];
  --r;
  if (r == 1)  goto L999;
 L3:
  j = l;
 L4:
  i = j;
  j <<= 1;
  if (j - r < 0) {
    goto L5;
  } else if (j == r) {
    goto L6;
  } else {
    goto L8;
  }
 L5:
  if (criter[j] < criter[j + 1]) {
    ++j;
  }
 L6:
  if (crit >= criter[j]) {
    goto L8;
  }
  record[i] = record[j];
  criter[i] = criter[j];
  goto L4;
 L8:
  record[i] = rec;
  criter[i] = crit;
  goto L2;
L999:
  record[1] = rec;
  criter[1] = crit;
  return 0;
} 

