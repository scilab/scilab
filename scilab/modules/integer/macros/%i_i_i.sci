// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998-2011 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function left=%i_i_i(varargin)
    //- insertion of an integer matrix in an matrix of integers
    //            for more than 2 indices
    // -insertion an integer matrix in an integer matrix which has a
    //            different integer type

    left  = varargin($);
    right = varargin($-1);

    right = iconvert(rigth, inttype(left));

    left(varargin(1:$-2)) = right;
endfunction
