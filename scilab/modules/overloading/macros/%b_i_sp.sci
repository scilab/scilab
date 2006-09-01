function M=%b_i_sp(varargin)
// Copyright INRIA
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=bool2s(varargin(rhs-1))//inserted matrix
if rhs<=4 then
  if rhs==3 then
    M(varargin(1))=N
  else
    M(varargin(1),varargin(2))=N
  end
else
  error('multidimensional sparse matrices are not handled')
end
 
endfunction
