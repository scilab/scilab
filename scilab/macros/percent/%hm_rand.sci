function M=%hm_rand(varargin)
// Copyright INRIA
//creates a random hypermatrix with shape given by vector of dimensions or an
//hypermatrix
nv=size(varargin)
if nv==1 then
M=varargin(1)
  if type(M)==1 then
    dims=M
  else
    dims=M('dims')
  end
  M=hypermat(dims,rand(prod(double(dims)),1))
else
  dims=ones(1,nv)
  if type(varargin($))==10 then 
    opt=varargin($),
    for k=1:nv-1,dims(k)=varargin(k),end
    M=hypermat(dims,rand(prod(dims),1,opt))
  else 
    for k=1:nv,dims(k)=varargin(k),end
    M=hypermat(dims,rand(prod(dims),1))
  end
end
endfunction
