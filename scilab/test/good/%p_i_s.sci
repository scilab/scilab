function M=%p_i_s(varargin)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //

  if rhs==3&(type(index)==10|type(index)==15) then
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  elseif rhs>4 then //more than 2 indices: insertion of a polynomial in an  matrix of numbers
    M=varargin($)
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(0,varargin(:))
  else
    //should not occurs (hard coded case)
  end
endfunction
