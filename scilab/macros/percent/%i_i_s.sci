function M=%i_i_s(varargin)
//insertion of an integer matrix in an matrix of double
// Copyright INRIA

[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  M=createstruct(index,N)
  return
end

//  rhs=argn(2)
//  M=varargin(rhs)// destination matrix
//  N=varargin(rhs-1)//inserted matrix
  
  if M<>[] then 
    //insertion of an integer matrix in a double one
     M(varargin(1:$-2))=double(N),
  else  
    M=mlist(['hm','dims','entries'],size(M),M(:))
    M(varargin(1:$-2))=N
  end
endfunction
