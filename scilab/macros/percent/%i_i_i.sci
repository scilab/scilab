function M=%i_i_i(varargin)
//insertion of an integer matrix in an matrix of integers for more than 2 indices
// Copyright INRIA
  rhs=argn(2)
  M=varargin(rhs)// destination matrix
  N=varargin(rhs-1)//inserted matrix
  
  if M<>[] then 
    //insertion of an integer matrix in a double one
     M(varargin(1:$-2))=double(N),
  else  
    M=mlist(['hm','dims','entries'],size(M),M(:))
    M(varargin(1:$-2))=N
  end
endfunction
