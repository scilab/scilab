function [dble]=convert2double(A)
// Copyright INRIA
// Scilab Project - V. Couvert
// This function converts input parameters to double to avoid to write overloading functions in Scilab

lhs=argn(1)

if A.vtype==Double then
  dble=A
  return
end

if only_double then
  dble=A
  dble.type=Type(Double,Unknown)
else
  if A.vtype==String then
    lhslist=list();
    lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Real)))
    dble=Funcall("asciimat",1,Rhs(A),lhslist)
  elseif A.vtype==Boolean then
    lhslist=list();
    lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Real)))
    dble=Funcall("bool2s",1,Rhs(A),lhslist)
  elseif A.vtype==Unknown then
    lhslist=list();
    lhslist($+1)=Variable("ans",Infer(A.dims,Type(Double,Unknown)))
    dble=Funcall("mtlb_double",1,Rhs(A),lhslist)
  else
    error("convert2double(): "+string(A.vtype)+" is not implemented !")
  end
end
endfunction
