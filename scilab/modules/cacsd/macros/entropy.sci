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


function [enx,X,Y,eny]=entropy(S,gama)
    //Computation of the entropy at gama of a linear system S
    //[enx,X,Y,eny]=entropy(S,gama)
    //!
    //

    S1=S(1);
    if S1(1)=="r" then S=tf2ss(S);end
    [A,B,C,D]=S(2:5);
    g1=1/gama;
    H=[A,g1*B*B';-g1*C'*C,-A'];
    [X,zero]=ric_desc(H);             //zero=A'*X+X*A+g1*g1*X*B*B'*X+C'*C;
    enx=sum(diag(X*B*B'));
    J=[A',g1*C'*C;-g1*B*B',-A];
    [Y,zero]=ric_desc(J);            //zero=A*Y+Y*A'+g1*g1*Y*C'*C*Y+B*B'
    eny=sum(diag(Y*C'*C));
endfunction
