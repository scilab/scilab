
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

function [f,p]=ftuneq(varargin)
    //
    //This function computes the F ratio for samples of unequal
    //size.
    //
    //"The most  efficient design is  to make all  samples the
    //same  size n.   However when  this is  nor  feasible, it
    //still  is possible  to modify  the  ANOVA calculations."
    //Note  that  the  definition  of  xbarbar  is  no  longer
    //mean(xbar), but  rather a weighted  average with weights
    //ni.  Additionally  it gives (in  p) the p-value  of the
    //computed Fischer ratio.
    //
    //Given a number  a of samples each of  them composed of n_i
    //(i from 1  to a) observations this fonction  computes in f
    //the Fischer  ratio (it is  the ratio between nr  times the
    //variance  of the  means of  samples  and the  mean of  the
    //variances of each sample).
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs,rhs]=argn(0)
    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"ftuneq",1))
    end
    data=[]
    total=0
    sse=0
    for i=1:rhs
        len(i)=size(varargin(i),"*"),
        mat=matrix(varargin(i),len(i),1),
        partial=sum(mat),
        xbar(i)=partial/len(i),
        total=total+partial,
        data=[data; mat]
        sse=sse+sum((mat-xbar(i)).^2)
    end
    xbarbar=total/sum(len)
    ssa=len'*((xbar-xbarbar).^2)
    msa=ssa/(rhs-1)
    deglib=sum(len-1)
    mse=sse/deglib
    f=msa/mse
    [ignored,p]=cdff("PQ",f,(rhs-1),deglib)
endfunction
