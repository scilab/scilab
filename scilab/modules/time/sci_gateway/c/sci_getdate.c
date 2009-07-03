/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include "gw_time.h"
#include "api_common.h"
#include "api_double.h"
#include "api_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getdate.h"
#include "transposeMatrix.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_getdate(char *fname,unsigned long fname_len)
{
  int n1 = 0,m1 = 0;
  int * p1_in_address = NULL;
  int res = 0;
  int i=0;
  double *DATEMATRIX=NULL;

  Rhs=Max(Rhs,0);
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;

  DATEMATRIX=(double *)MALLOC( (10)*sizeof(double) );
  for (i=0;i<10;i++) DATEMATRIX[i]=0.0;

  if (Rhs == 0)
    {
      time_t dt;
      int ierr=0;
      C2F(scigetdate)(&dt,&ierr);
      if (ierr)
	{
	  Scierror(999,_("%s: An error occurred: %s\n"),fname,strerror(ierr));
	  return 0;
	}
      else
	{
	  C2F(convertdate)(&dt,DATEMATRIX);
	  m1=1;
	  n1=10;
	  res = createMatrixOfDouble(Rhs+1, m1, n1, DATEMATRIX);
	}

    }
  else /* Rhs == 1 */
    {
      getVarAddressFromPosition(1, &p1_in_address);

      if (getVarType(p1_in_address) == sci_strings)
	{
	  char *Param1=NULL;
	  int size_Param1 = 0;

	  res = getMatrixOfString(p1_in_address, &m1, &n1, &size_Param1, NULL);
	  Param1 = (char *)MALLOC(size_Param1*sizeof(char));
	  res = getMatrixOfString(p1_in_address, &m1, &n1, &size_Param1, &Param1);

	  if (strcmp("s",Param1)==0)
	    {
	      time_t dt;
	      int ierr=0;
	      C2F(scigetdate)(&dt,&ierr);
	      DATEMATRIX[0]=(int)dt;
	      m1=1;
	      n1=1;
	      res = createMatrixOfDouble(Rhs+1, m1, n1, DATEMATRIX);
	      if (Param1) FREE(Param1);
	    }
	  else
	    {
	      Scierror(999,_("%s: Wrong type for input argument #%d: Integer or '%s' expected.\n"),fname,1,"s");
	      if (Param1) FREE(Param1);
	      return 0;
	    }
	}
      else
	{
	  if ( getVarType(p1_in_address) == sci_matrix )
	    {
	      int li=0;
	      int k=0;
	      int l=0;
	      double *param=NULL;

	      double *DATEARRAY=NULL;
	      double *DATEARRAYtmp=NULL;

	      res = getMatrixOfDouble(p1_in_address, &m1, &n1, &param);

	      l=10*m1*n1;
	      DATEARRAY=(double *)MALLOC( (l)*sizeof(double) );
	      for (k=0;k<l;k++) DATEARRAY[k]=0.0;

	      for(li=0;li<m1*n1;li++)
		{
		  int j=0;
		  time_t paramtemp=(int)param[li];
		  double millisecondes=param[li]-paramtemp;
		  C2F(convertdate)(&paramtemp,DATEMATRIX);
		  for (j=0;j<10;j++)
		    {
		      DATEARRAY[(li*10)+j]=DATEMATRIX[j];
		    }
		  if (millisecondes>0)
		    {
		      if (millisecondes>0.999) millisecondes=0.999;
		      DATEARRAY[(li*10)+9]=(int)(millisecondes*1000);
		    }
		}

	      m1=l/10;
	      n1=10;
	      DATEARRAYtmp=DATEARRAY;
	      DATEARRAY = transposeMatrixDouble(n1,m1,DATEARRAY);
	      res = createMatrixOfDouble(Rhs+1, m1, n1, DATEARRAY);

	      LhsVar(1)=Rhs+1;
	      C2F(putlhsvar)();

	      if (DATEMATRIX) {FREE(DATEMATRIX);DATEMATRIX=NULL;}
	      if (DATEARRAY) {FREE(DATEARRAY);DATEARRAY=NULL;}
	      if (DATEARRAYtmp) {FREE(DATEARRAYtmp);DATEARRAYtmp=NULL;}

	      return 0;
	    }
	  else
	    {
	      Scierror(999,_("%s: Wrong type for input argument #%d: Integer or '%s' expected.\n"),fname,1,"s");
	      return 0;
	    }
	}
    }

  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();

  if (DATEMATRIX) {FREE(DATEMATRIX);DATEMATRIX=NULL;}

  return 0;
}
/*--------------------------------------------------------------------------*/
