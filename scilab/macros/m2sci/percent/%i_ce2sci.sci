function [tree]=%i_ce2sci(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion in cells
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.

from=tree.operands($)
to=tree.operands(1)

if and(to.vtype<>[Cell,Unknown]) then
  error("%i_ce2sci: destination variable is not a cell: "+to.name+" is of type "+string(to.vtype))
elseif to.vtype==Unknown then
  insert(Equal(list(to),Funcall("cell",1,list(),list(to))))
end

// Just one index value
if rhs==1 then
  ind=tree.operands(2)
  if type(ind)<>15 then // One value index A(xx)={...}
    if typeof(ind)=="cste" then
      if ind.vtype<>String then // Not :
	tree.out(1).dims=list(1,ind.value)
      else
	tree.out(1).dims=from.dims
	execstr("tree.out(1).contents=from.contents")
      end
    else
      tree.out(1).dims=list(1,Unknown)
    end
    tree.operands(2)=list(Cste(1),ind)
    if can_infer(tree.operands(2)) then
      execstr("tree.out(1).contents"+expression2code(tree.operands(2))+".entries=1") // Remove this line when Scilab bug is corrected
      execstr("tree.out(1).contents"+expression2code(tree.operands(2))+".entries=from.contents.entries")
    end
    tree.out(1).vtype=Cell
  else // --- Insertion with more than one index value (index is a list) ---
    // Cell array of struct A{p,q,...}.name... or recursive index A{p,q,...}(1,2)...
    for kind=1:lstsize(tree.operands(2))
      if typeof(tree.operands(2)(kind))=="cste" then
	if tree.operands(2)(kind).vtype<>String then
	  tree.operands(2)(kind)=list(Cste(1),tree.operands(2)(kind))
	end
      end
    end
    
    IND=tree.operands(2)(1)
    // Update cell dims for inference
    if can_infer(IND) then
      if lstsize(IND)>lstsize(tree.out(1).dims) then
	for kd=lstsize(tree.out(1).dims):lstsize(IND)
	  tree.out(1).dims(kd)=Unknown
	end
      end
      for kd=1:lstsize(tree.out(1).dims)
	if tree.out(1).dims(kd)<>Unknown & tree.out(1).dims(kd)<IND(kd).value then
	  tree.out(1).dims(kd)=IND(kd).value
	end
      end
    end
    
    // Update cell contents
    if can_infer(tree.operands(2)) then
      infertree=tree.operands(2)
      endind=1
      if typeof(tree.operands(2)(1))=="cste" then
	if tree.operands(2)(1).vtype<>String then
	  endind=2
	end
      elseif typeof(tree.operands(2)(1))=="list" then
	endind=2
      end
      tree.out(1).contents=to.contents
      index=expression2code(infertree)
      execstr("tree.out(1).contents"+index+"=1") // Remove this line when Scilab bug corrected
      execstr("tree.out(1).contents"+index+"=from.infer")
    else
      tree.out(1).contents=cell()
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

  // Update contents...
  infertree=tree.operands
  infertree(1)=null()
  infertree($)=null()
  if can_infer(infertree) then
    index=expression2code(infertree)
    execstr("tree.out(1).contents"+index+".entries=1"); // Remove this line when Scilab bug corrected
    execstr("tree.out(1).contents"+index+".entries=from.contents.entries");
  end
end
errclear();
endfunction

  