function M=%i_i_s(varargin)
//insertion of an integer matrix in an matrix of double
// Copyright INRIA

rhs=argn(2)
index=varargin(1) 

if rhs==3&(type(index)==10|type(index)==15) then //struct or cell creation
  N=varargin(rhs-1)//inserted matrix
  M=createstruct(index,N)
  if type(index(1))<>10 & index(2)=="entries" then
    // change struct to cell
    f=getfield(1,M);f(1)="ce"
    setfield(1,f,M)
  end
elseif rhs>4 then//more than 2 indices: insertion of a boolean in a matrix of numbers
    M=varargin($)
    def=%f
    if M<>[] then varargin($-1)=double(varargin($-1)),def=0;end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(def,varargin(:))
  end
else //type conversion
  M=varargin($)
  M(varargin(1:$-2))=double(varargin($-1))
end
endfunction
