function tree=%rc_cell2sci(tree)
// Copyright INRIA

// Make a CELL 'row' with many columns

if typeof(tree.operands(1))=="funcall" then
  if tree.operands(1).name=="cell" then
    tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),cell())
    tree.out(1).infer.contents(1,1).entries=tree.operands(2).infer
    return
  end  
end

if tree.operands(1).vtype==Cell then
  tree.out(1).infer=tree.operands(1).infer
else
  tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),cell())
  tree.out(1).infer.contents(1,1).entries=tree.operands(1).infer
end
dim1=double(tree.operands(1).infer.contents.dims(1))
dim2=double(tree.operands(1).infer.contents.dims(2))
tree.out(1).infer.contents(dim1,dim2+1).entries=tree.operands(2).infer

if tree.out(1).dims(2)<>Unknown then
  tree.out(1).dims(2)=tree.out(1).dims(2)+1
end
if tree.out(1).dims(1)==0 then
  tree.out(1).dims(1)=tree.out(1).dims(1)+1
end
endfunction
