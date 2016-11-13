// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer , F.D
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=%lss_m_s(s1,D2)
    // s=s1*gain
    // SISO case FD
    [A1,B1,C1,D1,x1,dom1]=s1(2:7);
    [n2,m2]=size(D2);
    if prod(size(s1))==1 then
        if n2==1 then
            D=D1*D2; [A1,B1*D2];
            s=tlist(["lss","A","B","C","D","X0","dt"],A1,B1*D2,C1,D,x1,dom1);
            return;
        end
        if m2==1 then
            s=tlist(["lss","A","B","C","D","X0","dt"],A1,B1,D2*C1,D2*D1,x1,dom1);
            return;
        end
        [Q,M]=fullrf(D2);[n2,mq]=size(Q);
        if mq==1 then
            s=Q*tlist(["lss","A","B","C","D","X0","dt"],A1,B1*M,C1,D1*M,x1,dom1);
            return;
        end
        w=s1;
        for k=2:mq, w=sysdiag(w,s1);end
        s=w*M;s=Q*s;
        return;
    end
    D=D1*D2;
    s=tlist(["lss","A","B","C","D","X0","dt"],A1,B1*D2,C1,D,x1,dom1);
endfunction
