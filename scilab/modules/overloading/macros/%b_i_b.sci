// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function M=%b_i_b(varargin)

  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix

  dims=size(M)
  v=M(:);


  if rhs-2>size(dims,'*') then
    dims(size(dims,'*')+1:(rhs-2))=0
  end
  dims1=[]
  I=0
  for k=rhs-2:-1:1
    ik=varargin(k)
    if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
      ik=horner(ik,dims(k)) // explicit subscript
    elseif type(ik)==4 then // boolean subscript
      ik=find(ik)
    elseif mini(size(ik))<0 then // :
      ik=1:dims(k)
    end
    dims1(k)=max(max(ik),dims(k))
    if size(ik,'*')>1 then
      ik=ik(:)
      if size(I,'*')>1 then
	I=(dims1(k)*I).*.ones(ik)+ones(I).*.(ik-1)
      else
	I=dims1(k)*I+ik-1
      end
    else
      I=dims1(k)*I+ik-1
    end
  end
  //
  if prod(dims1)>size(v,'*') then v(prod(dims1))=%f,end
  v(I+1)=N(:)


  while  dims1($)==1 then dims1($)=[],end
  select size(dims1,'*')
  case 0
    M=v
  case 1
    M=v
  case 2
    M=matrix(v,dims1(1),dims1(2))
  else
    M=mlist(['hm','dims','entries'],int32(dims1),v)
  end
endfunction
