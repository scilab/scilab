#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../machine.h"


#include "../stack-c.h"

/*---------------------------------------------------------------
  Authors Pierrick Mode, Serge Steer INRIA 2005, Copyright INRIA
  --------------------------------------------------------------*/

extern int ripole(char *inputfile, char *outputfile, int debug, int verbose);
extern void GetenvB( char *name,char *env, int len);

/*Prototype*/
void xls_read(int *fd, int *cur_pos,double **data, int **chainesind, int *N, int *M, int *err);
void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos,int *nsheets);
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/



int C2F(intreadxls)(char *fname, long lfn)
{
  int m1,n1,l1,zero=0,ierr;
  double *data;
  int *ind;
  int M,N,MN;
  int pos,fd;

  CheckLhs(2,2);
  CheckRhs(2,2);

  /*  checking variable Pos */
  GetRhsVar(1,"d",&m1,&n1,&l1);
  fd=*stk(l1);
  GetRhsVar(2,"d",&m1,&n1,&l1);
  pos=*stk(l1);

  xls_read(&fd,&pos,&data, &ind, &N, &M,  &ierr);
  if (ierr == 1)
    {
      Scierror(999,"%s :Not enough memory to allocate results \r\n",fname);
      return 0;
    }
  else if(ierr == 2)
    {
      Scierror(999,"%s :Failed to read expected data, may be invalid xls file \r\n",fname);
      return 0;
    }
  else if(ierr == 3)
    {
      Scierror(999,"%s :End of File \r\n",fname);
      return 0;
    }

  /*  sciprint("readxls %d %d %d\n",M,N,ierr);*/

  MN=M*N;
  if (MN==0)
    {
      CreateVar(Rhs+1, "d", &zero,&zero, &l1);
      CreateVar(Rhs+2, "d", &zero,&zero, &l1);
    }

  else
    {
      CreateVarFromPtr(Rhs+1, "d", &M,&N, &data);
      CreateVarFromPtr(Rhs+2, "i", &M,&N, &ind);
      free(data);
      free(ind);
    }


  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  C2F(putlhsvar)();

  return 0;
}

#include <libgen.h>
#include "../sound/ole.h"

int C2F(intopenxls)(char *fname, long lfn)
{
  int i,k,m1,n1,l1,l2,one=1,fd,f_swap=0;
  int ierr,ns,result;
  double res;
  char **sst;
  char **Sheetnames;
  int *Abspos;
  int nsheets;
  int opt;
  char in[256],TMP[256];

  CheckLhs(4,4);
  CheckRhs(1,1);

  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);

  strcpy(in,cstk(l1));
  strcpy(TMP,getenv("TMPDIR"));
  strcat(TMP,"/");
  strcat(TMP,basename(in));
  result=ripole(cstk(l1), TMP, 0, 0);
  if (result != OLE_OK) {
    if (result == OLEER_NO_INPUT_FILE)
      Scierror(999,"%s :There is no  file %s \r\n",fname,cstk(l1));
    else if(result == OLEER_NOT_OLE_FILE || 
	    result == OLEER_INSANE_OLE_FILE || 
	    result == OLEER_LOADFAT_BAD_BOUNDARY || 
	    result == OLEER_MINIFAT_READ_FAIL || 
	    result == OLEER_PROPERTIES_READ_FAIL)
      Scierror(999,"%s :file %s is not an ole2 file\r\n",fname,cstk(l1));
    else if(result == -1)
      Scierror(999,"%s :file %s exists but cannot be opened\r\n",fname,cstk(l1));

    return 0;
  }
  strcat(TMP,"/");
  strcat(TMP,"Workbook");
  C2F(mopen)(&fd, TMP,"rb", &f_swap, &res, &ierr);
  if (ierr != 0)
    {
      Scierror(999,"%s :There is no xls stream in the ole2 file %s \r\n",fname,cstk(l1));
      return 0;
    }
  CreateVar(Rhs+1,"i",&one,&one,&l2);
  *istk(l2)=fd; /* logical unit */

  xls_open(&ierr, &fd, &sst ,&ns, &Sheetnames, &Abspos,&nsheets);
  /*return *err:
    0 = OK
    1 = not an OLE file
    2 = no Workbook included
    3 = memory allocation problem
    4 = incorrect file
    5 = not a BIFF8 xls file
   */
  if (ierr == 1)
    {
      Scierror(999,"%s :Not an ole2 file \r\n",fname);
      return 0;
    }
  else  if (ierr == 2)
    {
      Scierror(999,"%s : the file has no Workbook directory \r\n",fname);
      return 0;
    }
 else  if (ierr == 3)
    {
      Scierror(999,"%s : impossible to alloc enough memory \r\n",fname);
      return 0;
    }
  else  if (ierr == 4)
    {
      Scierror(990,"%s : incorrect or corrupted file \r\n",fname);
      return 0;
    }
  else  if (ierr == 3)
    {
      Scierror(999,"%s : Only BIFF8 file format is handled \r\n",fname);
      return 0;
    }

  if (ns != 0) {
    /* Create a typed list to return the properties */
    CreateVarFromPtr(Rhs+2,"S", &one, &ns, sst);
    for (k=0;k<ns;k++) free(sst[k]);
    free(sst);}
  else
    CreateVar(Rhs+2,"d",&ns,&ns,&l2);

  if (nsheets != 0) {
    /* Create a typed list to return the properties */
    CreateVarFromPtr(Rhs+3,"S", &one, &nsheets, Sheetnames);
    for (k=0;k<nsheets;k++) free(Sheetnames[k]);
    free(Sheetnames);
    CreateVar(Rhs+4,"d", &one, &nsheets, &l2);
    for (i=0;i<nsheets;i++) *stk(l2+i)=Abspos[i];
    free(Abspos);
  }
  else {
    CreateVar(Rhs+3,"d",&nsheets,&nsheets,&l2);
    CreateVar(Rhs+4,"d",&nsheets,&nsheets,&l2);
  }

  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  LhsVar(3)= Rhs+3;
  LhsVar(4)= Rhs+4;
  C2F(putlhsvar)();
  return 0;
}

