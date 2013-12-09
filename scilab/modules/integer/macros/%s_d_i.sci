// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) 2013 - Samuel GOUGEON : http://bugzilla.scilab.org/13000
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function r = %s_d_i(a,b)
    if a==[] then
        r = []
    else
        r = iconvert(a,inttype(b))./b
    end
endfunction
