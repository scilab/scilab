function [tree]=sci_sum(tree)
// File generated from sci_PROTO8.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab sum()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_sum()
// V.C.

// B = sum(A)
if rhs==1 then
  A = getrhs(tree)

  // Because %b_sum and %C_sum are not defined
  A = convert2double(A)
  tree.rhs=Rhs(A)
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end

  if is_a_vector(A) | is_empty(A) then
    tree.lhs(1).dims=list(1,1)
  elseif not_a_vector(A) then
    tree.lhs(1).dims=list(1,A.dims(2))
    tree.rhs=Rhs(A,1)
  elseif not_empty(A) then
    dim = first_non_singleton(A)
    if dim>0 then
      tree.rhs=Rhs(A,dim)
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim)=1
    else
      tree.rhs=Rhs(A,Funcall("firstnonsingleton",1,list(A),list()))
      tree.lhs(1).dims=allunknown(A.dims)
    end
  else
    scitree=tree
    scitree.name="mtlb_sum"
    repl1=tree
    repl1.rhs=Rhs(A,1)
    repl2=tree
    repl2.rhs=Rhs(A,Funcall("firstnonsingleton",1,list(A),list()))
    repl_poss(scitree,..
	tree,A,"is an empty matrix or a vector",..
	repl1,A,"is a matrix",..
	repl2,A,"is a multidimensional array");
    tree=scitree
    tree.lhs(1).dims=allunknown(A.dims)
  end

// B = sum(A,dim)
else
  [A,dim] = getrhs(tree)
  
  // Because %b_sum and %C_sum are not defined
  A=convert2double(A)
  dim=convert2double(dim)
  tree.rhs=Rhs(A,dim)
  
  
  if typeof(dim)=="cste" then
    if dim.value>size(A.dims) then
      tree.name="mtlb_sum"
      set_infos("Scilab sum() does not work when dim input argument is greater than number of dims of first rhs...",1)
      tree.lhs(1).dims=allunknown(A.dims)
    else
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim.value)=1
    end
  else
    tree.name="mtlb_sum"
    repl1=tree;repl1.name="sum";repl1.rhs=Rhs(A,dim)
    repl_poss(tree,..
	repl1,dim,"is < number of dims of first rhs")
    tree.lhs(1).dims=allunknown(A.dims)
  end
  
  if is_real(A) then
    tree.lhs(1).type=Type(Double,Real)
  else
    tree.lhs(1).type=Type(Double,Unknown)
  end
end
endfunction
