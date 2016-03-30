
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

function [X,dim]=range(A,k)
    // Computation of Range A^k ; the first dim rows of X span the
    // range of A^k.
    //!
    if argn(2)==1 then k=1,end
    k=double(k)
    if int(k)<>k|k<0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-negative integer expected.\n"),"range",2));
    end
    if size(A,1)<>size(A,2)|~isreal(A) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"range",1));
    end

    if k==0 then
        dim=size(A,1);X=eye(A);
    else
        [U,dim]=rowcomp(A);X=U;
        for l=2:k
            A=A*U';
            [U,dim]=rowcomp(A(:,1:dim));
            X=U*X;
        end;
    end
endfunction
