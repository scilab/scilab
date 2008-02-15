// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [x,lagr,f]=linpro(p,C,b,ci,cs,mi,x0,imp)
[lhs,rhs]=argn(0)
n=maxi(size(p));Q=0*ones(n,n);
select rhs
 case 8 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0,imp)
 case 7 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0)
 case 6
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi)
 case 5 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs)
 case 4 then
[x,lagr,f]=quapro(Q,p,C,b,ci)
 case 3 then
[x,lagr,f]=quapro(Q,p,C,b)
 else
error('Bad call to linpro')
end
endfunction
