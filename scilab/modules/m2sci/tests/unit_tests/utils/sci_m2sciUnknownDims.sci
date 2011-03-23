// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright DIGITEO - 2009 - Vincent COUVERT
// Copyright INRIA - 2002-2004 - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_m2sciUnknownDims(tree)
// Variable dimensions set to Unknown
nbdims=size(tree.dims)
newdims=list()
for k=1:nbdims
  newdims(k)=-1
end
tree.lhs(1).dims=newdims
tree.lhs(1).type=tree.rhs(1).type
endfunction


