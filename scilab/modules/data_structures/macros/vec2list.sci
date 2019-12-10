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
//
function li=vec2list(bigVector,varsizes,ind)
    //bigVector: big vector
    //varsizes: k x 2 matrix, varsizes(i,:)=size of ith matrix
    //li: list of k matrices, li(i)=matrix of size varsizes(i,:);

    [LHS, RHS] = argn(0)
    if RHS < 2 then
        error(sprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "vec2list", 2, 3));
    end

    if bigVector==[] then
        n=0;for dimi=varsizes',n=n+prod(dimi);end
        bigVector=zeros(n,1);
    end
    li=list();point=1;i=0;
    for dimi=varsizes'
        newpoint=point+prod(dimi)-1;i=i+1;
        li(i)=matrix(bigVector(point:newpoint),dimi(1),dimi(2));
        point=newpoint+1;
    end
    if RHS==3 then li=recons(li,ind); end
endfunction
