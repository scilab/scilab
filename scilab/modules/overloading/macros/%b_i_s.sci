// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function M=%b_i_s(varargin)


  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  //if M<>[] then M=M<>0,end
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //

  if rhs==3&(type(index)==10|type(index)==15) then
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  elseif rhs>4 then //more than 2 indices: insertion of a boolean in a matrix of numbers
    M=varargin($)
    if M<>[] then varargin($-1)=bool2s(varargin($-1)),end
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm(%f,varargin(:))
  else //type conversion
    M=varargin($)
    M(varargin(1:$-2))=bool2s(varargin($-1))
  end
endfunction
