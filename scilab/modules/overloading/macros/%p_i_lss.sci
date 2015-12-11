// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s2=%p_i_lss(i,j,s1,s2)
    //s2=%p_i_lss(i,j,s1,s2) <=> s2(i,j)=s1
    //!

    if type(i)==10 then  // sl('D')
        [lhs,rhs]=argn(0)
        if rhs<>3 then  error(21),end
        if i<>"D" then
            error(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%p_i_lss",1));
        end
        s2=s1;kf=5
        if size(s2(kf))<>size(j) then
            warning(msprintf(_("%s: Wrong type for input argument #%d.\n"),"%p_i_lss",1));
        end
        s2(kf)=j
        return
    end

    s1=tlist(["lss","A","B","C","D","X0","dt"],[],[],[],s1,[],[])
    s2(i,j)=s1
endfunction
