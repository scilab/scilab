function [tree]=sci_sparse(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab sparse()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// nzmax does not exist in Scilab
if rhs==6 then
  rhs=5
  set_infos("Call to sparse(): nzmax option ignored...",0)
end

// S = sparse(A)
if rhs==1 then
  A = getrhs(tree)
  // %c_sparse and %b_sparse are not defined
  A = convert2double(A)
  tree.rhs=Rhs(A)
  
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=Type(Sparse,A.property)

// S = sparse(m,n)
elseif rhs==2 then
  [m,n] = getrhs(tree)
  // %c_sparse and %b_sparse are not defined
  m = convert2double(m)
  n = convert2double(n)
  tree.rhs=Rhs([],[],Operation("rc",list(m,n),list()))
  
  dims=list(Unknown,Unknown)
  if typeof(m)=="cste" then
    dims(1)=m.value
  end
  if typeof(n)=="cste" then
    dims(2)=n.value
  end
  tree.lhs(1).dims=dims
  tree.lhs(1).type=Type(Sparse,Real)

// S = sparse(i,j,s)
elseif rhs==3 then
  [i,j,s] = getrhs(tree)
  // %c_sparse and %b_sparse are not defined
  i = convert2double(i)
  j = convert2double(j)
  s = convert2double(s)
  tree.rhs=Rhs(i,j,s)
  
  // Matlab tolerates length(i)<>length(j)...
  if size(i.dims)<>size(j.dims) | size(i.dims)<>size(s.dims) then
    set_infos("For Scilab sparse(), input values must have the same size, not sure they have...",2)
  end
  
  if i.dims(2)==1 & j.dims(2)==1 then
    tree.rhs=Rhs(Operation("rc",list(i,j),list()),s)
  elseif typeof(i)=="variable" & typeof(j)=="variable" then
    tree.rhs=Rhs(Operation("rc",list(Operation("ext",list(i,Cste(":")),list()),Operation("ext",list(j,Cste(":")),list())),list()),s)
  else
    if typeof(i)<>"variable" then
      V1=gettempvar(1)
      insert(Equal(list(V1),i))
    else
      V1=i
    end
    if typeof(j)<>"variable" then
      V2=gettempvar(2)
      insert(Equal(list(V2),j))
    else
      V2=j
    end
    tree.rhs=Rhs(Operation("rc",list(Operation("ext",list(V1,Cste(":")),list()),Operation("ext",list(V2,Cste(":")),list())),list()),s)
  end
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Sparse,s.property)
elseif rhs==4 then
  // Can not happen
//S = sparse(i,j,s,m,n)
elseif rhs==5 then
  
  [i,j,s,m,n] = getrhs(tree)
  i = convert2double(i)
  j = convert2double(j)
  s = convert2double(s)
  m = convert2double(m)
  n = convert2double(n)

  tree.rhs=Rhs(i,j,s,m,n)

  if i.dims(2)==1 & j.dims(2)=="1" then
    tree.rhs=Rhs(Operation("rc",list(i,j),list()),s)
  elseif typeof(i)=="variable" & typeof(j)=="variable" then
    rhs1=Operation("rc",list(Operation("ext",list(i,Cste(":")),list()),Operation("ext",list(j,Cste(":")),list())),list())
    rhs3=Operation("rc",list(m,n),list())
    tree.rhs=Rhs(rhs1,s,rhs3)
  else
    if typeof(i)<>"variable" then
      V1=gettempvar(1)
      insert(Equal(list(V1),i))
    else
      V1=i
    end
    if typeof(j)<>"variable" then
      V2=gettempvar(2)
      insert(Equal(list(V2),j))
    else
      V2=j
    end
    rhs1=Operation("rc",list(Operation("ext",list(V1,Cste(":")),list()),Operation("ext",list(V2,Cste(":")),list())),list())
    rhs3=Operation("rc",list(m,n),list())
    tree.rhs=Rhs(rhs1,s,rhs3)
  end
  tree.lhs(1).dims=list(Unknown,Unknown)
  tree.lhs(1).type=Type(Sparse,s.property)
end

endfunction
