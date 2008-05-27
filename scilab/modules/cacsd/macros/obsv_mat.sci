// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function o=obsv_mat(a,c)

[lhs,rhs]=argn(0)
select type(a)
 case 1  then
    if rhs==1 then error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"obsv_mat",2)),end
    [m,n]=size(a)
    if m<>n then error(20,1),end
    [mb,nb]=size(c);if nb<>n then error(60),end
  case 16 then
    flag=a(1)
    if flag(1)<>'lss' then error(91,1),end
    [a,c]=a([2,4])
    [n,n]=size(a)
else error(msprintf(gettext("%s: Wrong type of input arguments: %s,%s pair or syslin list.\n"),"obsv_mat","a","c"))

end;
o=c;for k=1:n-1, o=[c;o*a],end
endfunction
