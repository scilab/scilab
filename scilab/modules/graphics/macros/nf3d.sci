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

function [xx,yy,zz]=nf3d(x,y,z)
    // 3d coding transformation
    // from facets coded in three matrices x,y,z to scilab code for facets
    // accepted by plot3d
    //---------------------------------------------------------
    if argn(2)<>3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "nf3d", 3));
    end


    [n1,n2]=size(x)
    ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
    // ind=[1,2,n1+2,n1+1 , 2,3,n1+3,n1+2, ....  ,n1-1,n1,2n1,2n1-1
    ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);
    nx=prod(size(ind2))
    xx=matrix(x(ind2),4,nx/4);
    yy=matrix(y(ind2),4,nx/4);
    zz=matrix(z(ind2),4,nx/4);
endfunction
