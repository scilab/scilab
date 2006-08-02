function [tree]=sci_MFUN(tree)
// Prototype for eye false ones rand randn true zeros 
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()
// V.C.

// Used for false and true
name_sav=tree.name

if rhs==-1 then
  if tree.name=="ones"|tree.name=="eye" then
    tree=Cste(":")
  elseif tree.name=="zeros" then
    tree=Cste(0)
  elseif tree.name=="rand" then
    // Nothing changed
  elseif tree.name=="randn" then
    tree.rhs=Rhs(1,1,"normal")
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)
  elseif tree.name=="false" then
    tree=Cste(%F)
  elseif tree.name=="true" then
    tree=Cste(%T)
  end
  return
end

if rhs==0 then
  if tree.name=="ones"|tree.name=="eye" then
    tree=Cste(1)
  elseif tree.name=="zeros" then
    tree=Cste(0)
  elseif tree.name=="rand" then
    // Nothing changed
  elseif tree.name=="randn" then
    tree.rhs=Rhs(1,1,"normal")
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)
  elseif tree.name=="false" then
    tree=Cste(%F)
  elseif tree.name=="true" then
    tree=Cste(%T)
  end
  return
end

// Special cases for rand and randn
if tree.name=="randn" |tree.name=="rand" then
  if tree.rhs(1).vtype==String then // State
    if rhs==1 then // Get the state
      if tree.name=="rand" then
	onescall=Funcall("ones",1,Rhs(35,1),list())
	randcall=Funcall("rand",1,Rhs("seed"),list())
	tree=Operation("*",list(onescall,randcall),tree.lhs)
	tree.out(1).dims=list(35,1)
	tree.out(1).type=Type(Double,Real)
      else
	onescall=Funcall("ones",1,Rhs(2,1),list())
	randcall=Funcall("rand",1,Rhs("seed"),list())
	tree=Operation("*",list(onescall,randcall),tree.lhs)
	tree.out(1).dims=list(2,1)
	tree.out(1).type=Type(Double,Real)
      end
    else // Set the state
      tree=Funcall("rand",1,Rhs("seed",tree.rhs(2)),tree.lhs)
      tree.lhs(1).dims=list(0,0)
      tree.lhs(1).type=Type(Unknown,Unknown)
    end
    return
  end
end

opt=list(),if tree.name=="randn" then opt=Cste("normal"),end
  
// MFUN(n)
if rhs==1 then
  n = getrhs(tree)
  n=convert2double(n)
  if is_complex(n) then
    n=Funcall("real",1,list(n),list(Variable("",n.infer)))
  elseif ~is_real(n) then
    newn=Funcall("real",1,list(n),list(Variable("",n.infer)))
    repl_poss(newn,n,n,"is Real");
    n=newn
  end
  if is_a_scalar(n) then 
    if typeof(n)=="cste" then
      dim=n.value
    else
      dim=Unknown
    end
    
    tree=Funcall("SFUN",1,Rhs(n,n,opt),tree.lhs)
    tree.lhs(1).dims=list(dim,dim)
    tree.lhs(1).type=Type(Double,Real)
  // MFUN([n1,n2,...])
  elseif not_a_scalar(n)
    tmp=n
    if typeof(n)<>"variable" then
      tmp=gettempvar()
      insert(Equal(list(tmp),n))
    end
    newrhs=list()
    for k=1:size(n.dims)
      newrhs(k)=Operation("ext",list(tmp,Cste(k)),list())
    end
    if opt<>list() then
      newrhs($+1)=opt
    end
    tree=Funcall("SFUN",1,newrhs,tree.lhs)

    // All dimensions are unknown because we can not compute them here...
    tree.lhs(1).dims=list()
    if opt<>list() then
      for k=1:size(tree.rhs)-1
	tree.lhs(1).dims(k)=Unknown
      end
    else
      for k=1:size(tree.rhs)
	tree.lhs(1).dims(k)=Unknown
      end
    end
    tree.lhs(1).type=Type(Double,Real)
  // MFUN(n) or MFUN([n1,n2,...]) ?
  else 
    tree.name="mtlb_MFUN"

    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(1).type=Type(Double,Real)
  end
  
// MFUN(n1,n2,..)
else
  for k=1:size(tree.rhs)
    tree.rhs(k)=convert2double(tree.rhs(k))
    if is_complex(tree.rhs(k)) then
      tree.rhs(k)=Funcall("real",1,list(tree.rhs(k)),list()) 
    elseif ~is_real(tree.rhs(k)) then
      newn=Funcall("real",1,list(tree.rhs(k)),list())
      repl_poss(newn,tree.rhs(k),tree.rhs(k),"is Real");
      tree.rhs(k)=newn
    end
  end
  if opt<>list() then
    tree.rhs($+1)=opt
  end
  tree.name="SFUN"
  // If one input is a Cste, we can infer corresponding dimension...
  tree.lhs(1).dims=list()
  for k=1:size(tree.rhs)
    if typeof(tree.rhs(k))=="cste" then
      if tree.rhs(k).value<>"normal" then
	tree.lhs(1).dims($+1)=tree.rhs(k).value
      end
    else
      tree.lhs(1).dims($+1)=Unknown
    end
  end
  tree.lhs(1).type=Type(Double,Real)
end

if or(name_sav==["false","true"]) then
  if name_sav=="false" then
    tree.name="zeros"
  else
    tree.name="ones"
  end
  tree.lhs(1).type=Type(Boolean,Real)
  tree=Operation("==",list(tree,Cste(1)),tree.lhs)
end
endfunction






