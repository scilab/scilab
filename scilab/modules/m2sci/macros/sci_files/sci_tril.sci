// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [tree]=sci_tril(tree)
// File generated from sci_PROTO7.g: PLEASE DO NOT EDIT !
// M2SCI function
// Conversion function for Matlab tril()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_tril()

// L = tril(X,k)
if rhs==2 then
  [X,k] = getrhs(tree)
  // Convert k to double
  k=convert2double(k)
  // Imaginary part ok k is ignored in Matlab
  if is_complex(k) then
    k = Funcall("real",1,list(k),list(Variable("",k.infer)))
  elseif ~is_real(k) then
    newk = Funcall("real",1,list(k),list(Variable("",k.infer)))
    repl_poss(newk,k,k,gettext("is Real."));
    k=newk
  end
  tree.rhs=Rhs_tlist(X,k)
// L = tril(X)
else
  X = getrhs(tree)
end

// Common part
if or(X.vtype==[String,Unknown]) then
  tree.name="mtlb_tril"
else
  if X.vtype==Boolean then
    X=Funcall("bool2s",1,Rhs_tlist(X),list(Variable("",X.infer)))
    tree.rhs(1)=X
  end
end

if isempty(strindex(tree.name,"triu")) then
  tree.lhs(1).type=X.type
else
  tree.lhs(1).type=Type(X.vtype,Unknown)
end
tree.lhs(1).dims=X.dims
  
endfunction
