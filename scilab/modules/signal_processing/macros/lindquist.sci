// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
