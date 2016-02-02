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

function [f1,f2,f3,f4,f5,f6,f7]=%lss_e(i,j,f)
    // f= f(i,j)

    if type(i)==10 then
        [lhs,rhs]=argn(0)
        if rhs<>2 then  error(21),end
        nams=["A","B","C","D","X0","dt"]
        for k=1:prod(size(i))
            kf=find(convstr(i(k),"u")==nams)
            if kf==[] then error(21),end
            execstr("f"+string(k)+"=j(kf+1)")
        end
        return
    end
    if type(i)==4 then i=find(i),end
    if type(j)==4 then j=find(j),end
    [a,b,c,d,x0,dom]=f(2:7)
    f1=tlist(["lss","A","B","C","D","X0","dt"],a,b(:,j),c(i,:),d(i,j),x0,dom)
    if f1("D")==[] then f1=[],end
endfunction
