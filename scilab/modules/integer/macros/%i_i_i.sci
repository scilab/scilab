// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998-2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function left=%i_i_i(varargin)
    //- insertion of an integer matrix in an matrix of integers
    //            for more than 2 indices
    // -insertion an integer matrix in an integer matrix which has a
    //            different integer type

    left  = varargin($);
    rigth = varargin($-1);

    rigth = iconvert(rigth, inttype(left));

    left(varargin(1:$-2)) = rigth;
endfunction
