function [tree]=%i2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation functions: mtlb_i() and mtlb_is()
// V.C.

// Global variable for M2SCI
global("varslist")

from=tree.operands($)
to=tree.operands(1)

// Special case for varargin/varargout
if or(to.name==["varargin","varargout"]) then
  ind=tree.operands(2)
  if type(ind)<>15 then
    tree=Variable(to.name,Infer())
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


// Ajust rhs value (variable and value to insert are part of rhs)
rhs=rhs-2

// Insertion with cells
if to.vtype==Cell | from.vtype==Cell then
  tree=%i_ce2sci(tree)
  return
end

// Insertion with structs
if to.vtype==Struct | from.vtype==Struct then
  tree=%i_st2sci(tree)
  return
end

// Just one index value
if rhs==1 then
  ind=tree.operands(2)
  // --- Insertion of strings ---
  if to.vtype==String & from.vtype==String then
    tree=list()
    to.dims=allunknown(to.dims)
    [bval,index]=isdefinedvar(to)
    if bval then
      varslist(index).infer.dims=allunknown(to.dims)
    end
    insert(Equal(list(to),Funcall("mtlb_is",1,Rhs(to,from,ind),list(to))),1)
  // --- Insertion with just one index ---
  elseif type(ind)<>15 then
    if ind.vtype==String then
      if typeof(ind)=="cste" then
	if ind.value<>":" then
	  tree=%i_st2sci(tree)
	  return
	end
      end
    end
    
    iscste=typeof(ind)=="cste"
    iscolon=%F
    if iscste then
      iscolon=ind.value==":"
    end
    
    // --- Insertion with just one index which is not : ---
    if ~iscolon then
      if is_empty(to) then // Insertion in an empty matrix
	if typeof(ind)=="cste" then
	  tree.out(1).dims=list(1,ind.value)
	else
	  tree.out(1).dims=list(1,Unknown)
	end
	tree.out(1).type=to.type
	tree=%i2sci_s(tree)
      else
	if is_a_scalar(to) then
	  tree=%i2sci_s(tree)
	elseif is_a_vector(to) & to.dims(2)<>1 then
	  tree=%i2sci_r(tree)
	elseif is_a_vector(to) & to.dims(1)<>1 then
	  tree=%i2sci_c(tree)
	else
	  tree=%i2sci_g(tree)
	  return
	end
      end
    // --- Insertion with just one index which is : ---
    else
      if is_empty(to) then // Insertion in an empty matrix
	if from.dims(2)<>1 then
	  if typeof(from)=="variable" then
	    if isdefinedvar(from) then
	      break
	    end
	  end
	  insert(Equal(list(to),list(from)))
	end
      end
      tree.out(1).dims=list(Unknown,1)
      tree.out(1).type=to.type
    end
  // --- Insertion with more than one index value (index is a list) ---
  else
    indexisstr=%F  
    iscell=%F
    for k=1:lstsize(ind)
      if type(ind(k))<>15 then
	if ind(k).vtype==String then
	  if typeof(ind(k))=="cste" & ind(k).value=="entries" then
	    iscell=%T
	  else
	    indexisstr=%T
	  end
	end
      end
    end
    if iscell then
      tree=%i_ce2sci(tree)
       return
    end
    if indexisstr then
      tree=%i_st2sci(tree)
      return
    end
    error("%i2sci: recursive insertion in a variable which is not a Cell nor a Struct: "+to.name)
  end
// Two indexes: to(ind1,ind2,...)=from or more
else
  if to.vtype==String & from.vtype==String then
    tree=list()
    to.dims=allunknown(to.dims)
    [bval,index]=isdefinedvar(to)
    if bval then
      varslist(index).infer.dims=allunknown(to.dims)
    end
    insert(Equal(list(to),Funcall("mtlb_is",1,Rhs(to,from,tree.operands(2),tree.operands(3)),list(to))),1)
  else
    tree.out(1).dims=list()
    for k=1:lstsize(tree.operands)-2
      tree.out(1).dims(k)=Unknown
    end
    // dim can be infered when index is a constant and when index value is greater than older dim and this dim is not unknown
    for kdim=1:size(tree.operands)-2
      if typeof(tree.operands(kdim+1))=="cste" & tree.operands(kdim+1).vtype<>String then
	if to.dims(kdim)<>Unknown then
	  if to.dims(kdim)<=tree.operands(kdim+1).value then
	    tree.out(1).dims(kdim)=tree.operands(kdim+1).value;
	  else
	    tree.out(1).dims(kdim)=to.dims(kdim)
	  end
	end
      end
    end
   if is_empty(to) then
      // a(k,:)=b with a==[] is converted by a(1,1:length(b))=b
      if lstsize(tree.operands)-2 == 2 & typeof(tree.operands($-1))=="cste" & tree.operands($-1).value==":" then
	length_funcall=Funcall("length",1,Rhs(tree.operands($)),list())
	tree.operands($-1)=Operation(":",list(Cste(1),length_funcall),list())
      end
      // a(:,k)=b with a==[] is converted by a(1:length(b),1)=b
      if lstsize(tree.operands)-2 == 2 & typeof(tree.operands($-2))=="cste" & tree.operands($-2).value==":" then
	length_funcall=Funcall("length",1,Rhs(tree.operands($)),list())
	tree.operands($-2)=Operation(":",list(Cste(1),length_funcall),list())
      end
    end
    tree.out(1).type=from.type
  end
end
endfunction

  