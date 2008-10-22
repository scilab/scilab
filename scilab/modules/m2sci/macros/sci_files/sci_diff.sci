// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_diff(tree)
// M2SCI function
// Conversion function for Matlab diff()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_diff()

// Y = diff(X)
if rhs==1 then
  X=getrhs(tree)
  // %c_diff and %b_diff are not defined in Scilab
  X=convert2double(X)
  tree.rhs=Rhs_tlist(X)
  
  dim = first_non_singleton(X)
  
  if dim==-1 then
    // All output dims are unknown
    tree.lhs(1).dims=allunknown(X.dims)
    // First non singleton dimension will be computed at execution
    tree.rhs=Rhs_tlist(X,1,Funcall("firstnonsingleton",1,list(X),list()))
  else
    tree.lhs(1).dims=X.dims
    if dim==0 then
      tree.rhs=Rhs_tlist(X)
    else
      tree.rhs=Rhs_tlist(X,1,dim)
      tree.lhs(1).dims(dim)=max(X.dims(dim)-1,-1)
    end
  end
  tree.lhs(1).type=Type(Double,Unknown)
// Y = diff(X,n)
elseif rhs==2 then
  [X,n] = getrhs(tree);
  X = convert2double(X)
  n = convert2double(n)
  tree.rhs=Rhs_tlist(X,n)
  
  if ~is_a_vector(X) then
    tree.name="mtlb_diff"
  end
  tree.lhs(1).dims=allunknown(X.dims)
  tree.lhs(1).type=Type(Double,Unknown)
    
// Y = diff(X,n,dim)
else
  [X,n,dim] = getrhs(tree);
  X = convert2double(X)
  n = convert2double(n)
  dim = convert2double(dim)
  tree.rhs=Rhs_tlist(X,n,dim)
  
  tree.lhs(1).dims=allunknown(X.dims)
  tree.lhs(1).type=Type(Double,Unknown)
end

endfunction
