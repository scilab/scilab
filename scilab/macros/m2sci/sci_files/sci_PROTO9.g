function [tree]=sci_MFUN(tree)
// Prototype for *int**()
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_MFUN()
// V.C.

// %c_MFUN and %b_MFUN are not defined in Scilab
x = getrhs(tree)

// Matlab MFUN() can return Boolean type value
if or(x.vtype==[Boolean,Unknown]) then
  vtype=Unknown
else
  vtype=Int
end

x = convert2double(x)

tree.rhs=Rhs(x)

if is_complex(x) then
  set_infos("Scilab MFUN() does not work with Complex values: MFUN() call IGNORED !",2);
  if typeof(x)=="operation" then
    x.out=tree.lhs
  elseif typeof(x)=="funcall" then
    x.lhs=tree.lhs
  end
  tree=x
elseif ~is_real(x) then
  set_infos("Scilab MFUN() does not work with Complex values: MFUN() call IGNORED !",2);
  repl_poss(x,..
      tree,x,"is real")
  if typeof(x)=="operation" then
    x.out=tree.lhs
  elseif typeof(x)=="funcall" then
    x.lhs=tree.lhs
  end
  tree=x
else
  scitree=tree
  scitree.name="mtlb_MFUN"
  if tree.name=="int8" then
    repl_poss(scitree,..
	tree,x,"contains no %inf or -%inf values")
  elseif or(tree.name==["int16","int32"]) then
    repl_poss(scitree,..
	tree,x,"contains no %inf or -%inf or NaNs values")
  elseif or(tree.name==["uint8","uint16","uint32"]) then
    repl_poss(scitree,..
	tree,x,"contains no %inf values")
  end
  tree=scitree
  tree.lhs(1).dims=x.dims
  tree.lhs(1).type=x.type
end

endfunction
