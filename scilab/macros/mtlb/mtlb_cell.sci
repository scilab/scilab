function c=mtlb_cell(varargin)
// Copyright INRIA
 dims=0;val=list();
 if size(varargin)>0 then
   for k=1:size(varargin),dims(k)=varargin(k),end
   for k=1:prod(dims),val(k)=[],end
 end
 if size(dims,'*')<2 then dims(2)=min(1,dims(1));end
 c=mlist(['ce','dims','entries'],int32(dims),val)
endfunction
