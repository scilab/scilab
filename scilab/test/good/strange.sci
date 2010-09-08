// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [r]=strange(x,orien)
//
//The range  is  the distance between   the largest  and smaller  value,
//[r]=range(x) computes the range of vector or matrix x.
//
//[r]=range(x,'r')  (or equivalently  [r]=range(x,1)) give a  row vector
//with the range of each column.
//
//[r]=range(x,'c') (or equivalently [r]=range(x,2)) give a column vector
//with the range of each row.
//
//
  [lhs,rhs]=argn(0)
  if rhs==0 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"strange",0,2)), end
  if rhs==1 then
    r=max(x)-min(x)
  elseif rhs==2 then
    if orien=='r'|orien==1 then 
      r=max(x,'r')-min(x,'r')
    elseif orien=='c'|orien==2 then
      r=max(x,'c')-min(x,'c')
    else 
      error(msprintf(gettext("%s: Wrong value for %d input argument: ''%s'', ''%s'', %d or %d expected.\n"),"strange",2,"r","c",1,2)),
    end
  end
endfunction
