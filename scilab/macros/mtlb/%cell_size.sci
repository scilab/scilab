function varargout=%cell_size(c)
sz=matrix(c.dims,1,-1)
if argn(1)==1 then 
  varargout=list(sz)
else
  varargout=list()
  for k=1:argn(1),varargout(k)=sz(k),end
end
endfunction
