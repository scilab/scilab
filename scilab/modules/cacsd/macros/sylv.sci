// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function X = sylv(A,B,C,flag)
//  solve  A*X+X*B=C if flag=='c' or  A*X*B-X=C if flag=='d'
  if argn(2)<>4 then error(39),end
  if size(A,1)<> size(A,2) then error(20,1),end
  if size(B,1)<> size(B,2) then error(20,2),end
  if size(C,1)<> size(A,1) then 
    error(msprintf(gettext("%s: First and third input arguments have incompatible number of rows.\n"),"sylv"))
  end
  if size(C,2)<> size(B,2) then 
	error(msprintf(gettext("%s: Second and third input arguments have incompatible number of columns.\n"),"sylv"))
  end
    
  if flag=='c' then 
    flag=[0 0 0], 
  elseif flag=='d' then
    flag=[1 0 0],
  else
    error(36,3)
  end
  X=linmeq(1,A,B,C,flag)
endfunction
