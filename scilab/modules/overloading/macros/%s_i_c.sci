function M=%s_i_c(varargin)
//insertion of a matrix of numbers in a matrix of of string 
// Copyright INRIA, Serge Steer
  
  rhs=argn(2)
  M=varargin($)
  if rhs<=4 then //type conversion
    error(43)
  else //hypermatrix syntax
    if varargin($-1)<>[] then error(43),end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm('',varargin(:))
  end 
endfunction

