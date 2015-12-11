// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r = isrow(v)
    rhs=argn(2);
    if rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isrow", 1));
    end

    s = size(v)
    s2 = s(find(s(2:$)>=1))
    r = s(1)==1 & (length(s2)==1 | prod(s2)==1)
endfunction
