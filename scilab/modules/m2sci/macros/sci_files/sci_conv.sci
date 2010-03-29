// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_conv(tree)
// M2SCI function
// Conversion function for Matlab conv()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_conv()

[A,B]=getrhs(tree)

if and([A.dims(1),B.dims(1)]==1) | and([A.dims(2),B.dims(2)]>1) then // A and B row vectors
  conv=tree
  conv.name="convol"
  tree.name="clean"
  tree.rhs=list(conv)
  tree.lhs(1).type=Type(Double,Unknown)
elseif A.dims(1)>1 | A.dims(2)==1 | B.dims(1)>1 | B.dims(2)==1 then // A and/or B is a column vector
  conv=tree
  conv.name="convol"
  tree.name="clean"
  tree.rhs=list(conv)
  tree=Operation(".''",list(tree),tree.lhs)
  tree.out(1).type=Type(Double,Unknown)
else
  tree.name="mtlb_conv"
  tree.lhs(1).type=Type(Double,Unknown)
end

endfunction
