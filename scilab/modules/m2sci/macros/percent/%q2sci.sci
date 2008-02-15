// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=%q2sci(tree)
// M2SCI function
// Conversion function for Matlab elementwise left division
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree

[A,B] = getoperands(tree)

// Scilab left division does not work with Strings, Booleans...
//A = convert2double(A)
//B = convert2double(B)

tree.operands=list(A,B)

if is_real(A) & is_real(B) then
  tree.out(1).type=Type(Double,Real)
elseif (is_real(A) & is_complex(B)) | (is_real(B) & is_complex(A)) then
  tree.out(1).type=Type(Double,Complex)
else
  tree.out(1).type=Type(Double,Unknown)
end
  
if is_a_scalar(A) then
  tree.out(1).dims=B.dims
else
  tree.out(1).dims=A.dims
end
  
endfunction

