function c=cell(varargin)
  dims=[] ;for k=1:size(varargin),dims(k)=varargin(k),end
  val=list() ;for k=1:prod(dims),val(k)=[],end
  if dims==[] then dims=[0,0],end
  if size(dims,'*')<2 then dims(2)=1;end
  c=mlist(['cell','dims','entries'],dims,val)
endfunction
