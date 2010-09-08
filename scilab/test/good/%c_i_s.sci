// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%c_i_s(varargin)
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  index=varargin(1) //
  if size(M,'*')<>0 then
    error(_("Affection of a string in a matrix of numbers is not implemented."))
  end
  if rhs==3 then
     if type(index)==10 then  //M.x=N or M.entries=N 
      M=struct()
      M(index)=N
      if index=="entries" then //M.entries=N
	// change struct to cell
	f=getfield(1,M);f(1)="ce"
	setfield(1,f,M)
      end
      return
    elseif type(index)==15 then
      //M(i).x=N or M(i,j,..).x=N or M.x(i,j,..)or M(i,j..)
      //check for a name in the index list
      isstr=%f; for ii=index,if type(ii)==10 then  isstr=%t,break,end,end
      if isstr then
	M=createstruct(index,N)
	if type(index(1))<>10 & index(2)=="entries" then
	  // change struct to cell
	  f=getfield(1,M);f(1)="ce"
	  setfield(1,f,M)
	end
      else
	M(index(:))=N
      end
      return
    end

  elseif rhs>4 then //more than 2 indices: insertion of a string in an empty matrix
    M=varargin($)
    M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
    varargin($)=M;
    M=generic_i_hm('',varargin(:))
  else //should not occur (hard coded case)
    M=var
  end
endfunction

