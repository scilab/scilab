// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
