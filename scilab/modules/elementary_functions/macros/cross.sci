// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function c=cross(a,b)
    // This function returns the cross product of a and b
    rhs = argn(2);
    if rhs <> 2 then
        error(msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "cross",2));
    end
    if type(a) > 8 | type(a) == 6 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A real, complex, boolean or polynomial matrix expected.\n"), "cross", 1));
    end
    if type(b) > 8 | type(b) == 6 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A real, complex, boolean or polynomial matrix expected.\n"), "cross", 2));
    end

    if size(a,1) <> size(b,1) | size(a,2)<>size(b,2) then
        error(msprintf(_("%s: Wrong size for input arguments: Same sizes expected.\n"), "cross"));
    end
    if (size(a,1)<>1 | size(a,2)<>3) & size(a,1)<>3 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A matrix of size 1x3 or 3xN expected.\n"), "cross", 1));
    end

    if size(a,2)==3 & size(a,1)==1 then
        c=a(:,[2 3 1]).*b(:,[3 1 2]) - a(:,[3 1 2]).*b(:,[2 3 1]);
    else
        c=a([2 3 1],:).*b([3 1 2],:) - a([3 1 2],:).*b([2 3 1],:);
    end
endfunction
