function M=%s_i_ce(varargin)
// Copyright INRIA
//insertion of a matrix in an cell M(i,j...)=N
[lhs,rhs]=argn(0)
M=varargin(rhs)//
N=varargin(rhs-1)//inserted matrix
dims=M('dims')(:);

v=M('entries');

nindex=rhs-2
nd=size(dims,'*')
to_reshape=%f

//
if nindex==1 then 
  dims_save=dims,dims=prod(dims);
  nd=1
  to_reshape=%t
elseif nindex<nd then
  for k=nindex+1:nd, varargin(k)=1;end
  nindex=nd
elseif  nindex>nd then 
  dims(nd+1:nindex)=0
end

// Compute resulting matrix dimension (max of each index)
[I,dims1]=maxindex(dims,varargin(1:nindex))

if or(dims1>dims) then // at least on dimension has increased
  v=enlarge_shape(double(dims),double(dims1),v)
end
//insert the object N nindex times at each location given by I
for i=1:size(I,'*'), v(I(i)+1)=N,end

if to_reshape&dims1==dims then dims1=dims_save,end

// remove higher dimensions equal to 1

dims12=dims1(3:$)
while  dims12($)==1 then dims12($)=[],end
dims1=[dims1;dims12]
if size(dims1,'*')<2 then dims1(2)=1,end

//form the resulting data structure
M=mlist(['ce','dims','entries'],int32(dims1),v)
endfunction


