// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a=%spb_i_b(i,j,b,a)
    // %spis(i,j,b,a) insert a sparse boolean matrix b into full boolean matrix a
    // a(i,j)=b
    //!

    a(i,j)=full(b)
endfunction
