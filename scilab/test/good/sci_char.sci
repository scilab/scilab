function [tree]=sci_char(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab char()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// F.B

if rhs==1 then // char(A)
  A = getrhs(tree)
  if A.vtype==1 | A.vtype==8 | A.vtype==10 then
tree.lhs(1).dims=tree.rhs(1).dims
// input is cell
elseif A.vtype==16 | A.vtype==17 then
if size(A.contents.data)>0 then
  lhs_dims1=0;
  lhs_dims2=0;
for i=1:size(A.contents.data)
  if A.contents.data(i).dims(1)<>-1 & A.contents.data(i).dims(2)<>-1 then
    lhs_dims1=lhs_dims1+A.contents.data(i).dims(1)
    lhs_dims2=max(A.contents.data(i).dims(2),lhs_dims2)
  else 
    lhs_dims1=-1;
    lhs_dims2=-1;
  end
end
  tree.lhs(1).dims(1)=lhs_dims1;
  tree.lhs(1).dims(2)=lhs_dims2;
  end
elseif A.vtype==Unknown then
  tree.lhs(1).dims=list(-1,-1);
else
  error ("sci_char :" + string(A.vtype)  +" is not implemented"  )
end
elseif rhs>1 //char(t1,t2,t3,...)
  lhs_dims1=0;
  lhs_dims2=0;
  for i=1:rhs
    lhs_dims1=lhs_dims1+tree.rhs(i).dims(1)
    lhs_dims2=max(tree.rhs(i).dims(2),lhs_dims2)
  end
  tree.lhs(1).dims(1)=lhs_dims1;
  tree.lhs(1).dims(2)=lhs_dims2;
end
tree.lhs(1).type=Type(String,Real);
endfunction