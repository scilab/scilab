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
function a=%b_i_spb(i,j,b,a)
    // %spis(i,j,b,a) insert full matrix b into sparse matrix a for some special cases
    // a(i,j)=b
    //!

    [lhs,rhs]=argn(0)
    if rhs==3 then
        a=b;
        b=j;
        [m,n]=size(a)
        a=a(:)
        a(i)=b
    end
    [ij,v]=spget(a)
    if ij==[] then
        a=sparse([],[],[m,n])
    else
        j=int((ij(:,1)-1)/m)+1
        i=ij(:,1)-m*(j-1)
        a=sparse([i j],v,[m,n])
    end
endfunction
