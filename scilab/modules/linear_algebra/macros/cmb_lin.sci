
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [x]=cmb_lin(alfa,x,Beta,y)
    //  x =cmb_lin(alfa,x,Beta,y) evaluates alfa*x-Beta*y
    // (low-level routine)
    //!
    n=prod(size(x));
    for j=1:n,
        x(j)=addf(mulf(alfa,x(j)),mulf(mulf("-1",Beta),y(j)));
    end
endfunction
