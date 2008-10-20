function M=%hm_zeros(varargin)
// Copyright INRIA
//creates a zero hypermatrix with shape given by vector of dimensions or an
//hypermatrix
nv=size(varargin)
if nv==1 then
  M=varargin(1)
  if type(M)==1 then
    dims=M
  else
    dims=M('dims')
  end
  M=hypermat(dims,zeros(prod(double(dims)),1))
else
  dims=ones(1,nv)
  for k=1:nv,dims(k)=varargin(k),end
  M=hypermat(dims,zeros(prod(dims),1))
end
endfunction
