function M=%st_i_s(varargin)
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  M=createstruct(index,N)
  return
end
endfunction
