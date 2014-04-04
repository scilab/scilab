// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function [coefs]=regress(x,y)
    //
    //This function computes the regresion coefficients of two
    //variables x and y, both numerical vectors of same number
    //of elements  n. coefs=[a  b] be a  1x2 matrix  such that
    //Y=a+bX will be the equation of the ordinary least square
    //approximation to our data.
    //
    //References:   Wonacott,    T.H.    &   Wonacott,   R.J.;
    //Introductory Statistics, J.Wiley & Sons, 1990.
    //
    //
    
    warnobsolete("reglin", "5.5.1");
    [lhs, rhs] = argn(0);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"regress",2))
    end

    coefs=[]
    if (type(x) <> 1)|(type(y)<>1) then error(msprintf(gettext("%s: Wrong type for input arguments: Numerical expected.\n"),"regress")), end
    lx=length(x)
    if lx<>length(y) then error(msprintf(gettext("%s: Wrong size for both input arguments: same size expected.\n"),"regress")), end
    if lx==0 then error(msprintf(gettext("%s: Wrong size for input argument #%d: Must be > %d.\n"),"regress", 1, 0)), end
    x=matrix(x,lx,1)
    y=matrix(y,lx,1)
    xbar=sum(x)/lx
    ybar=sum(y)/lx
    coefs(2)=sum((x-xbar).*(y-ybar))/sum((x-xbar).^2)
    coefs(1)=ybar-coefs(2)*xbar
endfunction
