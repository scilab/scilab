function M=%s_i_spb(varargin)
// Copyright INRIA
[lhs,rhs]=argn(0)
M=bool2s(varargin(rhs))
N=varargin(rhs-1)//inserted matrix
if rhs<=4 then
  if rhs==3 then
    M(varargin(1))=N
  else
    M(varargin(1),varargin(2))=N
  end
  return
end
  
endfunction
