
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [w,rk]=colcomp(a,flag,tol)
    //Syntaxes : [w,rk]=colcomp(a)
    //           [w,rk]=colcomp(a,flag)
    //           [w,rk]=colcomp(a,flag,tol)
    //
    //column compression of a i.e. comput. of ker(a)
    //flag and tol are optional parameters
    //flag='qr' or 'svd' (defaut 'svd')
    //tol tolerance parameter (of order %eps as defaut value)
    //the ma-rk first columns of w span the kernel of a when size(a)=(na,ma)

    [ma,na]=size(a)
    [lhs,rhs]=argn(0)
    if a==[] then w=[];rk=0;return;end
    if norm(a,1) < sqrt(%eps)/10 then rk=0,w=eye(na,na),return,end
    if rhs ==2 then tol=sqrt(%eps)*norm(a,1)*max(ma,na),end
    if rhs==1 then flag="svd",tol=sqrt(%eps)*norm(a,1)*max(ma,na);end
    select flag
    case "qr" then [q,r,rk,e]=qr(a',tol);
        //w=[q(:,rk+1:ma),q(:,1:rk)]; <-- le  ma me parait suspect je met na
        w=q(:,na:-1:1)
    case "svd" then [u,s,v,rk]=svd(a',tol);
        //w=[u(:,rk+1:na),u(:,1:rk)];
        w=u(:,na:-1:1)
    end
endfunction
