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
//=============================================================================
namespace DotNetScilab
{
    public enum ScilabType
    {
        sci_matrix = 1,
        sci_poly = 2,
        sci_boolean = 4,
        sci_sparse = 5,
        sci_boolean_sparse = 6,
        sci_matlab_sparse = 7, /* matlab sparse matrix  */
        sci_ints = 8,
        sci_handles = 9,
        sci_strings = 10,
        sci_u_function = 11,
        sci_c_function = 13,
        sci_lib = 14,
        sci_list = 15,
        sci_tlist = 16,
        sci_mlist = 17,
        sci_pointer = 128, 
        sci_implicit_poly = 129,
        sci_intrinsic_function = 130
    };
}
//=============================================================================