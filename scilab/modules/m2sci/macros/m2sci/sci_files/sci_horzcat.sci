function [tree]=sci_horzcat(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab horzcat()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

allargs=tree.rhs
out=tree.lhs

op="rc"
for k=1:lstsize(allargs)
  if allargs(k).vtype==String then
    op="+"
    break
  end
end

tree=Operation(op,list(allargs(1),allargs(2)),list())

// Create output
for k=3:rhs
  tree=Operation(op,list(tree,allargs(k)),list())
end
tree.out=out

// Dimensions
outdims=allargs(1).dims
for k=1:lstsize(outdims)
  outdims(k)=Unknown
end
for k=1:rhs
  for l=1:lstsize(allargs(k).dims)
    if l<>2 & allargs(k).dims(l)<>Unknown then
      outdims(l)=allargs(k).dims(l)
    end
  end
end
nbcols=0
for k=1:rhs
  if allargs(k).dims(2)<>Unknown then
    nbcols=nbcols+allargs(k).dims(2)
  else
    nbcols=Unknown
    break
  end
end
outdims(2)=nbcols

// Property
prop=Real
for k=1:rhs
  if allargs(k).property==Complex then
    prop=Complex
    break
  elseif allargs(k).property==Unknown then
    prop=Unknown
    break
  end
end

tree.out(1).dims=outdims
tree.out(1).type=Type(allargs(1).vtype,prop)
endfunction
