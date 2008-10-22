function M=%c_i_c(varargin)
//insertion of an string matrix in an matrix of string for more than 2 indices
// Copyright INRIA, Serge Steer
  M=varargin($)
  M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
  varargin($)=M;
  M=generic_i_hm('',varargin(:))
endfunction
