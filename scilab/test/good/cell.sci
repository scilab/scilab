// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function c=cell(varargin)

dims=0;
val=list();
if size(varargin)==1 & size(varargin(1),"*")>1 then
  for kd=1:size(varargin(1),"*"),dims(1,kd)=varargin(1)(kd),end
  for k=1:prod(dims),val(k)=[],end
elseif size(varargin)==1 & size(varargin(1),"*")==1 then
  dims=[varargin(1) varargin(1)]
  for k=1:prod(dims),val(k)=[],end
elseif size(varargin)>0 then
  for k=1:size(varargin),dims(1,k)=varargin(k),end
  for k=1:prod(dims),val(k)=[],end
end

if size(dims,'*')<2 then dims(2)=min(1,dims(1));end

c=mlist(['ce','dims','entries'],int32(matrix(dims,1,-1)),val)
endfunction
