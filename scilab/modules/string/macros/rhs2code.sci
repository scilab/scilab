function C=rhs2code(rhs)
// Copyright INRIA
// Scilab Project - V. Couvert
// Translate rhs list of an instruction to macro code (called by tree2code)
// Input:
// - rhs: list of rhs arguments
// Output:
// - C: Scilab code corresponding to rhs
// V.C.

C=[]
n=size(rhs)

args=[]
if typeof(rhs)<>"list" then
  C=expression2code(rhs)
else
  for k=1:n
    args=[args expression2code(rhs(k))]
  end
  C=strcat(args,",")
end


endfunction
