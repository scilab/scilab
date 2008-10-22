function s=makecell(varargin)
// Copyright INRIA
// Equivalent for Matlab {}
// Author: V. Couvert

if lstsize(varargin)==0 then
  dims=[0 0]
  s=mlist(["ce","dims","entries"],int32(dims),list())
  return
end

dims=varargin(1)
if prod(dims)<>lstsize(varargin)-1 then
  error("Dimensions and entries do not match");
end

if prod(dims)==1 then
  s=mlist(["ce","dims","entries"],int32(dims),varargin(2))
else
  listentries=varargin
  listentries(1)=null()
  s=mlist(["ce","dims","entries"],int32(dims($:-1:1)),listentries)'
end
endfunction
