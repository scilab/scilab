/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_hdf5.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
/*  interface function */
/*--------------------------------------------------------------------------*/ 
#define HDF5_TAB_SIZE 2
static gw_generic_table Tab[HDF5_TAB_SIZE]={
	{sci_export_to_hdf5,"export_to_hdf5"},
	{sci_import_from_hdf5,"import_from_hdf5"}
	};
/*--------------------------------------------------------------------------*/ 
int gw_hdf5(void)
{
	Rhs = Max(0,Rhs);
	callFunctionFromGateway(Tab, HDF5_TAB_SIZE);
	return 0;
}
/*--------------------------------------------------------------------------*/ 

