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
function [r,ind]=aplat(l,r)
    //flattens a list. If l is constant it puts it in a list
    //ind contains the list structure
    if type(l)<>15 then r=list(l);ind=-1;return;end
    n=size(l)
    [lhs,rhs]=argn(0)
    if rhs==1 then r=list(),nr=0,end
    ind=list()
    i=0
    nind=0
    for li=l
        i=i+1
        if type(li)==15 then
            [r,ind1]=aplat(li,r)
            ni=size(ind1)
            for j=1:ni,nind=nind+1;ind(nind)=[i,ind1(j)];end
            nr=size(r)
        else
            nr=nr+1
            r(nr)=li
            nind=nind+1
            ind(nind)=i
        end
    end
endfunction
