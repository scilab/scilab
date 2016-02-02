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
function [r,ind]=recons(r,ind)
    //reconstruct a list from a flat list (see aplat)
    if ind==-1 then return;end
    nr=size(r)
    ma=0
    for k=nr:-1:1
        mm=size(ind(k),"*");
        if ma<=mm then ma=mm;ki=k; end
    end

    if ma<=1 then return; end
    vi=ind(ki);vi=vi(1:ma-1);
    k=ki
    vj=vi

    while vj==vi
        k=k+1
        if k>nr then break; end
        vv=ind(k);
        if size(vv,"*")==ma then vj=vv(1:ma-1); else vj=[]; end
    end
    kj=k-1
    rt=list(r(ki))
    for k=ki+1:kj
        rt(k-ki+1)=r(ki+1)
        r(ki+1)=null()
        ind(ki+1)=null()
    end
    ind(ki)=vi
    r(ki)=rt
    [r,ind]=recons(r,ind)

endfunction
