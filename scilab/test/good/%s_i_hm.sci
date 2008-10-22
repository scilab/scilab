function M=%s_i_hm(varargin)
// Copyright INRIA
//insertion of a matrix of float in an hypermatrix
  M=varargin($)
  select type(M.entries) 
  case 1 then //destination is an hypermat of double
    M=generic_i_hm(0,varargin(:))
  case 4 then //destination is an hypermat of boolean
    N=varargin($-1)
    if N<>[] then N=N<>0,end
    M=generic_i_hm(%f,varargin(1:$-2),N,M)
  case 8 then //destination is an hypermat of integer
    it=inttype(M.entries)
    z=iconvert(0,it)
    N=varargin($-1)
    if N<>[] then N=iconvert(N,it),end
    M=generic_i_hm(z,varargin(1:$-2),N,M)
  else
    M=generic_i_hm(0,varargin(:))
  end
endfunction
