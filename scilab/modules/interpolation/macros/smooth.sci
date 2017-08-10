//
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
function [pt]=smooth(ptd,pas)
    [lhs,rhs]=argn(0)
    [m,n]=size(ptd)
    d=splin(ptd(1,:),ptd(2,:))
    if rhs==1 then l=abs(ptd(1,n)-ptd(1,1));pas=l/100;end
    pt=[ptd(1,1)+pas:pas:ptd(1,n)]
    pt=[ptd(:,1) [pt;interp(pt,ptd(1,:),ptd(2,:),d)] ptd(:,n)]
endfunction
