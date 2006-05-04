function M=st_i_generic(varargin)
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //
  if M==[] then M=struct(),M(index)=N;return;end
  if rhs==3&(type(index)==10|type(index)==15) then
    if type(index)==15 then
      M=struct()
      M(index)=N
    else
      M=createstruct(index,N)
    end
  else
    error(1)
  end

endfunction
