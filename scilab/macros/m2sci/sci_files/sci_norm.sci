function [tree]=sci_norm(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab norm()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_norm()
// V.C.

// n = norm(A)
if rhs==1 then
  A = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A)
// n = norm(A,p)
else
  [A,p] = getrhs(tree)
  A = convert2double(A)
  tree.rhs=Rhs(A,p)
  
  // Matlab method to compute -%inf norm for matrices
  newtree=Operation("''",list(A),list())
  newtree=Funcall("abs",1,list(newtree),list())
  newtree=Funcall("sum",1,Rhs(newtree,1),list())
  
  // p==-%inf
  if typeof(p)=="operation" then
    if p.operator=="-" & size(p.operands)==1 & typeof(p.operands(1))=="variable" then
      if p.operands(1).name=="%inf" then
	if not_a_vector(A) then
	  tree=Funcall("max",1,Rhs(newtree),tree.lhs)
	elseif ~is_a_vector(A) & ~is_a_scalar(A) then
	  scitree=tree
	  scitree.name="mtlb_norm"
	  repl1=Funcall("max",1,Rhs(newtree),list())
	  repl_poss(scitree,..
	      tree,A,"is a scalar or a vector",..
	      repl1,A,"is a matrix");
	  tree=scitree
	end
	tree.lhs(1).dims=list(1,1)
	tree.lhs(1).type=Type(Double,Real)
	return
      end
    end
  end

  // Arrive here if do not knwon if p==-%inf
  scitree=tree
  scitree.name="mtlb_norm"
  repl1=Funcall("max",1,Rhs(newtree),list())

  repl_poss(scitree,..
      tree,p,"<> -inf");
  
  set_infos("If "+rhs2code(p)+" is equal to -inf then",1);
  repl_poss(scitree,..
      tree,A,"is a scalar or a vector",..
      repl1,A,"is a matrix");
  tree=scitree
end

tree.lhs(1).dims=list(1,1)
tree.lhs(1).type=Type(Double,Real)

endfunction
