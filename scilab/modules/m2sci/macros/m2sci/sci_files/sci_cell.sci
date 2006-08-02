function [tree]=sci_cell(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab cell()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==0 then
  tree.lhs(1).dims=list(0,0)
  tree.lhs(1).type=Type(Cell,Unknown)
  return
end

// cell(n)
if rhs==1 then
  n = getrhs(tree)
  if is_complex(n) then
    n=Funcall("real",1,list(n),list(Variable("",n.infer)))
  elseif ~is_real(n) then
    newn=Funcall("real",1,list(n),list(Variable("",n.infer)))
    repl_poss(newn,n,n,"is Real");
    n=newn
  end
  if is_a_scalar(n) then 
    if typeof(n)=="cste" then
      dim=n.value
    else
      dim=Unknown
    end
    
    tree.lhs(1).dims=list(dim,dim)
    tree.lhs(1).type=Type(Cell,Unknown)
  // cell([n1,n2,...])
  elseif not_a_scalar(n)
    // All dimensions are unknown because we can not compute them here...
    tree.lhs(1).dims=list()
    for k=1:size(tree.rhs)
      tree.lhs(1).dims(k)=Unknown
    end
    tree.lhs(1).type=Type(Cell,Unknown)
  // cell(n) or cell([n1,n2,...]) ?
  else 
    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(1).type=Type(Cell,Unknown)
  end
  
// cell(n1,n2,..)
else
  for k=1:size(tree.rhs)
    if is_complex(tree.rhs(k)) then
      tree.rhs(k)=Funcall("real",1,list(tree.rhs(k)),list()) 
    elseif ~is_real(tree.rhs(k)) then
      newn=Funcall("real",1,list(tree.rhs(k)),list())
      repl_poss(newn,tree.rhs(k),tree.rhs(k),"is Real");
      tree.rhs(k)=newn
    end
  end

  // If one input is a Cste, we can infer corresponding dimension...
  tree.lhs(1).dims=list()
  for k=1:size(tree.rhs)
    if typeof(tree.rhs(k))=="cste" then
      tree.lhs(1).dims($+1)=tree.rhs(k).value
    else
      tree.lhs(1).dims($+1)=Unknown
    end
  end
  tree.lhs(1).type=Type(Cell,Unknown)
end

endfunction
