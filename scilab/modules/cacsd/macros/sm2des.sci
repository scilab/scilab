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

function des=sm2des(sysmat,n)
    // Generalized system-matrix to descriptor
    // sysmat=[-s E + A , B; C , D]
    // des=list('des',A,B,C,D,E)
    // n=size(A) (assumed square) (optional parameter)

    [LHS,RHS]=argn(0)
    [nl,nc]=size(sysmat);
    E=-coeff(sysmat,1);
    if RHS==1 then
        k=nc;n=k;
        while E(:,k)==0*E(:,k)
            k=k-1;n=k;
        end
        l=nl;n1=l;
        while E(l,:)==0*E(l,:)
            l=l-1;n1=l;
        end
        if n1<>n then
            warning(msprintf(gettext("%s: Cannot find the state dimension.\n"),"sm2des"));
        end
    end
    E=E(1:n,1:n);
    A=coeff(sysmat(1:n,1:n),0);
    B=coeff(sysmat(1:n,n+1:nc),0);
    C=coeff(sysmat(n+1:nl,1:n),0);
    D=coeff(sysmat(n+1:nl,n+1:nc),0);
    des=tlist(["des","A","B","C","D","E"],A,B,C,D,E);
endfunction
