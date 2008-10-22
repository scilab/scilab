// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_beta(tree)
// M2SCI function
// Conversion function for Matlab beta()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree

[A,B]=getrhs(tree)
A=convert2double(A)
B=convert2double(B)
tree.rhs=list(A,B)

if is_a_scalar(A) & not_a_scalar(B) then // A is a scalar but not B
  n=gettempvar()
  insert(Equal(list(n),B))
  insert(Equal(list(Operation("ins",list(n,Cste(":")),list())),A))
  tree.rhs(1)=n
  tree.lhs(1).dims=B.dims
elseif is_a_scalar(B) & not_a_scalar(A) then // B is be a scalar but not A
  n=gettempvar()
  insert(Equal(list(n),A))
  insert(Equal(list(Operation("ins",list(n,Cste(":")),list())),B))
  tree.rhs(2)=n
  tree.lhs(1).dims=A.dims
elseif is_a_scalar(A) & is_a_scalar(B) then // Both A and B are scalars
elseif not_a_scalar(A) & not_a_scalar(B) then // A and B have the same size
else
  tree.name="mtlb_beta"
end

tree.lhs(1).type=Type(Double,Real)
endfunction
