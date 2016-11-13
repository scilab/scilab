
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


function [m]=nanmean(x,orient)
    //
    //This function returns in scalar m the mean of the values (ignoring the
    //NANs) of  a  vector  or  matrix  x.
    //
    //For  a vector   or  matrix x,  m=nanmean(x) or  m=nanmean(x,'*')
    //returns in scalar m the mean of all the entries (ignoring the NANs) of
    //x.
    //
    //m=nanmean(x,'r')  (or,   equivalently, m=nanmean(x,1))  returns in
    //each entry of  the row vector  m  of type 1xsize(x,'c')  the mean of
    //each column of x (ignoring the NANs).
    //
    //m=nanmeanf(x,'c')(or, equivalently, m=nanmean(x,2))  returns    in
    //each entry of the column vector m of  type size(x,'c')x1 the mean of
    //each row of x (ignoring the NANs).
    //
    //
    if argn(2)==0 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanmean",1,2)), end
    if argn(2)==1 then  orient="*",end
    //replace nans by 0
    isn=isnan(x)
    x(isn)=0

    N=size(x,orient)-sum(bool2s(isn),orient)
    allnans=find(N==0)
    N(allnans)=1
    m = sum(x,orient)./N;
    m(allnans)=%nan

endfunction
