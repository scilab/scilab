
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1988-2008 - INRIA - François DELEBECQUE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Bfs,Bis,chis]=glever(E,A,s)
    // [Bfs,Bis,chis]=glever(E,A,'s')
    // Computation of (s*E-A)^-1 ('s'=character string with default value 's')
    // Generelized Leverrier's algorithm for a matrix pencil ;
    // (s*E-A)^-1 = (Bfs/chis) - Bis
    // chis = characteristic polynomial (up to a multiplicative constant)
    // Bfs  = polynomial matrix
    // Bis  = polynomial matrix ( - expansion of (s*E-A)^-1 at infinity).
    // Caveat: uses clean to simplify Bfs,Bis and chis !
    // See also shuffle, determ, invr, coffg

    [LHS,RHS]=argn(0);
    if RHS==1 then [E,A]=pen2ea(E),s=poly(0,"s");end
    if RHS==2 then s=poly(0,"s"),end;
    if RHS==3 then s=poly(0,s);end
    [Si,Pi,Di,index]=penlaur(E,A);
    k=round(sum(diag(Si*E)));

    a0=1;
    B=Si;
    SiASi=Si*A*Si;
    chis=a0+0*s;
    Bfs=Si+0*s*Si;

    for i=1:k,
        B=SiASi*E*B;
        alfa=-sum(diag(E*B))/i;
        B=B+alfa*Si;
        chis=s*chis+alfa;
        if i<k then Bfs=s*Bfs+B;end
    end
    Bis=Pi;
    AAD=s*A*Di;
    P=eye(A);

    for nu=1:index+1,
        P=AAD*P;
        Bis=clean(Bis+Pi*P,1.d-10);
    end
    Bfs=clean(Bfs,1.d-10);
    chis=clean(chis,1.d-10);
endfunction
