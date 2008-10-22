//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [pt]=smooth(ptd,pas)
[lhs,rhs]=argn(0)
[m,n]=size(ptd)
d=splin(ptd(1,:),ptd(2,:))
if rhs==1 then l=abs(ptd(1,n)-ptd(1,1));pas=l/100;end
pt=[ptd(1,1)+pas:pas:ptd(1,n)]
pt=[ptd(:,1) [pt;interp(pt,ptd(1,:),ptd(2,:),d)] ptd(:,n)]
endfunction
