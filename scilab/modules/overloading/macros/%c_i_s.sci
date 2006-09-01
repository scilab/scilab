function M=%c_i_s(varargin)
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //

  if rhs==3&(type(index)==10|type(index)==15) then //Cell and struct case
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  elseif rhs>4 then //more than 2 indices: insertion of a string in an empty matrix
    if size(M,'*')<>0 then
      error('affection of a string  in a matrix of numbers is not implemented')
    end
    M=varargin($)
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm('',varargin(:))
  else //should not occur (hard coded case)
    if size(M,'*')<>0 then
      error('affection of a string  in a matrix of numbers is not implemented')
    end
    M=var
  end
endfunction

