function [tree]=sci_max(tree)
// File generated from sci_PROTO1.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab max()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_max()
// V.C.

// C = max(A) or [C,I] = max(A)
if rhs==1 then
  A = getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown // If A is a scalar then Matlab return Double type value else a Boolean type value
  end
  A = convert2double(A)
  tree.rhs = Rhs(A)
  dim = first_non_singleton(A)
  tree.rhs=Rhs(A,'m') 
  if dim==-1 then
    // All output dims are unknown
    tree.lhs(1).dims=allunknown(A.dims)
    //tmp=gettempvar() F.B: 9/11/2005//
    //insert(Equal(list(tmp),A)) F.B: 9/11/2005//
    // First non singleton dimension will be computed at execution
    //tree.rhs=Rhs(tmp,Funcall("firstnonsingleton",1,list(tmp),list())) F.B: 9/11/2005//
  else
    tree.lhs(1).dims=A.dims
    if dim==0 then
      tree.rhs=Rhs(A)
      tree.lhs(1).dims=list(1,1)
    else
      tree.lhs(1).dims(dim)=1
    end
  end
  
  // C = max(A) or [C,I] = max(A)
  if is_real(A) then
    tree.lhs(1).type=Type(vtype,Real)
  elseif is_complex(A) then
    // Scilab max() does not work with complexes so mtlb_max() is called
    tree.name="mtlb_max"
    tree.lhs(1).type=Type(Double,Unknown)
  else
    // Scilab max() does not work with complexes so mtlb_max() is called but can be replaced
    tree.name="mtlb_max"
    tree.lhs(1).type=Type(vtype,Unknown)
  end
  
  // [C,I] = max(A)
  if lhs==2 then 
    // All dims Unknown because if A is [] then I is also [] else I is a row vector
    tree.lhs(2).dims=allunknown(A.dims)
    tree.lhs(2).type=Type(Double,Real)
  end
// C = max(A,B)
elseif rhs==2 then
  [A,B]=getrhs(tree)
  vtype=Double
  if A.vtype==Boolean & B.vtype==Boolean | A.vtype==Unknown & B.vtype==Unknown then
    vtype=Unknown
  end
  A=convert2double(A)
  B=convert2double(B)
  tree.rhs=Rhs(A,B)

  if is_real(A) & is_real(B) then 
    if not_empty(A) & not_empty(B) then
      tree.lhs(1).dims=A.dims
      tree.lhs(1).type=Type(vtype,Real)
    else
      // Perhaps an input is an empty matrix
      tree.name="mtlb_max"
      tree.lhs(1).dims=A.dims
      tree.lhs(1).type=Type(vtype,Real)
    end
  else
    // Inputs can be complexes and/or empty matrices
    tree.name="mtlb_max"
    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(vtype,Unknown)
  end
// C = max(A,[],dim) or [C,I] = max(A,[],dim)   
else
  [A,tmp,dim]=getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown
  end
  A=convert2double(A)
  tree.rhs=Rhs(A)

  // C = max(A,[],dim) or [C,I] = max(A,[],dim)
  if or(lhs==[1,2]) then
    if is_real(A) then
      tree.lhs(1).type=Type(Double,Real)
      if typeof(dim)=="cste" then
	if dim.value==1 then
	  tree.rhs=Rhs(A,"r")
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(1)=Unknown // 0 or 1
	elseif dim.value==2 then
	  tree.rhs=Rhs(A,"c")
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(2)=Unknown // 0 or 1
	elseif dim.value<=size(A.dims) then
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(dim.value)=1
	else
	  // Scilab max() does not work when dim  is greater than number of dims of A
	  tree.name="mtlb_max"
	  tree.rhs=Rhs(A,tmp,dim)
	  tree.lhs(1).dims=A.dims
	end
      else
	// If dim is 1 it can be replaced by 'r'
	// If dim is 2 it can be replaced by 'c'
	tree.name="mtlb_max"
	tree.rhs=Rhs(A,tmp,dim)
	tree.lhs(1).dims=allunknown(A.dims)
      end
    else
      // A can be complex....
      tree.name="mtlb_max"
      tree.rhs=Rhs(A,tmp,dim)
      tree.lhs(1).dims=allunknown(A.dims)
      tree.lhs(1).type=Type(Double,Unknown)
    end
  end
  
  // [C,I] = max(A,[],dim)
  if lhs==2 then
    tree.lhs(2).type=Type(Double,Real)
    if is_real(A) then
      if typeof(dim)=="cste" then
	if dim.value<=size(A.dims) then
	  tree.lhs(2).dims=allunknown(A.dims)
	else
	  tree.lhs(2).dims=A.dims
	end
      else
	tree.lhs(2).dims=allunknown(A.dims)
      end
    else
      tree.lhs(2).dims=allunknown(A.dims)
    end
  end
end
endfunction
