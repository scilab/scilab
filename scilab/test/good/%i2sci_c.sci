function [tree]=%i2sci_c(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab insertion in column vectors (called by %i2sci())
// Input: tree = Matlab operation tree
// Output: tree = Scilab equivalent for tree
// V.C.
from=tree.operands($)
to=tree.operands(1)
ind=tree.operands(2)

if from.dims(1)==1 & from.dims(2)==1 then // Insert a scalar
elseif from.dims(2)==1 then // Insert a row vector
elseif from.dims(1)<>Unknown & from.dims(2)<>Unknown then // Insert a matrix with known sizes
  tree.operands($)=Funcall("matrix",1,Rhs(from,1,Operation("-",list(Cste(1)),list())))
else
  if ~isdefinedvar(from) then
    w=gettempvar()
    insert(Equal(list(w),from))
  else
    w=from
  end
  
  // from=from(:)'
  tmp=Operation("ext",list(w,Cste(":")),list())
  tmp=Operation("''",list(tmp),list())
  
  tree.operands(4)=tmp
  tree.operands(3)=Cste(1)
  tree.operands(2)=ind
end

// Data inference
tree.out(1).dims=list(Unknown,1)
tree.out(1).type=to.type
endfunction

  
 