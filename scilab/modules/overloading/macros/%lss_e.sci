// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
