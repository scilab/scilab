function [tree]=sci_min(tree)
// File generated from sci_PROTO1.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab min()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_min()
// V.C.

// C = min(A) or [C,I] = min(A)
if rhs==1 then
  A = getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown // If A is a scalar then Matlab return Double type value else a Boolean type value
  end
  A = convert2double(A)
  tree.rhs = Rhs(A)
  dim = first_non_singleton(A)

  if dim==-1 then
    // All output dims are unknown
    tree.lhs(1).dims=allunknown(A.dims)
    tmp=gettempvar()
    insert(Equal(list(tmp),A))
    // First non singleton dimension will be computed at execution
    tree.rhs=Rhs(tmp,Funcall("firstnonsingleton",1,list(tmp),list()))
  else
    tree.lhs(1).dims=A.dims
    if dim==0 then
      tree.rhs=Rhs(A)
      tree.lhs(1).dims=list(1,1)
    elseif dim==1 then
      tree.rhs=Rhs(A,"r")
      tree.lhs(1).dims(dim)=1
    elseif dim==2 then
      tree.rhs=Rhs(A,"c")
      tree.lhs(1).dims(dim)=1
    else
      tree.rhs=Rhs(A,dim)
      tree.lhs(1).dims(dim)=1
    end
  end
  
  // C = min(A) or [C,I] = min(A) 
  if is_real(A) then
    tree.lhs(1).type=Type(vtype,Real)
    if dim==-1 then
      repl1=tree
      repl1.rhs=Rhs(A)
      repl2=tree
      repl2.rhs=Rhs(A,"r")
      repl_poss(tree,..
	  repl1,A,"is a scalar or a vector",..
	  repl2,A,"is a matrix")
    end
  elseif is_complex(A) then
    tree.name="mtlb_min"
    tree.lhs(1).type=Type(Double,Unknown)
    set_infos("Scilab min() does not work with complexes so mtlb_min() is called",0)
  else
    tree.name="mtlb_min"
    tree.lhs(1).type=Type(vtype,Unknown)
    repl1=tree
    repl1.name="min"
    repl_poss(tree,..
	repl1,A,"is not complex")
  end
  
  // [C,I] = min(A)
  if lhs==2 then 
    // All dims Unknown because if A is [] then I is also [] else I is a row vector
    tree.lhs(2).dims=allunknown(A.dims)
    tree.lhs(2).type=Type(Double,Real)
  end
// C = min(A,B)
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
      tree.name="mtlb_min"
      tree.lhs(1).dims=A.dims
      tree.lhs(1).type=Type(vtype,Real)
      repl_poss(tree,..
	  tree,list(A,B),"are not empty matrices")
    end
  else
    tree.name="mtlb_min"
    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(vtype,Unknown)
    repl_poss(tree,..
	tree,list(A,B),"are not empty matrices and real values")
  end
// C = min(A,[],dim) or [C,I] = min(A,[],dim)   
else
  [A,tmp,dim]=getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown
  end
  A=convert2double(A)
  tree.rhs=Rhs(A)

  // C = min(A,[],dim) or [C,I] = min(A,[],dim)
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
	  set_infos("Scilab min() does not work when dim input argument is greater than number of dims of first rhs...",1)
	  tree.name="mtlb_min"
	  tree.rhs=Rhs(A,tmp,dim)
	  tree.lhs(1).dims=A.dims
	end
      else
	tree.name="mtlb_min"
	tree.rhs=Rhs(A,tmp,dim)
	tree.lhs(1).dims=allunknown(A.dims)
	repl1=tree;repl1.rhs=Rhs(A,"r")
	repl2=tree;repl2.rhs=Rhs(A,"c")
	repl_poss(tree,..
	    repl1,dim,"is equal to 1",..
	    repl2,dim,"is equal to 2")
      end
    else
      set_infos("See M2SCI documentation for mtlb_min() replacement possibilities",1)
      tree.name="mtlb_min"
      tree.rhs=Rhs(A,tmp,dim)
      tree.lhs(1).dims=allunknown(A.dims)
      tree.lhs(1).type=Type(Double,Unknown)
    end
  end
  
  // [C,I] = min(A,[],dim)
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
