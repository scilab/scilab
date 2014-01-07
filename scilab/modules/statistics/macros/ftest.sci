
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function [f,p]=ftest(samples)
    //
    //Given a  number nc of samples each  of them composed of nr
    //observations this fonction computes in f the Fischer ratio
    //(it is the  ratio between  nr times  the variance   of the
    //means of  samples  and the  mean of the  variances of each
    //sample).  Additionally it gives (in p) the p-value of the
    //computed  Fischer ratio.  samples   is a matrix  with type
    //nrxnc.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    if argn(2) == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"ftest",1))
    end
    [nr nc]=size(samples)
    xbar=mean(samples,"r")
    xbarbar=mean(xbar)
    sx2=sum((xbar-xbarbar).^2)/(nc-1)
    sp2=(sum(sum((samples-ones(nr,1)*xbar).^2),"r"))/(nc*(nr-1))
    f=(nr*sx2)/sp2
    [ignored,p]=cdff("PQ",f,(nc-1),(nc*(nr-1)))
endfunction
