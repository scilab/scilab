function [tree]=%e_st2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab extraction from structs
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// Emulation function: mtlb_e()
// V.C.

// Global variable for M2SCI
global("varslist")

var=tree.operands(1)

// Do not consider variable as a rhs...
rhs=rhs-1

// One index value
if rhs==1 then
  ind=tree.operands(2)
  
  // --- Recursive extraction ---
  if type(ind)==15 then
    // If last index value is not a fieldname, it is ignored
    // it will be considered after all other indexes
    // Avoid some extraction problems from character strings
    lastisnotfield=typeof(ind($))=="list"
    if ~lastisnotfield then
      lastisnotfield=ind($).vtype<>String
    end
      
    // Inference can be done (all indexes values are known)
    if can_infer(ind) then
      infertree=tree.operands(2)
      if lastisnotfield then // Last index is not a fieldname, ignored here
	infertree($)=null()
      end
      indtxt=expression2code(infertree)
      
      [bval,index]=isdefinedvar(var)
      if ~bval then
	error("M2sci bug: extraction from unknown variable "+var.name+" in varslist")
      else
	INFER=varslist(index).infer
	ierr=execstr("INFER=varslist(index).infer.contents"+expression2code(infertree),"errcatch");
	if ierr<>0 | typeof(INFER)<>"infer" then // infer==[] or non-exsisting index in inference data
	  // Inference can not be done
	else
	  tree.out(1).infer=INFER
	end
      end
    else
      // Inference can not be done
    end
    
    // Convert last extraction operation is not already done
    if lastisnotfield then
      newtree=tree
      newtree.operands(2)($)=null()
      tmp=gettempvar()
      insert(Equal(list(tmp),newtree))
      // tmp variable added to varslist so that inference can be done
      varslist($+1)=M2scivar(tmp.name,tmp.name,newtree.infer)
      
      // Change index
      IND=list()
      tmp.infer=newtree.infer
      IND(1)=tmp
      if typeof(tree.operands(2)($))=="list" then
	for k=1:lstsize(tree.operands(2)($))
	  IND($+1)=tree.operands(2)($)(k)
	end
      else
	IND(2)=tree.operands(2)($)
      end
      tree=Operation("ext",IND,tree.out)
      rhs=lstsize(IND) // Done in operation2sci for other cases
      tree=%e2sci(tree)
    end

  else
    // extraction from struct
    // this case should happen only if var(ind.value) contains only one value
    
    [bval,index]=isdefinedvar(var)
    if ~bval then
      error("M2sci bug: extraction from unknown variable "+var.name+" in varslist")
    else
      if varslist(index).vtype<>Struct then
	// variable not defined as a struct, modify inference
	varslist(index).infer.vtype=Struct
	varslist(index).infer.contents=struct()
      end
      INFER=varslist(index).infer
    end
    INF=INFER
    ierr=execstr("INF=INFER.contents(ind.value)","errcatch")
    if ierr<>0 then
      tree.out(1).vtype=Struct
    elseif typeof(INF)<>"infer" then
      tree.out(1).vtype=Struct
    else
      tree.out(1).dims=INF.dims
      tree.out(1).type=INF.type
      tree.out(1).contents=INF.contents
    end
  end
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
	  dims(k-1)=var.dims(k-1)
	end
      end
    else
      dims(k-1)=Unknown
    end
  end

  tree.out(1).dims=dims
  tree.out(1).type=var.type
  
  cste_nb=0
  indexes=[]
  for kind=2:size(tree.operands)
    if typeof(tree.operands(kind))=="cste" then
      cste_nb=cste_nb+1
      indexes=[indexes,string(tree.operands(kind).value)]
    else
      break
    end
  end
  indexes=strcat(indexes,",")
  
  if cste_nb==size(tree.operands)-1 then // Inference can be done
    ierr=execstr("CONT=var.contents("+indexes+")","errcatch")
    if ierr<>0 then // Unknown index in contents
      tree.out(1).contents=list()
    else
      tree.out(1).contents=CONT
    end
  else
    tree.out(1).contents=list()
  end
end
endfunction

  