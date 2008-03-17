// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function []=halt(varargin)
//halt() stops execution until something is entered in the keyboard.
//!

[lhs,rhs]=argn(0);
if (rhs == 0) then 
  msg = 'halt';
else
  if (rhs == 1) then
    msg=string(varargin(1));
  else
    error('incorrect parameter(s). see help halt',999);
  end
end
mprintf(msg);mscanf('%c')
endfunction
