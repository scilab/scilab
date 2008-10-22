function [tree]=sci_diag(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab diag()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_diag()
// V.C.

// B = diag(A,k)
if rhs==2 then
  [A,k] = getrhs(tree)
  // In Scilab k must be a real or complex matrix
  k = convert2double(k)
  tree.rhs=Rhs(A,k)

  // Compute dims to for inference
  if typeof(k)=="cste" then
    if A.dims(1)==1 then // diag of a row vector
      n=A.dims(2)+k.value
      m=n
    elseif A.dims(2)==1 then // diag of a column vector
      n=A.dims(1)+k.value
      m=n
    else
      m=Unknown;
      n=Unknown;
    end
  else
    m=Unknown;
    n=Unknown;
  end
  tree.lhs(1).dims=list(m,n)

  if or(A.vtype==[String,Unknown]) then
    tree.name="mtlb_diag"
    tree.lhs(1).type=A.type
  else
    if A.vtype==Boolean then
      tree.rhs=Rhs(Funcall("bool2s",1,list(A),list()),k)
      tree.lhs(1).type=Type(Boolean,Real)
    else
      tree.lhs(1).type=A.type
    end
  end
// B = diag(A)
else
  A = getrhs(tree)

  // Compute dims to for inference
  if A.dims(1)==1 then // diag of a row vector
    n=A.dims(2)
    m=n
  elseif A.dims(2)==1 then // diag of a column vector
    n=A.dims(1)
    m=n
  elseif not_a_vector(A) then
    m=min(A.dims);
    if not_empty(A) then
      n=1
    else
      n=Unknown;
    end
  else
    m=Unknown;
    n=Unknown;
  end
  tree.lhs(1).dims=list(m,n)

  if or(A.vtype==[String,Unknown]) then
    tree.name="mtlb_diag"
    tree.lhs(1).type=A.type
  else
    if A.vtype==Boolean then
      tree.rhs=Rhs(Funcall("bool2s",1,list(A),list()))
      tree.lhs(1).type=Type(Boolean,Real)
    else
      tree.lhs(1).type=A.type
    end
  end
end
endfunction
