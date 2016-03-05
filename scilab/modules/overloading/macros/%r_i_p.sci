// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function f2=%r_i_p(varargin)
    // /author Serge Steer INRIA
    //
    f2=varargin($-1)
    n= varargin($)
    // f2=%r_i_p(i,j,f2,p)   insertion
    d=ones(n);
    n(varargin(1:$-2))=f2.num;
    d=ones(n);
    d(varargin(1:$-2))=f2.den;

    f2=rlist(n,d,f2("dt"))
endfunction
