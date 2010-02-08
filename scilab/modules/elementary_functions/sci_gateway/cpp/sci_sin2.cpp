/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "elem_func_gw.hxx"

extern "C"
{
	#include "sin.h"
}

using namespace types;

Function::ReturnValue sci_sin(types::typed_list &in, int* _piRetCount, types::typed_list &out)
{
	double *pDataInR	= NULL;
	double *pDataInI	= NULL;
	double *pDataOutR = NULL;
	double *pDataOutI = NULL;
	Double *pRetVal		= NULL;

	if(in.size() != 1)
	{
		return Function::Error;
	}

	if(in[0]->getType() != types::InternalType::RealDouble)
	{
		return Function::Error;
	}


	Double *pIn				= in[0]->getAsDouble();
	if(pIn->isComplex())
	{
		pDataInR	=	pIn->real_get();
		pDataInI	=	pIn->img_get();

		pRetVal = new Double(pIn->rows_get(), pIn->cols_get(), &pDataOutR, &pDataOutI);

		for(int i = 0 ; i < pIn->size_get() ; i++)
		{
			zsins(pDataInR[i], pDataInI[i], &pDataOutR[i], &pDataOutI[i]);
		}
	}
	else
	{
		pDataInR	=	pIn->real_get();
		pRetVal = new Double(pIn->rows_get(), pIn->cols_get(), &pDataOutR);
		for(int i = 0 ; i < pIn->size_get() ; i++)
		{
			pDataOutR[i] = dsins(pDataInR[i]);
		}
	}

	out.push_back(pRetVal);
	*_piRetCount = 1;
	return Function::OK;
}

