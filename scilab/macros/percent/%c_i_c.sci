function M=%c_i_c(varargin)
//insertion of an integer matrix in an matrix of integers for more than 2 indices
// Copyright INRIA
  rhs=argn(2)

  if rhs>4 then //output an hypermatrix
    M=varargin(rhs)
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin(rhs)=M;
    M=generic_i_hm('',varargin(:))

  else
    M=varargin(rhs)// destination matrix
    N=varargin(rhs-1)//inserted matrix

    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    M(varargin(1:$-2))=N
  end
endfunction
