function [tree]=sci_any(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab any()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_any()
// V.C.

tree.name="or"

// B = any(A)
if rhs==1 then
  A = getrhs(tree)

  // %c_or and %b_or are not defined
  A = convert2double(A)
  
  if is_complex(A) then
    A = Funcall("abs",1,Rhs(A),list(Variable("",A.infer)))
  elseif ~is_real(A) then
    newA = Funcall("abs",1,Rhs(A),list(Variable("",A.infer)))
    repl_poss(newA,..
	A,A,"is real")
    A=newA
  end
  tree.rhs=Rhs(A)
  
  // if A is not a multidimensional array
  if size(A.dims)==2 then
    if is_a_vector(A) then
      tree.lhs(1).dims=list(1,1)
    elseif not_a_vector(A) then
      tree.rhs=Rhs(A,1)
      tree.lhs(1).dims=list(1,A.dims(2))
    else
      scitree=tree
      scitree.name="mtlb_any"
      repl1=tree
      repl1.rhs=Rhs(A,1)
      repl_poss(scitree,..
	  tree,A,"is a vector",..
	  repl1,A,"is a matrix");
      tree=scitree
      tree.lhs(1).dims=list(Unknown,Unknown)
    end
  // if A is a multidimensional array
  else
    dim = first_non_singleton(A)
    if dim<>Unknown then
      tree.rhs=Rhs(A,dim)
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim)=1
    else
      newrhs=Funcall("firstnonsingleton",1,list(A),list())
      tree.rhs=Rhs(A,newrhs)
      tree.lhs(1).dims=allunknown(A.dims)
    end
  end
  tree.lhs(1).type=Type(Boolean,Real)

// B = any(A,dim)
else
  [A,dim] = getrhs(tree)

  // %c_or and %b_or are not defined
  A = convert2double(A)
  
  if is_complex(A) then
    A = Funcall("abs",1,Rhs(A),list(Variable("",A.infer)))
  elseif ~is_real(A) then
    newA = Funcall("abs",1,Rhs(A),list(Variable("",A.infer)))
    repl_poss(newA,..
	A,A,"is real")
    A=newA
  end
  tree.rhs=Rhs(A,dim)
  
  if typeof(dim)=="cste" then
    if dim.value<=size(A.dims) then
      tree.lhs(1).dims=A.dims
      tree.lhs(1).dims(dim.value)=1
      tree.lhs(1).type=Type(Boolean,Real)
    else
      if not_empty(A) then
	tree=Operation("<>",list(A,Cste(0)),tree.lhs)
	tree.out(1).dims=A.dims
      else
	scitree=tree
	scitree.name="mtlb_any"
	repl1=Operation("<>",list(A,Cste(0)),tree.lhs)
	repl_poss(scitree,..
	    repl1,A,"is not an empty matrix")
	tree=scitree
	tree.lhs(1).dims=A.dims
      end
    end
  else
    scitree=tree
    scitree.name="mtlb_any"
    repl1=Operation("<>",list(A,Cste(0)),tree.lhs)
    repl_poss(scitree,..
	tree,dim,"is <= number of dimensions of "+rhs2code(A),..
	repl1,dim,"is >  number of dimensions of "+rhs2code(A)+" and "+rhs2code(A)+" is not an empty matrix")
    tree=scitree
    tree.lhs(1).dims=allunknwon(A.dims)
  end
end
endfunction
