function [tree]=sci_isa(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab isa()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

[OBJ,class] = getrhs(tree)

if typeof(class)=="cste" then
  
  typetree=Funcall("type",1,Rhs(OBJ),list())
  typeoftree=Funcall("typeof",1,Rhs(OBJ),list())
  
  select class.value
  case "logical"
    tree=Operation("==",list(typetree,Cste(4)),tree.lhs)
  case "char"
    tree=Operation("==",list(typetree,Cste(10)),tree.lhs)
  case "numeric"
    catop=Operation("rc",list(Cste(1),Cste(5)),list())
    catop=Operation("rc",list(catop,Cste(8)),list())
    eqtree=Operation("==",list(typetree,catop),list())
    tree=Funcall("or",1,list(eqtree),list(tree.lhs(1)))
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Boolean,Real)
    return
  case "int8"
    tree=Operation("==",list(typeoftree,Cste("int8")),tree.lhs)
  case "uint8"
    tree=Operation("==",list(typeoftree,Cste("uint8")),tree.lhs)
  case "int16"
    tree=Operation("==",list(typeoftree,Cste("int16")),tree.lhs)
  case "uint16"
    tree=Operation("==",list(typeoftree,Cste("uint16")),tree.lhs)
  case "int32"
    tree=Operation("==",list(typeoftree,Cste("int32")),tree.lhs)
  case "uint32"
    tree=Operation("==",list(typeoftree,Cste("uint32")),tree.lhs)
  case "single"
    tree=Operation("==",list(typetree,Cste(1)),tree.lhs)
  case "double"
    tree=Operation("==",list(typetree,Cste(1)),tree.lhs)
  case "cell"
    tree=Operation("==",list(typeoftree,Cste("ce")),tree.lhs)
  case "struct"
    tree=Operation("==",list(typeoftree,Cste("st")),tree.lhs)
  case "function_handle"
    tree=Operation("==",list(typetree,Cste(13)),tree.lhs)
  case "sparse"
    tree=Operation("==",list(typetree,Cste(5)),tree.lhs)
  case "lti"
    tree=Operation("==",list(typeoftree,Cste("state-space")),tree.lhs)
  else
    set_infos("Unhandled class: "+class.value,2)
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Boolean,Real)
    return
  end
  tree.out(1).dims=list(1,1)
  tree.out(1).type=Type(Boolean,Real)
else
  tree.name="mtlb_isa"
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Boolean,Real)
end
endfunction
