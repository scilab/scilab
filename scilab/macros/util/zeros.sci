function z=zeros(varargin)
// Copyright INRIA
nv=size(varargin)
if nv==0 then
  z=0
elseif nv==1 then
  n=varargin(1)
  z=0*ones(n)
elseif nv==2 then
  n=varargin(1);m=varargin(2)
  z=0*ones(n,m)
else
  dims=ones(1,nv)
  for k=1:nv,dims(k)=varargin(k),end
  z=hypermat(dims,zeros(prod(dims),1))
end
endfunction
