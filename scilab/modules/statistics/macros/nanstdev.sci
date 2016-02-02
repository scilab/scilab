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

function [s]=nanstdev(x,orient)
    //
    //This function    computes the standard  deviation of   the values of a
    //vector or matrix x (ignoring the NANs).
    //
    //For a vector or a matrix x, y=nanstdev(x)  returns in the scalar y the
    //standard deviation of all the entries of x (ignoring the NANs).
    //
    //y=nanstdev(x,'r')  (or,  equivalently, y=nanstdev(x,1)) is the rowwise
    //standard deviation. It returns in each  entry of the  row vector y the
    //standard deviation of each column of x (ignoring the NANs).
    //
    //y=nanstdev(x,'c')  (or,    equivalently,   y=nanstdev(x,2))    is  the
    //columnwise standard deviation. It returns in  each entry of the column
    //vector y the standard deviation of each row of x (ignoring the NANs).
    //
    //
    //Fixings 2001-10-01. Communication from
    //Collewet Guylaine <guylaine.collewet@cemagref.fr>
    //

    if argn(2)==0 then error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"nanstdev",1,2)), end

    if x==[] then s=%nan, return, end

    if argn(2)==1 then  orient="*",end
    if orient=="r" then orient=1,elseif orient=="c" then orient=2,end

    isn=isnan(x)
    Mean=nanmean(x,orient);
    if orient=="*" then
        N=size(x,orient)-sum(bool2s(isn),orient)
        x=x-Mean
    else
        N=size(x,orient)-sum(bool2s(isn),orient)
        ind=list();for k=size(x),ind($+1)=:;end
        ind(orient)=ones(size(x,orient),1)
        x=x-Mean(ind(:))
    end
    x(isn)=0
    allnans=find(N==0)
    N(allnans)=1
    s=sqrt(sum(x.*x,orient)./max(N-1,1));
    s(allnans)=%nan
endfunction
