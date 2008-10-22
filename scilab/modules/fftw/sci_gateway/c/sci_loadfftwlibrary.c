/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sci_loadfftwlibrary.h"
#include "callfftw.h"
#include "Scierror.h"
#include "fftwlibname.h"
#include "gw_fftw.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
int sci_loadfftwlibrary(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	char *FFTWLibname=NULL;

	CheckRhs(1,1);

	if (GetType(1) == sci_strings)
	{
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		FFTWLibname=cstk(l1);
		setfftwlibname(FFTWLibname);

		n1=1;
		if ( LoadFFTWLibrary(FFTWLibname) )
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		 Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
	}
	return(0);
}
/*--------------------------------------------------------------------------*/ 
