// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//2012/08/06 add return value with variable list.

function ret = who_user(%__bPrint__)
    //get user variables
    [nams,mem]=who('get'); //get all variables
    p=predef(); //number of system variable
    st=stacksize()
    nams=nams(1:$-p+1);mem=mem(1:$-p+1);
    //modifiable system variables
    excluded=['demolist','%helps','%helps_modules','home','who_user', "%__bPrint__"];
    ke=grep(nams,excluded)
    nams(ke)=[];mem(ke)=[];
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
        '';
        txt;
        '';
        msprintf(gettext("Using %s elements out of %s"),string(sum(mem)), string(st(1)-(st(2)-sum(mem))))]
    write(%io(2),txt,'(1x,a)')
endfunction
