// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function rep = %r_0(R)
    s = poly(0, varn(R(2)(1)))
    rep = ones(R).' ./ s ;
    rep(2) = R(2).'
    rep(3) = R(3).';
endfunction
