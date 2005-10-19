function M=%i_i_hm(varargin)
// Copyright INRIA
//insertion of an integer matrix in an hypermatrix
  N=varargin($-1)
  M=generic_i_hm(iconvert(0,inttype(N(1))),varargin(:))
endfunction
