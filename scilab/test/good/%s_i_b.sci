function M=%s_i_b(varargin)
// Copyright INRIA
  rhs=argn(2)
  M=varargin($)
  if rhs<=4 then //type conversion
    M=bool2s(M)
    M(varargin(1:$-2))=varargin($-1)
  else //hypermatrix syntax
    if varargin($-1)<>[] then M=bool2s(M),end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
  end 
endfunction
