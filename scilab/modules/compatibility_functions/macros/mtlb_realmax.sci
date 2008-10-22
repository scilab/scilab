// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function val=mtlb_realmax(prec)
// Emulation function for Matlab realmax()

rhs=argn(2);

if rhs<=0 then
  val=number_properties("huge");
elseif rhs==1 then
  if prec=="double" then
    val=number_properties("huge");
  else
    error(msprintf(gettext("%s: Not yet implemented.\n"),"mtlb_realmax"));
  end
else
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"mtlb_realmax",0,1));
end
endfunction
