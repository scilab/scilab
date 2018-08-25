// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ret = who_user(%__bPrint__)
    //get user variables
    [nams,mem] = who("get"); //get all variables
    p = predef("names");
    excluded=["nargin" "nargout" "demolist","%helps","%helps_modules","who_user", "%__bPrint__"];
    p = [p' excluded];

    // remove predef and other excluded vars
    for i = 1:size(p,"*")
        k = nams==p(i)
        nams(k) = []
        mem(k) = []
    end
    ret = nams

    [%_lhs, %_rhs] = argn();

    if %_rhs == 0 then
        %__bPrint__ = %t;
    end

    if %__bPrint__ == %f then
        return;
    end

    n=size(nams,1);

    if n==0 then
        return
    end

    //format names on n*10 characters
    ll=length(nams)+2;m=int((ll-1)/10)+1;
    for k=1:max(m)
        ks=find(m==k);
        if ks<>[] then
            nams(ks)=part(nams(ks),1:(k*10));
        end
    end

    nlc=lines(); nc=nlc(1)//window sizes

    txt=[]

    k=1
    while k<=n
        m=find(cumsum(length(nams(k:$)))<nc);
        m=m($)
        txt=[txt;strcat(nams(k:k-1+m))];
        k=k+m;
    end

    txt=[gettext("User variables are:");
    "";
    txt;
    "";
    msprintf(gettext("Using %s elements"),string(sum(mem)))]
    write(%io(2),txt,"(1x,a)")
endfunction
