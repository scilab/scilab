function m=%ce_matrix(m,varargin)
  dims=m.dims
  new=[]
  for v=varargin
    new=[new,v]
  end
  n=find(new==-1)
  if size(n,'*')==1 then
    d=prod(dims)/abs(prod(new))
    if d<>int(d) then error(60),end
    new(n)=d
  elseif n<>[] then
    error(42)
  end
  if prod(new)<>prod(dims) then error(60),end
  m.dims=new
endfunction
