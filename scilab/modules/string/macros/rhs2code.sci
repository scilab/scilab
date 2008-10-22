// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function C=rhs2code(rhs)
// Translate rhs list of an instruction to macro code (called by tree2code)
// Input:
// - rhs: list of rhs arguments
// Output:
// - C: Scilab code corresponding to rhs
// V.C.

C=[]
n=size(rhs)

args=[]
if typeof(rhs)<>"list" then
  C=expression2code(rhs)
else
  for k=1:n
    args=[args expression2code(rhs(k))]
  end
  C=strcat(args,",")
end


endfunction
