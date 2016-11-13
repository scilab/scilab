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

function x=%sp_l_s(a,b)
    // a\b , a sparse b full


    [ma,na]=size(a)
    [mb,nb]=size(b)
    if mb*nb==1 then x=a\(b*speye(na,na)),return;end

    if ma<>mb then error(12),end

    if ma<>na then b=a'*b;a=a'*a;end

    if isreal(a)&isreal(b) then
        [h,rk]=lufact(a)
        if rk<min(ma,na) then warning("deficient rank: rank = "+string(rk)),end
        x=[]
        for k=1:nb
            x=[x,lusolve(h,b(:,k))]
        end
        ludel(h)
    else
        [h,rk]=lufact([real(a) -imag(a);imag(a) real(a)])
        if rk<2*min(ma,na) then warning("deficient rank: rank = "+string(rk/2)),end
        x=[]
        for k=1:nb
            x=[x,lusolve(h,[real(b(:,k));imag(b(:,k))])]
        end
        x=x(1:$/2,:)+%i*x($/2+1:$,:)
        ludel(h)
    end
endfunction
