function [tree]=sci_struct(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab struct()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

tree.lhs(1).type=Type(Struct,Unknown)

// Dims...
dims=tree.rhs(2).dims
for k=4:2:rhs
  for l=1:lstsize(dims)
    if tree.rhs(k).dims(l)==Unknown then
      dims(l)=Unknown
    else
      if dims(l)<>Unknown then
	if dims(l)<tree.rhs(k).dims(l) then
	  dims(l)=tree.rhs(k).dims(l)
	end
      end
    end
      
  end
end

tree.lhs(1).dims=dims

tree.lhs(1).contents=struct()

// Update contents
dims=[0 0]
allnotcells=%T
for k=1:2:rhs
  allnotcells=allnotcells & tree.rhs(k+1).vtype<>Cell
end
if allnotcells then
  dims=[1 1]
  tree.lhs(1).dims=list(1,1)
else
  for k=1:2:rhs
    if tree.rhs(k+1).vtype==Cell then
      if double(tree.rhs(k+1).infer.contents.dims(1))>dims(1) then
	dims(1)=double(tree.rhs(k+1).infer.contents.dims(1))
      end
      if double(tree.rhs(k+1).infer.contents.dims(2))>dims(2) then
	dims(2)=double(tree.rhs(k+1).infer.contents.dims(2))
      end
    end
  end
end

for k=1:2:rhs
  dim1=dims(1)
  dim2=dims(2)
  for kd1=1:dims(1)
    for kd2=1:dims(2)
      if tree.rhs(k+1).vtype==Cell then
	if prod(double(tree.rhs(k+1).infer.contents.dims))<>1 then
	  tree.lhs(1).contents(kd1,kd2)(tree.rhs(k).value)=tree.rhs(k+1).infer.contents(kd1,kd2).entries
	else
	  tree.lhs(1).contents(kd1,kd2)(tree.rhs(k).value)=tree.rhs(k+1).infer.contents.entries
	end
      else
	tree.lhs(1).contents(tree.rhs(k).value)=tree.rhs(k+1).infer
      end
    end
  end
end
endfunction
