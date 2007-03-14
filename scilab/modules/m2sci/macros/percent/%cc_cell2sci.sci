function tree=%cc_cell2sci(tree)
// Copyright INRIA

// Make a CELL 'column' with many rows

if typeof(tree.operands(1))=="funcall" then
  if tree.operands(1).name=="cell" then
    tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),Contents())
    tree.out(1).contents.index($+1)=list(list(Cste(1),Cste(1)),Cste("entries"))
    tree.out(1).contents.data($+1)=tree.operands(2).infer
    return
  end
end

if tree.operands(1).vtype==Cell then
  tree.out(1).infer=tree.operands(1).infer
else
  tree.out(1).infer=Infer(list(1,1),Type(Cell,Unknown),Contents())
  tree.out(1).contents.index($+1)=list(list(Cste(1),Cste(1)),Cste("entries"))
  tree.out(1).contents.data($+1)=tree.operands(1).infer
end
dim1=tree.operands(1).dims(1)
dim2=tree.operands(1).dims(2)

INFER=list()
if dim2==1 then
  INFER(1)=tree.operands(2).infer
else
  // Second operand is a rc or a +
  op=tree.operands(2)
  for kd2=1:dim2-1
    for ki=lstsize(INFER):-1:1
      INFER(ki+1)=INFER(ki)
    end
    INFER(1)=op.operands(2).infer
    op=op.operands(1)
  end
  for ki=lstsize(INFER):-1:1
    INFER(ki+1)=INFER(ki)
  end
  INFER(1)=op.infer
end
for kd2=1:dim2
  tree.out(1).contents.index($+1)=list(list(Cste(dim1+1),Cste(kd2)),Cste("entries"))
  tree.out(1).contents.data($+1)=INFER(kd2)
  tree.out(1).contents.index($+1)=list(Cste(dim1+1),Cste(kd2))
  tree.out(1).contents.data($+1)=Infer(list(1,1),Type(Cell,Unknown),..
      Contents(list(list(list(Cste(1),Cste(1)),Cste("entries"))),list(tree.out(1).contents.data($))))
end


if tree.out(1).dims(1)<>Unknown then
  tree.out(1).dims(1)=tree.out(1).dims(1)+1
end
if tree.out(1).dims(2)==0 then
  tree.out(1).dims(2)=tree.out(1).dims(2)+1
end
endfunction
