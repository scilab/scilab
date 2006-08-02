function M=%infer_i_s(varargin)
// Copyright INRIA

[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  if type(index)<>15 then
   M=struct()
    M(index)=N
    if index=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
   else
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  end
  return
end
endfunction
