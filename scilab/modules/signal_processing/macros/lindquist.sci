// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [P,R,T]=lindquist(n,H,F,G,R0)
    //[Pn,Rn,Tn]=lindquist(n,H,F,G,R0)
    //macro which computes iteratively the minimal solution of the algebraic
    //Riccati equation and gives the matrices Rn and Tt of the filter model,
    //by the lindquist algorithm.
    //   n     : number of iterations.
    //   H,F,G : estimated triple from the covariance sequence of y.
    //   R0    : E(yk*yk')
    //   Pn    : solution of the Riccati equation after n iterations.
    //   Rn,Tn : gain matrices of the filter.
    //!
    [d,m]=size(H);
    //initialization
    Gn=G;
    Rn=R0;
    Pn=zeros(m,m)
    Kn=0*ones(m,d);

    //recursion
    for j=1:n,
        //  Pn=Pn+Gn/Rn*Gn'
        //  Kn=Pn*H'
        Kn=Kn+Gn/Rn*Gn'*H';
        r1=R0-H*Kn;
        Rn=Rn-Gn'*H'/r1*H*Gn;
        Gn=(F-(G-F*Kn)/r1*H)*Gn;
    end

    //gain matrices of the filter.
    //P=Pn
    //R=R0-H*P*H'
    //T=(G-F*P*H')/R
    R=R0-H*Kn
    T=(G-F*Kn)/R
    P=Kn
endfunction
