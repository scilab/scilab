function c=cell(varargin)
// Copyright INRIA - Scilab Project

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
