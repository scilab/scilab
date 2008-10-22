function [tree]=sci_log2(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab log2()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// %c_log2/%c_frexp and %b_log2/%b_frexp are not defined in Scilab
X = getrhs(tree)
X = convert2double(X)
tree.rhs=Rhs(X)

// Y = log2(X)
if lhs==1 then
  tree.lhs(1).infer=X.infer
// [F,E] = log2(X)
else
  if is_complex(X) then
    X = Funcall("real",1,list(X),list(Variable("",X.infer)))
  elseif ~is_real(X) then
    newX = Funcall("real",1,list(X),list(Variable("",X.infer)))
    repl_poss(newX,X,X,"is real");
    X=newX
  end
  tree.rhs=Rhs(X)
  tree.name="frexp"
  tree.lhs(1).dims=X.dims
  tree.lhs(1).type=Type(Double,Real)
end

endfunction
