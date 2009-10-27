// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [xx,yy,zz]=nf3d(x,y,z)
// 3d coding transformation 
// from facets coded in three matrices x,y,z to scilab code for facets 
// accepted by plot3d 
//---------------------------------------------------------
if argn(2)<>3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "nf3d", 3));
end


[n1,n2]=size(x)
ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
// ind=[1,2,n1+2,n1+1 , 2,3,n1+3,n1+2, ....  ,n1-1,n1,2n1,2n1-1
ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);
nx=prod(size(ind2))
xx=matrix(x(ind2),4,nx/4);
yy=matrix(y(ind2),4,nx/4);
zz=matrix(z(ind2),4,nx/4);
endfunction
