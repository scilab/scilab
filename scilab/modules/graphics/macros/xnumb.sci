// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
