/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/* ONLY used by F2C with scilab_f2c.sln on Windows */
/* this modification removes some warning about no defined or redefined COMMON */
/* We force definition of COMMON only used in current dynamic library */
/*--------------------------------------------------------------------------*/ 
/* see fortran code for definition of this COMMON */
#ifdef _MSC_VER
struct 
{
    long int iero;
} ierinv_;
#endif
/*--------------------------------------------------------------------------*/ 
