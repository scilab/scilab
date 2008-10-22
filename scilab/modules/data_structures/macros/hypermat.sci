// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function M=hypermat(dims,v)
//initialize an hypermatrix whose dimensions are given in the vector dims
// all entries are set to 0
//
// M data structure contains the vector of matrix dimensions M('dims')
// and the vector of entries M('entries') such as the leftmost subcripts vary first
// [M(1,1,..);..;M(n1,1,..);...;M(1,n2,..);..;M(n1,n2,..);...]
  [lhs,rhs]=argn(0)
  dims=double(dims)
  if dims==[] then dims=[0 0],end
  if size(dims,'*')==1 then dims=[1 dims],end

  if argn(2)<2 then v=zeros(prod(dims),1),end
  if size(v,'*')<> double(prod(dims)) then
    
    error(msprintf(gettext('%s: Number of entries does not match product of dimensions'),'hypermat'));
  end
  if size(dims,'*')<=2 then
    M=matrix(v,dims)
  else
    M=mlist(['hm','dims','entries'],int32(matrix(dims,1,-1)),matrix(v,-1,1))
  end
endfunction
