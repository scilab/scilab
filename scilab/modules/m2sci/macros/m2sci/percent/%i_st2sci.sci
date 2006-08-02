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
  varslist(index).infer.contents=Contents()
  tree.out(1).infer=Infer(list(0,0),Type(Struct,Unknown),Contents())
elseif typeof(to)=="operation" & to.vtype<>Struct then
  // To be sure that variable will now be of type Struct
  [bval,index]=isdefinedvar(to.operands(1))
  varslist(index).infer.type.vtype=Struct
  varslist(index).infer.contents=Contents()
  tree.out(1).infer=Infer(list(0,0),Type(Struct,Unknown),Contents())
end

// Just one index value
if rhs==1 then
  ind=tree.operands(2)
  // --- Insertion with just one index ---
  if type(ind)<>15 then
    // --- Insertion in a struct with just one index ---
    if ind.vtype==String then // A.f
      tree.out(1).dims=list(1,1);
      tree.out(1).vtype=Struct
      tree.out(1).contents.index($+1)=list(list(Cste(1),Cste(1)),ind)
      tree.out(1).contents.data($+1)=from.infer
    else
      if from.vtype<>Double then // X(p)=struct(...)
	tree.operands(2)=list(Cste(1),tree.operands(2))
	tree.out(1).vtype=Struct
	if typeof(ind)=="cste" then
	  if ind.vtype<>String then // Not :
	    tree.out(1).dims=list(1,ind.value)
	    
	    tree.out(1).contents.index($+1)=list(Cste(1),ind)
	    
	    // Update contents for an extraction of type: z = X(p)
	    CONT=Contents()
	    for k=1:lstsize(from.infer.contents.index)
	      if type(from.contents.index(k)(1))==15 then
		CONT.index($+1)=list(from.contents.index(k)(2))
	      else
		CONT.index($+1)=list(from.contents.index(k))
	      end
	      CONT.data($+1)=from.contents.data(k)
	    end
	    tree.out(1).contents.data($+1)=Infer(list(1,1),Type(Struct,Unknown),CONT)

	    // Update contents for extraction of type: z = X(p).f
	    for k=1:lstsize(from.infer.contents.index)
	      if type(from.contents.index(k)(1))==15 then
		tree.out(1).contents.index($+1)=list(list(Cste(1),ind),from.contents.index(k)(2))
	      else
		tree.out(1).contents.index($+1)=list(list(Cste(1),ind),from.contents.index(k))
	      end
	      tree.out(1).contents.data($+1)=from.contents.data(k)
	    end
	  else
	    tree.out(1).dims=from.dims
	    tree.out(1).contents=from.contents
	  end
	end
      else
	if is_empty(from) then // Clear element: A(p)=[]
	  // Nothing done
	else // Change type of variable
	  error("Not yet implemented");
	end
      end
    end
  // --- Insertion with more than one index value (index is a recursive index list) --- 
  else
  
    // Change index value if just one double
    for k=1:lstsize(ind)
      //ind(k+1) <-> tree.operands(2)(k+1)
      if typeof(ind(k))=="cste" | (typeof(ind(k))<>"list" & is_a_scalar(ind(k))) then 
	if ind(k).vtype<>String then
	  tree.operands(2)(k)=list(Cste(1),tree.operands(2)(k))
	end
      end
    end
    ind=tree.operands(2);
    
    if typeof(ind($))=="list" | ind($).vtype~=String then // X.p(m,n)=y
      tmp=gettempvar()
      oplist=list()
      
      tmpind=ind
      tmpind($)=null()
      if or(get_contents_infer(tree.operands(1),tmpind)<>Infer()) then
	tmp.infer=get_contents_infer(tree.operands(1),tmpind)
      end
      oplist(1)=tmp

      for kind=1:size(ind($))
	oplist($+1)=ind($)(kind)
      end

      oplist($+1)=tree.operands($)
      
      newop=Operation("ins",oplist,list(tmp))
      newop=%i2sci(newop)
      tree.out(1).infer.contents.index($+1)=tmpind
      tree.out(1).infer.contents.data($+1)=newop.out(1).infer
    end
    
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
	if typeof(possible_dims(k))<>"cste" then
	  infdims(k)=Unknown
        elseif infdims(k)<>Unknown & infdims(k)<possible_dims(k).value then
	  infdims(k)=possible_dims(k).value
	end
      end
      tree.out(1).infer.contents.index($+1)=ind
      tree.out(1).infer.contents.data($+1)=from.infer
      tree.out(1).dims=infdims
      tree.out(1).type=Type(Struct,Unknown)
      // A.b.f
    else 
      tree.out(1).dims=list(1,1)
      tree.out(1).type=Type(Struct,Unknown)
      tree.out(1).infer.contents.index($+1)=ind
      tree.out(1).infer.contents.data($+1)=from.infer
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
	  tree.out(1).dims(kdim)=to.dims(kdim)
	end
      end
    end
  end
  tree.out(1).type=from.type
    
  // Update contents
  ind=tree.operands
  ind(1)=null()
  ind($)=null()
  tree.out(1).infer.contents.index($+1)=ind
  tree.out(1).infer.contents.data($+1)=from.infer

end
endfunction

  