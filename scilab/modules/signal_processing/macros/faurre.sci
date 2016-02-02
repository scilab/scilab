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

function [P,R,T]=faurre(n,H,F,G,R0)
    //[P,R,T]=faurre(n,H,F,G,R0)
    //macro which computes iteratively the minimal solution of the algebraic
    //Riccati equation and gives the matrices Rt and Tt of the filter model.
    //   n     : number of iterations.
    //   H,F,G : estimated triple from the covariance sequence of y.
    //   R0    : E(yk*yk')
    //   P    : solution of the Riccati equation after n iterations.
    //   R,T  : gain matrix of the filter.
    //!

    //initialization
    Pn=G*inv(R0)*G'
    //recursion
    for k=1:n,
        A=(G-F*Pn*H');
        Pn=F*Pn*F'+A/(R0-H*Pn*H')*A',
    end;
    P=Pn
    //gain matrices of the filter.
    R=R0-H*P*H';
    T=(G-F*P*H')/R;
endfunction
