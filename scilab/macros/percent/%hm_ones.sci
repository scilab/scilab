function M=%hm_ones(varargin)
// Copyright INRIA
nv=size(varargin)
if nv==1 then
  M=varargin(1)
  if type(M)==1 then
    dims=M
  else
    dims=M('dims')
  end
  M=hypermat(dims,ones(prod(double(dims)),1))
else
  dims=ones(1,nv)
  for k=1:nv,dims(k)=varargin(k),end
  M=hypermat(dims,ones(prod(dims),1))
end
endfunction
