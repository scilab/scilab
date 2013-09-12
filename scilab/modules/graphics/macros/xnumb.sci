// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function xnumb(x,y,nums,box,angle)
    if and(argn(2)<>[3:5]) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "xnumb", 3, 5));
    end

    n=size(x,"*");
    if argn(2)<5 then angle=zeros(1,n),end
    if argn(2)<4 then box=0,end

    if size(y,"*") >n|size(nums,"*" )<>n|size(angle,"*" )<>n then
        error(msprintf(_("%s: Wrong size for input arguments: Same sizes expected.\n"),"xnumb"));
    end

    for k=1:n
        xstring(x(k),y(k),msprintf("%-5.4g",nums(k)),angle(k),box)
    end
endfunction
