function c=struct(varargin)
 dims=0;val=list();
 if size(varargin)>0 then
   for k=1:size(varargin),dims(k)=varargin(k),end
   for k=1:prod(dims),val(k)=[],end
 end
 if size(dims,'*')<2 then dims(2)=min(1,dims(1));end
 c=mlist(['st','dims'],int32(dims))
endfunction
