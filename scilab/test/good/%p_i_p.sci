function M=%p_i_p(varargin)
//insertion of an polynomial matrix in an matrix of polynomilas for more than 2 indices
// Copyright INRIA, Serge Steer
  M=varargin($)
  M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
  varargin($)=M;
  M=generic_i_hm(0,varargin(:))
endfunction
