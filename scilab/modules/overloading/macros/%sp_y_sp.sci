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

function r=%sp_y_sp(a,b)
    //  a./.b with a and b sparse

    [ija,va,mna]=spget(a)
    [ijb,vb,mnb]=spget(b)
    if size(ijb,1)<>prod(mnb)|or(vb==0) then error(27),end
    ia=ija(:,1);ja=ija(:,2)
    ib=ijb(:,1);jb=ijb(:,2)

    ij=[((ia-ones(ia))*mnb(1)).*.ones(ib)+ones(ia).*.ib,..
    ((ja-ones(ja))*mnb(2)).*.ones(jb)+ones(ia).*.jb]
    r=sparse(ij,va./.vb,mna.*mnb)
endfunction
