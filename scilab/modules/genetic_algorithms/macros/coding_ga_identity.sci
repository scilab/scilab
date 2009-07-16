// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [pop_out] = coding_ga_identity(pop_in,direction,param)
// pop_in is a list().
// pop_out must be a list().
if (direction=='code') then
  pop_out = pop_in;
elseif (direction=='decode') then
  pop_out = pop_in;
else
  error(sprintf(gettext("%s: wrong direction"),"coding_ga_identity"));
end
endfunction
