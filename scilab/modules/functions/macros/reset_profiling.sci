// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function reset_profiling(funname)
//reset profiling data in bytecode
  nsiz=6
  execstr('code=bytecode('+funname+')')
  lc=1
  lc = lc + nsiz*double(code(lc)) + 1
  lc = lc + nsiz*double(code(lc)) + 1
  long=code(lc)
  lc = lc+1
  c=code(lc:$)
  c1=bytecodewalk(c,25,resetprof)
  code=[code(1:lc-2) int32(size(c1,'*')) c1]
  execstr(funname+' = resume(bytecode(code))')
endfunction
function [c,l]=resetprof(l)
//reset profiling data in bytecode
  c=int32([25 0 0]);l=l+3;
endfunction
