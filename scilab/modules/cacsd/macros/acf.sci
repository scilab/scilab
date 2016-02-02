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

function [ac,Mean]=acf(x,n,minim,maxim)
    //function acp(x,n,[minim,maxim])
    // Autocorrelation for one-deimensional process

    [lhs,rhs]=argn(0)
    if rhs <= 1 ; n=prod(size(x))/4;end
    if rhs <= 2 ; minim=-1.0;end
    if rhs <= 3 ; maxim= 1.0;end
    [cov,Mean]=corr(x,n+1);
    ac=cov'/cov(1);
    plot2d3("onn",(0:n)',ac,[1],"011"," ",[0,minim,n,maxim]);
    //stde=sqrt((1+2*ac(2:n+1)'*ac(2:n+1))*1/prod(size(x)))
    stde=2*sqrt(1/prod(size(x)));
    plot2d( [0,0,0;n,n,n],[0,stde,-stde;0,stde,-stde],[1,2,2],"000")
    xtitle(gettext("Autocorrelation Function"));
endfunction
