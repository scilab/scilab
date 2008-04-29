// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [s,k] = sort(V,F)
// sort function
// help sort for more informations

  s =[];
  k =[];
  
  if V==[] then return,end
  
  if (argn(2) == 2) then 
    // check type argument 2 must be a string
    if  F==1|F=='r' then
      F='r'
    elseif  F==2|F=='c' then
      F='c'
    else
      error(44,2);
    end
  else
     F = 'g'; // default flag
  end
  
  select type(V)
  case 1 then // matrix
    if isreal(V) then // real
      [s,k] = gsort(V,F);
    else // complex
      [s,k] = gsort(abs(V),F);
      s = V(k);
    end
  case 10 then  // strings
    
    // Warning Compatiblity with previous version string are sorted by increasing order
    [s,k] = gsort(V,F,'i'); 
  case 5 then // vector sparse
    S = size(V);
    if and(S > 1) then
      error(msprintf(gettext("%s: Wrong size for first input argument: Vector expected.\n"),"sort"),10000);
    else
      // Lhs == 1 only with sparse vectors
      if (argn(1) == 2) then
	error(41);
      end
      
      // sort sparse vector
      [pout1,pout2] = spget(V);
      s = sparse(pout1,gsort(pout2,F));
    end
  else // others cases
    error(246)
  end
  

endfunction

