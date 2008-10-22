function [tree]=sci_find(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab find()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_find()
// V.C.

x = getrhs(tree)

// Because %c_find is not defined
x = convert2double(x)
tree.rhs=Rhs(x)

// k = find(x)
if lhs==1 then
  if is_complex(x) then
    x = Funcall("abs",1,list(x),list(Variable("",x.infer)))
  elseif ~is_real(x) then
    newx = Funcall("abs",1,list(x),list(Variable("",x.infer)))
    repl_poss(newx,x,x,"is real");
    x=newx
  end
  tree.rhs=Rhs(x)
    
  tree1=Operation("''",list(tree),tree.lhs)
  tree2=Funcall("matrix",1,Rhs(tree,-1,1),tree.lhs)
  
  tree.lhs(1).type=Type(Double,Real)

  if x.dims(1)==1 then // Row vector
    tree.lhs(1).dims=list(1,Unknown)
  elseif x.dims(2)==1 then // Column vector
    tree=tree1
    tree.out(1).dims=list(Unknown,1)
    tree.out(1).type=Type(Double,Real)
  elseif not_a_vector(x) then
    tree=tree2
    tree.lhs(1).dims=list(Unknown,Unknown)
  else
    tree.name="mtlb_find"
    tree.lhs(1).dims=list(Unknown,Unknown)
  end
// [i,j] = find(x)
elseif lhs==2 then
  if is_complex(x) then
    x = Funcall("abs",1,list(x),list(Variable("",x.infer)))
  elseif ~is_real(x) then
    newx = Funcall("abs",1,list(x),list(Variable("",x.infer)))
    repl_poss(newx,x,x,"is real");
    x=newx
  end
  tree.rhs=Rhs(x)

  tree.lhs(1).type=Type(Double,Real)
  tree.lhs(2).type=Type(Double,Real)
  if x.dims(1)==1 then // Row Vector
    tree.lhs(1).dims=list(1,Unknown)
    tree.lhs(2).dims=list(1,Unknown)
  else  
    tree.name="mtlb_find"
    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(2).dims=list(Unknown,Unknown)
  end
// [i,j,v] = find(x)
else
  tree.name="mtlb_find"
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(2).dims=list(Unknown,Unknown)
  tree.lhs(3).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Double,Real)
  tree.lhs(2).type=Type(Double,Real)
  tree.lhs(3).type=Type(Unknown,Unknown)
end

endfunction
