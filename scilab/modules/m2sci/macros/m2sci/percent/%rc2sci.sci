function [tree]=%rc2sci(tree)
// Copyright INRIA

// Make a 'row' with many columns

if tree.operands(1).vtype==Cell then
  tree=%rc_cell2sci(tree)
  return
end

colnb=size(tree.operands)
row=list()
rowsize=[]
colsize=[]
typ=Double

realcols=0
complexcols=0

for k=1:colnb
  rk=tree.operands(k)
  row(k)=rk
  if rk.vtype==String then
    typ=String
  end
  rowsize=[rowsize tree.operands(k).dims(1)]
  colsize=[colsize tree.operands(k).dims(2)]
  if rk.property==Complex then
    complexcols=complexcols+1
  end
  if rk.property==Real then
    realcols=realcols+1
  end
end

if realcols==colnb then
  prop=Real
elseif complexcols<>0 then
  prop=Complex
else
  prop=Unknown
end

undef=find(rowsize==-1)
void=find(rowsize==0)
rowsize([undef void])=[]
if rowsize==[] then
  if undef<>[] then
    sr=-1
  else
    sr=0
  end
else
  [w,k]=min(length(rowsize))
  sr=rowsize(k)
end

if typ<>String then
  if find(colsize==-1)==[] then
    w=colsize(1)
    for k=2:colnb
      w=w+colsize(k)
    end
    sc=w
  else
    sc=-1
  end

  tree.out(1).dims=list(sr,sc)
  tree.out(1).type=Type(rk.vtype,prop)
else
  if find(colsize==-1)==[] then
    w=colsize(1)
    for k=2:colnb
      w=w+colsize(k)
    end
    sc=w
  else
    sc=-1
  end

  tree.out(1).dims=list(sr,sc)
  tree.out(1).type=Type(String,Real)
  tree=Operation("+",tree.operands,tree.out)
end
tree.out(1).contents=Contents()
endfunction


