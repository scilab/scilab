
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//


function [dif]=nand2mean(sample1,sample2,conf)
    //
    //This function    computes   an estimate (dif(1))  for  the
    //difference of the means of two independent samples (arrays
    //sample1 and  sample2) and gives  the half amplitude of the
    //range  of variability of  dif with an indicated confidence
    //level (dif(2)). The choice of the normal or t fonctions as
    //the  probability fonction depends  on the sizes of sample1
    //and sample2.  We  suppose that the underlying variances of
    //both populations are equal. NAN values are not counted.
    //
    //In absence of the confidence  parameter a confidence level
    //of 95% is assumed.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs,rhs]=argn(0)
    if rhs<2|rhs>3  then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nand2mean",2,3)),
    elseif rhs==2 then
        conf=.975
    end
    if (sample1==[]|sample2==[]) then
        dif=%nan
        return,
    end
    isn1=isnan(sample1)
    isn2=isnan(sample2)
    sample1(isn1)=0
    sample2(isn2)=0
    n1=sum(bool2s(~isn1))
    n2=sum(bool2s(~isn2))
    mean1=sum(sample1)/n1
    mean2=sum(sample2)/n2
    svarp=sqrt((sum((sample1(~isn1)-mean1).^2)+sum((sample2(~isn2) ..
    -mean2).^2))/(n1+n2-2))
    dif(1)=mean1-mean2
    if (n1+n2-2)>100 then
        dif(2)=cdfnor("X",0,1,conf,1-conf)*svarp*sqrt((1/n1)+(1/n2))
    else
        dif(2)=cdft("T",n1+n2-2,conf,(1-conf))*svarp*sqrt((1/n1)+(1/n2))
    end
endfunction
