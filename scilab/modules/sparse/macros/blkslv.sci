// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function rhs = blkslv(spcho,rhs)
    // Private utility shared by chfact() and chsolve()
    //
    //[xlnz,nnzl,xsuper,xlindx,lindx,snode,split,tmpsiz,perm,invp,lnz]=spcho(2:12);
    xsuper = spcho("xsuper");
    nsuper = size(xsuper,1)-1;
    neqns = size(rhs,1);
    rhs = blkslvi(nsuper,xsuper,spcho("xlindx"),spcho("lindx"),spcho("xlnz"),...
    spcho("lnz"),rhs);
endfunction
