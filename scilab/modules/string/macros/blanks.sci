//=============================================
// blanks macro
// blanks(n)
// Create string of blank characters
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================
function str = blanks(n)
  if ~or(type(n)==[1,8]) then
    error(44,1)
  end
  if n>0 then
    str=part(" ",ones(1,n))
  elseif n==0
    str=""
  else
    error(116,1);
  end
endfunction
//=============================================



