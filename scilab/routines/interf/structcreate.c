#include "../stack-c.h"
/*
int C2F(test)()
{
  int sz[3],nz,nf,retval;
  char* fnames[2];
  
  printf("ici\n");
  sz[0]=2;
  sz[1]=3;
  sz[2]=2;
  nz=3;

  fnames[0]="A";
  fnames[1]="Foo";
  nf=2;
  Top = Top +1;
  C2F(structcreate)(&Top, &nz, sz, &nf, fnames, &retval);
printf("la\n");
}
*/

#define memused(it,mn) ((((mn)*( it % 10))/sizeof(int))+1)

#include "../stack-c.h"
/* Table of constant values */

static integer c17 = 17;
static integer c1 = 1;
static integer c4 = 4;

int C2F(structcreate)(lw, nz, sz, nf, fnames,retval)
integer *lw, *nz, *sz, *nf, *retval;
char *fnames[];
{
    integer next,k,lr;

    static integer l;
    static integer l0, n1, il;

    *retval = 0;
    l0 = *Lstk(*lw);

    C2F(intersci).ntypes[*lw -Top + Rhs - 1] = '$';
    C2F(intersci).iwhere[*lw -Top + Rhs - 1] = *Lstk(*lw);

    if (*lw > intersiz) {
      Scierror(998,"Too many arguments in the stack edit stack.h and enlarge intersiz\r\n");
	return 1;
    }
/*     size of first entry of the mlist (type) */
    structtyp(&n1, "size");

    *retval = 1;

/*     create the mlist header */
    il = iadr(l0);
    Err = sadr(il+6) + n1 - *Lstk(Bot);
    if (Err > 0) {
	C2F(error)(&c17);
	return 1;
    }
    *istk(il) = 17;
    *istk(il+1) = 3+*nf;
    *istk(il + 2) = 1;

/*     set mlist type entry */
    l = sadr(il+6+*nf);
    structtyp(istk(iadr(l)), "set");
    *istk(il + 3) = *istk(il + 2) + n1;
    l += n1;
/*     set dims entry (int32 matrix) */
    if (!C2F(creimati)("structcreate", &l, &c4, nz, &c1, &lr, &c1,  12L)) return 1;
    C2F(icopy)(nz,sz,&c1,istk(lr),&c1);
    n1 = sadr(iadr(l)+4) + memused(c4,*nz) - l;
    *istk(il + 4) = *istk(il + 3) + n1;
    l += n1;
/*     set fields names entry (vector of strings)*/
    if (!cre_smat_from_str_i("structcreate", &l, nf, &c1, fnames, 12L ,&next)) return 1;
    n1 = next - l;
    *istk(il + 5) = *istk(il + 4) + n1;
    l += n1;
/*     set struct fields (empty lists)*/
    for (k=0;k<*nf;k++)
      {
	creemptylist(&l,&next);
	n1 = next - l;
	l += n1;
	*istk(il + 6 + k) = *istk(il + 5 + k) + n1;
      }
    *Lstk(*lw+1)=l;
} 


int creemptylist(slw,lw)
     integer *slw, *lw;
{
  integer ix1;
  integer il;
  il = iadr(*slw);
  *istk(il ) = 15;
  *istk(il + 1) = 0;
  *istk(il + 2) = 1;
  ix1 = il +  + 3;
  *lw = sadr(ix1);
  return 0;
}
int structtyp(ivt, job)
integer *ivt;
char *job;
{

    /* Local variables */
    static integer l;

/*     definition of first field  of tlist's type: struct */
/*     tlist fields are: */
/*     dims */
/*     fields */


    /* Parameter adjustments */
    --ivt;

    if (strcmp(job, "size") == 0) {
/*        size of the data structure */
	ivt[1] = 24;
    } else if (strcmp(job, "nchar") == 0) {
/*        number of chars defining the type field */
	ivt[1] = 16;
    } else if (strcmp(job, "nfield") == 0) {
/*        number of fields in the tlist */
	ivt[1] = 3;
    } else if (strcmp(job, "ptr") == 0) {
/*        pointers on individual strings */
	ivt[1] = 1;
	ivt[2] = 7;
	ivt[3] = 11;
	ivt[4] = 17;
    } else {
/*        Character string Variable header */
	ivt[1] = 10;
	ivt[2] = 1;
	ivt[3] = 3;
	ivt[4] = 0;
	ivt[5] = 1;
	l = 8;
/*        entry (1,1) = "struct" */
	ivt[l + 1] = 28;
	ivt[l + 2] = 29;
	ivt[l + 3] = 27;
	ivt[l + 4] = 30;
	ivt[l + 5] = 12;
	ivt[l + 6] = 29;
	ivt[6] = ivt[5] + 6;
	l += 6;
/*        entry (2,1) = "dims" */
	ivt[l + 1] = 13;
	ivt[l + 2] = 18;
	ivt[l + 3] = 22;
	ivt[l + 4] = 28;
	ivt[7] = ivt[6] + 4;
	l += 4;
/*        entry (3,1) = "fields" */
	ivt[l + 1] = 15;
	ivt[l + 2] = 18;
	ivt[l + 3] = 14;
	ivt[l + 4] = 21;
	ivt[l + 5] = 13;
	ivt[l + 6] = 28;
	ivt[8] = ivt[7] + 6;
	l += 6;
    }
    return 0;
} /* structtyp_ */
