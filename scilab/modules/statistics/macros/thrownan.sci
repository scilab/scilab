
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [nonan,numb]=thrownan(x)
//
//
//This  function  returns  in  vector  nonan  the  values
//(ignoring the NANs) of a  vector or matrix x and in the
//corresponding places of vector  numb the indexes of the
//value.
//
//For  a  vector  or matrix  x,  [nonan,numb]=thrownan(x)
//considers x, whatever his dimensions are, like a linear
//vector (columns  first).
//
//
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"thrownan",1)), end
  if x==[] then s=[], return,end
  numb=find(bool2s(~isnan(x)))
  nonan=x(~isnan(x))

endfunction
