function c=cell(varargin)
 dims=0;val=list();
 if size(varargin)>0 then
   for k=1:size(varargin),dims(1,k)=varargin(k),end
   for k=1:prod(dims),val(k)=[],end
 end
 if size(dims,'*')<2 then dims(2)=min(1,dims(1));end
 c=mlist(['ce','dims','entries'],int32(matrix(dims,1,-1)),val)
endfunction
