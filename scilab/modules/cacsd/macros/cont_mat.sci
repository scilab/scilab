// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=cont_mat(a,b)
//c=cont_mat(a,b) or c=cont_mat(sl) is the controllability matrix.
// of the pair a,b or of the system sl=[a,b,c,d] (syslin list)
//                 2       n
//i.e. c=[b, ab, ab,...; ab ]  
//!
[lhs,rhs]=argn(0)
select type(a)
 case 1  then
    if rhs==1 then error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"cont_mat",2)),end
    [m,n]=size(a)
    if m<>n then error(20,1),end
    [mb,nb]=size(b);if mb<>n then error(60),end

 case 16 then
    if a(1)<>'lss' then error(91,1),end
    [a,b]=a([2,3])
    [n,n]=size(a)  
 else error(msprintf(gettext("%s: Wrong type of input arguments: %s,%s pair or syslin list.\n"),"cont_mat","a","b"))
end;
c=b;for k=1:n-1, c=[b,a*c],end
endfunction
