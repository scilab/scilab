function [tree]=sci_meshgrid(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab meshgrid()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

if rhs==1
  A=getrhs(tree);
  A=convert2double(A);
  tree.rhs=Rhs(A);
elseif rhs==2
  [A,B]=getrhs(tree);
  A=convert2double(A);
  B=convert2double(B);
  tree.rhs=Rhs(A,B);
end

if lhs==1
  if tree.rhs(1).dims(1)<>Unknown
    tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
  else
    tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
  end
elseif lhs==2
  if tree.rhs(1).dims(1)<>Unknown
    tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
  else
    tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
  end
  if rhs==1
    tree.lhs(2).dims=tree.lhs(1).dims;
  else
    if tree.rhs(2).dims(1)<>Unknown
      tree.lhs(2).dims=list(tree.rhs(2).dims(1),tree.rhs(2).dims(1));
    else
      tree.lhs(2).dims=list(tree.rhs(2).dims(2),tree.rhs(2).dims(2));
    end
  end
else
  if tree.rhs(1).dims(1)<>Unknown
    tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
  else
    tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
  end
  
  if tree.rhs(2).dims(1)<>Unknown
    tree.lhs(2).dims=list(tree.rhs(2).dims(1),tree.rhs(2).dims(1));
  else
    tree.lhs(2).dims=list(tree.rhs(2).dims(2),tree.rhs(2).dims(2));
  end
  
  if tree.rhs(3).dims(1)<>Unknown
    tree.lhs(3).dims=list(tree.rhs(3).dims(1),tree.rhs(3).dims(1));
  else
    tree.lhs(3).dims=list(tree.rhs(3).dims(2),tree.rhs(3).dims(2));
  end
end
endfunction
