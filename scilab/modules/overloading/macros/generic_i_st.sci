// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -  F. Delebecque, S. Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function out=generic_i_st(i,in,out)

// out.i=in  
// this function is called only if the i field is not already defined in out
//
  if prod(out.dims)>1 then error(1),end 
  T=getfield(1,out);
  T($+1)=i;setfield(1,T,out)
  setfield($+1,in,out)
  if double(out.dims)==[0 0] then out.dims=int32([1 1]),end
endfunction
