function c=cell(varargin)
  dims=[] ;for k=1:size(varargin),dims(k)=varargin(k),end
  val=list() ;for k=1:prod(dims),val(k)=[],end
  c=mlist(['cell','dims','entries'],dims,val)
endfunction
