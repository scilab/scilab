function M=%s_i_i(varargin)
  rhs=argn(2)
  M=varargin($)
  is=inttype(M)
  if rhs<=4 then //type conversion
    M(varargin(1:$-2))=iconvert(varargin($-1),is)
  else //hypermatrix syntax
    if varargin($-1)<>[] then varargin($-1)=iconvert(varargin($-1),is),end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
  end 
endfunction
