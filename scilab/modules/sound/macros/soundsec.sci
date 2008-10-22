// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// Copyright (C) ???? - ENPC
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t]=soundsec (n,rate)
// Return n seconds of t parameter at rate rate (sample/sec)
  [lhs,rhs]=argn(0);
  if rhs <=1 ; rate=22050; end;
  t= linspace(0,n,n*rate);
endfunction
