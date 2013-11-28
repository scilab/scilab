// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier: made obsolete, merged in stdev
// Copyright (C) XXXX - INRIA
// Copyright (C) XXXX - ENPC
// Copyright (C) XXXX - Ecole des Mines de Nantes - Philippe Castagliola
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function sd=st_deviation(x,cr)
    //
    warnobsolete("stdev", "5.5.1");
    if argn(2)<2 then cr="*",end
    if x == [] then sd=%nan;return ;end
    if typeof(x)=="hypermat" then sd=%hm_stdev(x,cr),return,end
    [m,n]=size(x);
    if cr=="*" then
        n=m*n
        select n
        case 0 then sd=%nan
        case 1 then sd=0
        else
            sd=sqrt(sum((x-mean(x)).^2)/(n-1));
        end
    elseif cr=="c"|cr==2
        if n==1 then
            sd=zeros(m,1)
        else
            sd=sqrt(sum((x-mean(x,"c")*ones(x(1,:))).^2,"c")/(n-1));
        end
    elseif cr=="r"|cr==1
        if m==1 then
            sd=zeros(1,n)
        else
            sd=sqrt(sum((x-ones(x(:,1))*mean(x,"r")).^2,"r")/(m-1));
        end
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"),"st_deviation",2,"*","r","c",1,2));
    end
endfunction
