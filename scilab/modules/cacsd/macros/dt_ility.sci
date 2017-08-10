// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [m1,m2,u2,sl2]=dt_ility(sl,tol)
    //dual of st_ility
    //detectability means m1=0;
    //m1=dimension of unstable,unobservable subspace
    //m2=dimension of unobservable subspace; m2>=m1
    //sl2=ss2ss(sl,u2)
    //!

    [LHS,RHS]=argn(0)
    if RHS==2 then
        [n1,n2,u1,sl1]=st_ility(sl',tol);
    else
        [n1,n2,u1,sl1]=st_ility(sl');
    end
    [nx,nx]=size(sl(2));
    u2=[u1(:,n1+1:nx),u1(:,n2+1:n1),u1(:,1:n2)];
    sl2=ss2ss(sl,u2);
    m1=nx-n1;m2=nx-n2;
endfunction
