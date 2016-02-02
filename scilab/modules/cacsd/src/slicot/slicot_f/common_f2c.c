/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
