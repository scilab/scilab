// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_MFUN(tree)
// Prototype for max and min
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()

// C = MFUN(A) or [C,I] = MFUN(A)
if rhs==1 then
  A = getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown // If A is a scalar then Matlab return Double type value else a Boolean type value
  end
  A = convert2double(A)
  tree.rhs = Rhs_tlist(A)
  dim = first_non_singleton(A)

  if dim==-1 then
    // All output dims are unknown
    tree.lhs(1).dims=allunknown(A.dims)
    tmp=gettempvar()
    m2sci_insert(Equal(list(tmp),A))
    // First non singleton dimension will be computed at execution
    tree.rhs=Rhs_tlist(tmp,Funcall("firstnonsingleton",1,list(tmp),list()))
  else
    tree.lhs(1).dims=A.dims
    if dim==0 then
      tree.rhs=Rhs_tlist(A)
      if is_empty(A) then
        tree.lhs(1).dims=A.dims
      else
        tree.lhs(1).dims=list(1,1)
      end
    elseif dim==1 then
      tree.rhs=Rhs_tlist(A,"r")
      tree.lhs(1).dims(dim)=1
    elseif dim==2 then
      tree.rhs=Rhs_tlist(A,"c")
      tree.lhs(1).dims(dim)=1
    else
      tree.rhs=Rhs_tlist(A,dim)
      tree.lhs(1).dims(dim)=1
    end
  end
  
  // C = MFUN(A) or [C,I] = MFUN(A) 
  if is_real(A) then
    tree.lhs(1).type=Type(vtype,Real)
  elseif is_complex(A) then
    // Scilab MFUN() does not work with complexes so mtlb_MFUN() is called
    tree.name="mtlb_MFUN"
    tree.lhs(1).type=Type(Double,Unknown)
  else
    // Scilab MFUN() does not work with complexes so mtlb_MFUN() is called but can be replaced
    tree.name="mtlb_MFUN"
    tree.lhs(1).type=Type(vtype,Unknown)
  end
  
  // [C,I] = MFUN(A)
  if lhs==2 then 
    // All dims Unknown because if A is [] then I is also [] else I is a row vector
    tree.lhs(2).dims=allunknown(A.dims)
    tree.lhs(2).type=Type(Double,Real)
  end
// C = MFUN(A,B)
elseif rhs==2 then
  [A,B]=getrhs(tree)
  vtype=Double
  if A.vtype==Boolean & B.vtype==Boolean | A.vtype==Unknown & B.vtype==Unknown then
    vtype=Unknown
  end
  A=convert2double(A)
  B=convert2double(B)
  tree.rhs=Rhs_tlist(A,B)

  if is_real(A) & is_real(B) then 
    if not_empty(A) & not_empty(B) then
      tree.lhs(1).dims=A.dims
      tree.lhs(1).type=Type(vtype,Real)
    else
      // Perhaps an input is an empty matrix
      tree.name="mtlb_MFUN"
      tree.lhs(1).dims=A.dims
      tree.lhs(1).type=Type(vtype,Real)
    end
  else
    // Inputs can be complexes and/or empty matrices
    tree.name="mtlb_MFUN"
    tree.lhs(1).dims=A.dims
    tree.lhs(1).type=Type(vtype,Unknown)
  end
// C = MFUN(A,[],dim) or [C,I] = MFUN(A,[],dim)   
else
  [A,tmp,dim]=getrhs(tree)
  vtype=Double
  if or(A.vtype==[Boolean,Unknown]) then
    vtype=Unknown
  end
  A=convert2double(A)
  tree.rhs=Rhs_tlist(A)

  // C = MFUN(A,[],dim) or [C,I] = MFUN(A,[],dim)
  if or(lhs==[1,2]) then
    if is_real(A) then
      tree.lhs(1).type=Type(vtype,Real)
      if typeof(dim)=="cste" then
	if dim.value==1 then
	  tree.rhs=Rhs_tlist(A,"r")
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(1)=Unknown // 0 or 1
	elseif dim.value==2 then
	  tree.rhs=Rhs_tlist(A,"c")
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(2)=Unknown // 0 or 1
	elseif dim.value<=size(A.dims) then
	  tree.lhs(1).dims=A.dims
	  tree.lhs(1).dims(dim.value)=1
	else
	  // Scilab MFUN() does not work when dim  is greater than number of dims of A
	  tree.name="mtlb_MFUN"
	  tree.rhs=Rhs_tlist(A,tmp,dim)
	  tree.lhs(1).dims=A.dims
	end
      else
	// If dim is 1 it can be replaced by 'r'
	// If dim is 2 it can be replaced by 'c'
	tree.name="mtlb_MFUN"
	tree.rhs=Rhs_tlist(A,tmp,dim)
	tree.lhs(1).dims=allunknown(A.dims)
      end
    else
      // A can be complex....
      tree.name="mtlb_MFUN"
      tree.rhs=Rhs_tlist(A,tmp,dim)
      tree.lhs(1).dims=allunknown(A.dims)
      tree.lhs(1).type=Type(vtype,Unknown)
    end
  end
  
  // [C,I] = MFUN(A,[],dim)
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
