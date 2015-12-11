// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Samuel GOUGEON - 2015 - sgougeon@free.fr : Bug 13893
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function h = %r_simp(h)
    // implement complex case
    // 0/den => 0/1
    n = numer(h)
    d = denom(h)
    i = find(clean(n)==0 & clean(d)~=0)
    d(i) = 1
    h = rlist(n, d, h(4))
endfunction
