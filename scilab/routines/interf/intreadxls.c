#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../machine.h"

#include "../stack-c.h"

#ifdef _MSC_VER
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


/*---------------------------------------------------------------
  Authors Pierrick Mode, Serge Steer INRIA 2005, Copyright INRIA
  --------------------------------------------------------------*/

extern int ripole(char *inputfile, char *outputfile, int debug, int verbose);
extern void GetenvB( char *name,char *env, int len);
extern void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);
extern int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout);

/*Prototype*/
void xls_read(int *fd, int *cur_pos,double **data, int **chainesind, int *N, int *M, int *err);
void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos,int *nsheets);
/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/



int C2F(intreadxls)(char *fname, long lfn)
{
  int m1,n1,l1,zero=0,ierr;
  double *data=NULL;
  int *ind=NULL;
  int M,N,MN;
  int pos,fd;

  CheckLhs(2,2);
  CheckRhs(2,2);

  /*  checking variable Pos */
  GetRhsVar(1,"d",&m1,&n1,&l1);
  fd=(int)*stk(l1);
  GetRhsVar(2,"d",&m1,&n1,&l1);
  pos=(int)*stk(l1);

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
      CreateVarFromPtr(Rhs+1, "d", &N,&M, &data);
      CreateVarFromPtr(Rhs+2, "i", &N,&M, &ind);
      FREE(data);
      FREE(ind);
    }


  LhsVar(1)= Rhs+1;
  LhsVar(2)= Rhs+2;
  C2F(putlhsvar)();

  return 0;
}

#include "../fileio/ole.h"
static char *xls_basename (name)
char *name;
{
  char *base=NULL;
#ifdef _MSC_VER
  base = strrchr (name, '\\');
#else
  base = strrchr (name, '/');
#endif

  return base ? base + 1 : name;
}

int C2F(intopenxls)(char *fname, long lfn)
{
#undef IN
  int i,k,m1,n1,l1,l2,one=1,fd,f_swap=0;
  int ierr,ns,result;
  double res;
  char **sst=NULL;
  char **Sheetnames=NULL;
  int *Abspos=NULL;
  int nsheets;
  char IN[256],TMP[256];
  char sep[2];
#ifdef _MSC_VER
  sep[0]='\\';
#else
  sep[0]='/';
#endif
  sep[1]='\0';

  CheckLhs(4,4);
  CheckRhs(1,1);

  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  C2F(cluni0)(cstk(l1), IN, &ns,(long int) (m1*n1),256L);

  strcpy(TMP,getenv("TMPDIR"));
  
  strcat(TMP,sep);
  strcat(TMP,xls_basename(IN));
  if (C2F(iop).ddt==-10) 
    result=ripole(IN, TMP, 1, 1);
  else
    result=ripole(IN, TMP, 0, 0);
   if (result != OLE_OK) {
    if (result == OLEER_NO_INPUT_FILE)
      Scierror(999,"%s :There is no  file %s \r\n","xls_open",IN);
    else if(result == -1)
      Scierror(999,"%s :file %s  cannot be opened\r\n","xls_open",IN);
    else if(result == OLEER_NOT_OLE_FILE)
      Scierror(999,"%s :file %s is not an ole2 file\r\n","xls_open",IN);
    else if(result == OLEER_INSANE_OLE_FILE)
      Scierror(999,"%s :file %s is  an insane ole2 file\r\n","xls_open",IN);
    else if(result == OLEER_MINIFAT_READ_FAIL)
      Scierror(999,"%s :file %s error when reading minifat\r\n","xls_open",IN);
    else if(result == OLEER_PROPERTIES_READ_FAIL)
      Scierror(999,"%s :file %s error when reading ole properties\r\n","xls_open",IN);
    else if(result == OLEER_MEMORY_OVERFLOW)
      Scierror(999,"%s :file %s memory overflow when analysing ole stream\r\n","xls_open",IN);
    else if(result == OLEER_MINISTREAM_READ_FAIL)
      Scierror(999,"%s :file %s error when reading ole ministream\r\n","xls_open",IN);
    else if(result == OLEER_NORMALSTREAM_STREAM_READ_FAIL)
      Scierror(999,"%s :file %s error when reading ole stream\r\n","xls_open",IN);
    else
      Scierror(999,"%s :file %s  ripole converter problem=%d\r\n","xls_open",IN,result);
    return 0;
  }
  strcat(TMP,sep);
  strcat(TMP,"Workbook");
  C2F(mopen)(&fd, TMP,"rb", &f_swap, &res, &ierr);
  if (ierr != 0)
    {
      Scierror(999,"%s :There is no xls stream in the ole2 file %s \r\n","xls_open",IN);
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
      Scierror(999,"%s :Not an ole2 file \r\n","xls_open");
      return 0;
    }
  else  if (ierr == 2)
    {
      Scierror(999,"%s : the file has no Workbook directory \r\n","xls_open");
      return 0;
    }
 else  if (ierr == 3)
    {
      Scierror(999,"%s : impossible to alloc enough memory \r\n","xls_open");
      return 0;
    }
  else  if (ierr == 4)
    {
      Scierror(990,"%s : incorrect or corrupted file \r\n","xls_open");
      return 0;
    }
  else  if ((ierr == 3)||(ierr == 5))
    {
      Scierror(999,"%s : Only BIFF8 file format is handled \r\n","xls_open");
      return 0;
    }

  if (ns != 0) 
  {
    /* Create a typed list to return the properties */
    CreateVarFromPtr(Rhs+2,"S", &one, &ns, sst);
    for (k=0;k<ns;k++) 
	{
		if ( (sst) && (sst[k]))
		{
			FREE(sst[k]);
			sst[k]=NULL;
		}
	}
	if (sst)
	{
		FREE(sst);
		sst=NULL;
	}
  }
  else
    CreateVar(Rhs+2,"d",&ns,&ns,&l2);

  if (nsheets != 0) 
  {
    /* Create a typed list to return the properties */
    CreateVarFromPtr(Rhs+3,"S", &one, &nsheets, Sheetnames);
	
	if (Sheetnames)
	{
		for (k=0;k<nsheets;k++) 
		{
			if (Sheetnames[k])
			{
				FREE(Sheetnames[k]);
				Sheetnames[k]=NULL;
			}
		}
		FREE(Sheetnames);
		Sheetnames=NULL;
	}

    
    
    CreateVar(Rhs+4,"d", &one, &nsheets, &l2);
    for (i=0;i<nsheets;i++) *stk(l2+i)=Abspos[i];
	if (Abspos)
	{
		FREE(Abspos);
		Abspos=NULL;
	}
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

