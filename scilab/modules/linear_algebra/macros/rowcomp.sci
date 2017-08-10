
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1987-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [w,rk]=rowcomp(A,flag,tol)
    //Row compression of A <--> computation of im(A)
    //flag and tol are optional parameters
    //flag='qr' or 'svd' (default 'svd')
    //tol tolerance parameter (sqrt(%eps)*norm(A,1) as default value)
    //the rk first (top) rows of w span the row range of a
    //the rk first columns of w' span the image of a

    if A==[] then w=[];rk=0;return;end

    [ma,na]=size(A)
    rhs=argn(2)

    if norm(A,1) < sqrt(%eps)/10 then rk=0,w=eye(ma,ma),return;end

    select rhs
    case 1 then// default values for flag and tol
        flag="svd",tol=sqrt(%eps)*norm(A,1);
    case 2 then //default value for tol
        tol=sqrt(%eps)*norm(A,1)
    else
        if size(tol,"*")>1|~isreal(tol)|tol<0 then
            error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative scalar expected.\n"),"rowcomp",3));
        end
    end
    select flag
    case "qr" then
        [q,r,rk,e]=qr(A,tol);w=q';
    case "svd" then
        [u,s,v,rk]=svd(A,tol);w=u' ;
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"rowcomp",2,"''qr'',''svd''"));
    end
endfunction


