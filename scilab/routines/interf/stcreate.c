#include "../stack-c.h"
#define memused(it,mn) ((((mn)*( it % 10))/sizeof(int))+1)
#include "../stack-c.h"

/* Table of constant values */
static integer c17 = 17;
static integer c1 = 1;
static integer c4 = 4;

int C2F(stcreate)(lw, nz, sz, nf, fnames, retval)
integer *lw, *nz, *sz, *nf, *retval;
char *fnames[];
{
    integer next,k,lr,nels;

    static integer l;
    static integer l0, n1, il;

    *retval = 0;
    l0 = *lstk(*lw);

    C2F(intersci).ntypes[*lw -Top + Rhs - 1] = '$';
    C2F(intersci).iwhere[*lw -Top + Rhs - 1] = *lstk(*lw);

    if (*lw > intersiz) {
      Scierror(998,"Too many arguments in the stack edit stack.h and enlarge intersiz\r\n");
	return 1;
    }
    sttyp(&n1, "size", nf, fnames);
    
    *retval = 1;
/*     create the mlist header */
    il = iadr(l0);
    Err = sadr(il+6) + n1 - *lstk(Bot);
    if (Err > 0) {
	C2F(error)(&c17);
	return 1;
    }
    *istk(il) = 17;
    *istk(il+1) = 2+*nf;
    *istk(il + 2) = 1;
/*     set mlist type ["st","dims","field1",...,"fieldnf"] */
    l = sadr(il+5+*nf);
    sttyp(istk(iadr(l)), "set", nf, fnames);
    *istk(il + 3) = *istk(il + 2) + n1;
    l += n1;
/*     set dims entry (int32 matrix) */
    if (!C2F(creimati)("stcreate", &l, &c4, nz, &c1, &lr, &c1,  12L)) return 1;
    C2F(icopy)(nz,sz,&c1,istk(lr),&c1);
    n1 = sadr(iadr(l)+4) + memused(c4,*nz) - l;
    *istk(il + 4) = *istk(il + 3) + n1;
    l += n1;
    
/*     set struct fields */
    nels=1;
    for (k=0; k< *nz;k++) nels=nels*sz[k];
    if (nels==1) {
    for (k=0; k< *nf;k++)
      {
	creonevoid(&l,&next);
	n1 = next - l;
	l += n1;
	*istk(il + 5 + k) = *istk(il + 4 + k) + n1;
      }
    }
    else {
    for (k=0; k< *nf;k++)
      {
	crelistofvoids(&l,&next,&nels);
	n1 = next - l;
	l += n1;
	*istk(il + 5 + k) = *istk(il + 4 + k) + n1;
      }
    }
    *lstk(*lw+1)=l;
    return 1; 
} 
int creonevoid(slw,lw)
     integer *slw, *lw;
{
  int il;
  il = iadr(*slw);
  *istk(il  )=1;
  *istk(il+1)=0;
  *istk(il+2)=0;
  *istk(il+3)=0;
  il += 4;
  *lw = sadr(il);
  return 0;
}
int crelistofvoids(slw,lw,nels)
     integer *slw, *lw, *nels;
{
  int il,k;
  il = iadr(*slw);
  *istk(il) = 15;
  *istk(il + 1) = *nels;
  *istk(il + 2) = 1;
  for (k=0; k<*nels; k++) *istk(il+3+k) = *istk(il+2+k) + 2;
  il += 3+*nels;
  il=iadr(sadr(il));
  for (k=0; k<*nels; k++) {
    *istk(il  )=1;
    *istk(il+1)=0;
    *istk(il+2)=0;
    *istk(il+3)=0;
    il += 4;
  };
  if ( ((il-iadr(*slw)) %2) ) il +=1;
  *lw =  sadr(il);
  return 0;
}

int sttyp(ivt, job, nf, fnames)
     int *nf;     int *ivt;     char *job;     char *fnames[];
{
  static int l; int ls,k,shft,stp,Job;

    if (strcmp(job, "size") == 0) {
/*        size of header = ["st","dims","f_1",...,"f_nf"] */
      ls=0;
      for ( k= 0; k < *nf; k++) ls += strlen(fnames[k]);
      ivt[0] = 4 + (2 +*nf) + 1 + 2 + 4 + ls;  /* 2 + *nf strings */
      /* if (ivt[0] % 2) ivt[0]++;  */
    } else {
/*      header = row vector made of 2 + *nf strings */
	ivt[0] = 10;
	ivt[1] = 1;
	ivt[2] = 2 + *nf;
	ivt[3] = 0;
	ivt[4] = 1;
	stp=4;
	l = 5 + (2 + *nf);
/*       string 1 =  "st" */
	shft=2;
	ivt[l] = 28;      /*  "s"   */
	ivt[l + 1] = 29;  /*  "t"   */
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
/*        string 2 =  "dims" */
	shft=4;
	ivt[l] = 13;      /* "d"   */
	ivt[l + 1] = 18;  /* "i"   */
	ivt[l + 2] = 22;  /* "m"   */
	ivt[l + 3] = 28;  /* "s"   */
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
/*        strings 3:2+nf = ["field_1","field_2","field_nf"]    */
	for (k=0; k < *nf; k++) {
	shft = strlen(fnames[k]);
	C2F(cvstr)(&shft,&ivt[l],fnames[k],(Job=0,&Job),shft);
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
	}
    }
    return 0;
}
