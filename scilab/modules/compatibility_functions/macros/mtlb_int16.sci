// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i=mtlb_int16(x)
// Emulation function for Matlab int16()

if x==%inf then
  i=32767
elseif x==-%inf then
  i=-32768
elseif isnan(x) then
  i=0
else
  i=int16(x)
end
endfunction
