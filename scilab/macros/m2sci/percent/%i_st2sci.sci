function [tree]=%i_st2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion in structs
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation functions: mtlb_i() and mtlb_is()
// V.C.

// Global variable for M2SCI
global("varslist")

from=tree.operands($)
to=tree.operands(1)

// Insertion of a struct in a not-struct array
if typeof(to)=="variable" & to.vtype<>Struct then
  
  // To be sure that variable will now be of type Struct
  [bval,index]=isdefinedvar(to)
  varslist(index).infer.type.vtype=Struct
  
  // No more needed
  // insert(Equal(list(to),Funcall("struct",1,list(),list(to))))
  
  tree.out(1).infer=Infer(list(0,0),Type(Struct,Unknown),struct())
elseif typeof(to)=="operation" & to.vtype<>Struct then
  
  // To be sure that variable will now be of type Struct
  [bval,index]=isdefinedvar(to.operands(1))
  varslist(index).infer.type.vtype=Struct

  // No more needed
  // insert(Equal(list(to),Funcall("struct",1,list(),list(to))))
  
  tree.out(1).infer=Infer(list(0,0),Type(Struct,Unknown),struct())
end

// Just one index value
if rhs==1 then
  ind=tree.operands(2)
  // --- Insertion with just one index ---
  if type(ind)<>15 then
    // --- Insertion in a struct with just one index ---
    if ind.vtype==String then
      if to.vtype<>Struct then
	to.type=Type(Struct,Unknown)
	to.contents=struct(ind.value,from.infer)
      end
      to.dims=list(1,1)
      tree.out(1).infer=to.infer
      tree.out(1).contents(ind.value)=from.infer
    else
      // X(p)=struct(...)
      tree.operands(2)=list(Cste(1),tree.operands(2))
      if typeof(ind)=="cste" then
	if ind.vtype<>String then // Not :
	  tree.out(1).dims=list(1,ind.value)
	  if typeof(tree.out(1).contents)<>"st" then
	    tree.out(1).contents=struct()
	  end
	  execstr("tree.out(1).contents"+expression2code(tree.operands(2))+"=from.contents")
	else
	  tree.out(1).dims=from.dims
	  execstr("tree.out(1).contents=from.contents.entries")
	end
      else
	execstr("tree.out(1).contents=Infer()")
      end
      tree.out(1).vtype=Struct
    end
  // --- Insertion with more than one index value (index is a list) --- 
  else
  
    // Change index value if just one double
    for k=1:lstsize(ind)
      //ind(k+1) <-> tree.operands(2)(k+1)
      if typeof(ind(k))=="cste" then 
	if ind(k).vtype<>String then
	  tree.operands(2)(k)=list(Cste(1),tree.operands(2)(k))
	end
      end
    end
    
    if can_infer(tree.operands(2)) then
      // Inference can be done
      
      tree.out(1).infer=to.infer
      
      infertree=tree.operands(2)
      
      // A(x,y,...).f
      if typeof(infertree(1))=="list" then
	possible_dims=infertree(1)
	infdims=tree.out(1).dims
	if lstsize(infdims)<lstsize(possible_dims) then
	  for k=lstsize(infdims)+1:lstsize(possible_dims)
	    infdims(k)=Unknown
	  end
	end
	for k=1:lstsize(possible_dims)
	  if infdims(k)<>Unknown & infdims(k)<possible_dims(k).value then
	    infdims(k)=possible_dims(k).value
	  end
	end
	execstr("tree.out(1).contents"+expression2code(infertree)+"=1") // Should be removed
	execstr("tree.out(1).contents"+expression2code(infertree)+"=from.infer")
	// A.f
      else
	infdims=list(1,1)
	tree.out(1).contents=struct()
	tree.out(1).contents(infertree(1).value)=Infer()
      end
      tree.out(1).type=Type(Struct,Unknown)
      tree.out(1).dims=infdims
    else
      // Inference can not be done
      tree.out(1).type=Type(Struct,Unknown)
      tree.out(1).dims=list(Unknown,Unknown)
    end
    
  end
// Two indexes: to(ind1,ind2,...)=from or more
else
  tree.out(1).dims=list()
  for k=1:lstsize(tree.operands)-2
    tree.out(1).dims(k)=Unknown
  end
  
  // dim can be infered when index is a constant and when index value is greater than older dim and this dim is not unknown
  for kdim=1:size(tree.operands)-2
    if typeof(tree.operands(kdim+1))=="cste" then
      if to.dims(kdim)<>Unknown then
	if to.dims(kdim)<=tree.operands(kdim+1).value then
	  tree.out(1).dims(kdim)=tree.operands(kdim+1).value;
	else
	  tree.out(1).dims(kdim)=to.dims(k)
	end
      end
    end
  end
  tree.out(1).type=from.type
    
  // Update contents

  //Verify that all indexes are constants
  cste_nb=0
  for kind=1:size(tree.operands)-2
    if typeof(tree.operands(kind+1))=="cste" then
      cste_nb=cste_nb+1
    end
  end
  
  if cste_nb==size(tree.operands)-2 then
    indexes=[]
    for kind=1:size(tree.operands)-2
      indexes=[indexes,string(tree.operands(kind+1).value)]
    end
    ind=tree.operands
    ind(1)=null()
    ind($)=null()
    indexes=strcat(indexes,",")
    if typeof(to.contents)<>"constant" then
      tree.out(1).contents=to.contents
    else
       execstr("tree.out(1).contents"+expression2code(ind)+"=1"); // Should be removed
    end
    
    execstr("tree.out(1).contents"+expression2code(ind)+"=from.infer.contents");
  end
end
errclear();
endfunction

  