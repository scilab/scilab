function [tree]=%e2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab extraction
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_e()
// V.C.

// Global variable for M2SCI
global("varslist")

var=tree.operands(1)

// Special case for varargin/varargout
if or(var.name==["varargin","varargout"]) then
  ind=tree.operands(2)
  if type(ind)<>15 then
    tree=Variable(var.name,Infer())
  else
    if type(ind(1))==15 | ind(1).vtype<>String then
      tree.operands(2)(2)=null() // Del 'entries'
    elseif typeof(ind(1))=="cste" then 
      tree.operands(2)(2)=null() // Del 'entries'
    else
      tree.operands(2)(1)=null() // Del 'entries'
    end
  end
  return
end

// Extraction from cells
if var.vtype==Cell then
  tree=%e_ce2sci(tree)
  return
end

// Extraction from structs
if var.vtype==Struct then
  tree=%e_st2sci(tree)
  return
end

// Do not consider variable as a rhs...
rhs=rhs-1

// One index value
if rhs==1 then
  ind=tree.operands(2)
  // --- Recursive extraction ---
  if type(ind)==15 then
    for kind=1:lstsize(ind)
      if type(ind(kind))<>15 then
	if ind(kind).vtype==String then
	  if ind(kind).value=="entries" then
	    tree.operands(1).vtype=Cell
	    rhs=rhs+1
	    tree=%e_ce2sci(tree)
	    return
	  else
	    tree.operands(1).vtype=Struct
	    rhs=rhs+1
	    tree=%e_st2sci(tree)
	    return
	  end
	end
      end
    end
    error("%e2sci: recursive extraction from a variable "+var.name+" of type "+string(var.vtype))
  elseif var.vtype==String then // Character string extraction
    tree=Funcall("part",1,Rhs(var,ind),tree.out)
    if is_a_scalar(ind) then
      tree.lhs(1).dims=list(1,1)
    else
      tree.lhs(1).dims=list(1,ind.dims(2))
    end
    tree.lhs(1).type=var.type
  else // Extraction x(i)
    if var.vtype==Unknown then // Unknown type -> can be String
      tree=Funcall("mtlb_e",1,Rhs(var,ind),tree.out)
      tree.lhs(1).dims=list(Unknown,Unknown)
      tree.lhs(1).type=var.type
    else
      if typeof(ind)=="cste" then
	if ind.value==":" then
	  if var.dims(1)<>Unknown & var.dims(2)<>Unknown then
	    tree.out(1).dims=list(var.dims(1)*var.dims(2),1)
	    tree.out(1).type=var.type
	  else
	    tree.out(1).dims=list(Unknown,1)
	    tree.out(1).type=var.type
	  end
	  return
	end
      end
      
      if ind.dims(1)==1 & ind.dims(2)==1 then
	tree.out(1).dims=list(1,1)
	tree.out(1).type=var.type
      elseif var.dims(1)==1 then
	tree.out(1).dims=list(1,Unknown)
	tree.out(1).type=var.type
      elseif var.dims(2)==1 then
	tree.out(1).dims=list(Unknown,1)
	tree.out(1).type=var.type
      elseif ind.dims(2)==1 then
	tree.out(1).dims=list(Unknown,1)
	tree.out(1).type=var.type
      elseif ind.dims(1)==1 then
	tree.out(1).dims=list(1,Unknown)
	tree.out(1).type=var.type
      elseif var.dims(1)<>Unknown & var.dims(2)<>Unknown then
	tree=Operation(".''",list(tree),tree.out)
	tree.out(1).dims=list(Unknown,1)
	tree.out(1).type=var.type
      else // at leat one dimension unknown
	tree=Funcall("mtlb_e",1,Rhs(var,ind),tree.out)
	tree.lhs(1).dims=list(Unknown,Unknown)
	tree.lhs(1).type=var.type
      end
    end
  end
// No rhs: for example variable display  
elseif rhs==0 then
  
// More than one index value
else 
  dims=list()
  for k=2:rhs+1
    dimsum=0
    for l=1:size(tree.operands(k).dims)
      dimsum=dimsum+tree.operands(k).dims(l)
    end
    if dimsum==size(tree.operands(k).dims) // All dims are 1
      dims(k-1)=1
      if typeof(tree.operands(k))=="cste" then
	if tree.operands(k).value==":" then
	  if k<=lstsize(var.dims)+1 then
	    dims(k-1)=var.dims(k-1);
	  else
	    dims(k-1)=Unknown;
	  end
	end
      end
    else
      dims(k-1)=Unknown
    end
  end

  if var.vtype==String then // extraction in strings
    if rhs==2 then
      rhsarg=Operation("ext",list(var,tree.operands(2)),list())
      tree=Funcall("part",1,Rhs(rhsarg,tree.operands(3)),tree.out)
      tree.lhs(1).dims=list(dims(1:2))
      tree.lhs(1).type=var.type
    else
      error("%e2sci(): Extraction from strings with more than two indexes not implemented!")
    end
  else
    tree.out(1).dims=dims
    tree.out(1).type=var.type
  end
end
endfunction

  