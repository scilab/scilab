// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=mtlb_l(a,b)
// Emulation function for Matlab left division

if type(a)==10 then
  a=asciimat(a)
end
if type(b)==10 then
  b=asciimat(b)
end

if or(type(a)==[4,6]) then
  a=bool2s(a)
end
if or(type(b)==[4,6]) then
  b=bool2s(b)
end

y=a\b

endfunction
