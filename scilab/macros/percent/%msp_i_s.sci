function M=%msp_i_s(varargin)
// %msp_i_s(i,j,b,a) insert matlab sparse matrix b into full matrix a
// M(i,j)=b
//!
// Copyright INRIA
[lhs,rhs]=argn(0)

M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  M=createstruct(index,N)
  return
end

i=varargin(1);
j=varargin(2);
M=varargin(4);
b=varargin(3);
if rhs==4 then
  M(i,j)=full(b)
else
  M=b;//b=j
  M(i)=full(j)
end
endfunction
