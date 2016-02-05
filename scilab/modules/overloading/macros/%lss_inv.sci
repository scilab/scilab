// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x=%lss_inv(a)

    d=a(5);
    [m,n]=size(d);
    polyn=(type(d)==2);constant=(type(d)==1);
    if constant&(m==n) then
        minsv=min(svd(d));rcd=rcond(d);s=poly(0,"s");
    end
    if constant&(m<>n) then
        minsv=min(svd(d));s=poly(0,"s");
    end

    if polyn then rcd=0;minsv=0;s=poly(0,varn(d));end
    if m==n then
        if rcd > 1.d-6 then
            x=invsyslin(a)
        else
            h=systmat(a);
            se=rand("seed");
            valfa=rand(1,10,"normal")/100;
            rand("seed",se);
            www=[];
            for k=1:10
            www=[www,rcond(horner(h,valfa(k)))];end
            [w,k1]=max(www);alfa=valfa(k1);
            x=invrs(a,alfa);
        end
    elseif m<n then
        warning("non square system! --> right inverse")
        if minsv > 1.d-6 then
            x=invsyslin(a)
        else
            [stmp,ws]=rowregul(a,0,0);
            if min(svd(stmp(5))) > 1.d-6 then
                x=invsyslin(stmp)*ws
            else
                error(19)
            end
        end
    elseif m>n then
        warning("non square system! --> left inverse")
        if minsv > 1.d-6 then
            x=invsyslin(a)
        else
            [stmp,ws]=rowregul(a,0,0);
            if min(svd(stmp(5))) > 1.d-6 then
                x=invsyslin(stmp)*ws
            else
                error(19)
            end
        end
    end
endfunction
