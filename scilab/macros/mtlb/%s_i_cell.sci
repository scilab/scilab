function M=%s_i_cell(varargin)
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
  v=enlarge_shape(dims,dims1,v)
end
//insert the object N nindex times at each location given by I
for i=1:size(I,'*'), v(I(i)+1)=N,end

if to_reshape&dims1==dims then dims1=dims_save,end

// remove higher dimensions equal to 1
while  dims1($)==1 then dims1($)=[],end
if size(dims1,'*')<2 then dims1(2)=1,end

//form the resulting data structure
M=mlist(['cell','dims','entries'],dims1,v)
endfunction

function [I,dims1]=maxindex(dims,varargin)
//
// I vector of 1D indexes, 
// dims1: maximum value of each individual index
nindex=size(dims,'*')
dims1=[]
I=0;I1=0

for k=nindex:-1:1
  ik=varargin(k)//the kth subscript
  if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
    ik=horner(ik,dims(k)) // explicit subscript
    dims1(k,1)=max(max(ik),dims(k))
  elseif type(ik)==4 then // boolean subscript
    ik=find(ik)
    dims1(k,1)=max(max(ik),dims(k))
  elseif mini(size(ik))<0 then // :
    ik=1:dims(k)
    dims1(k,1)=max(max(ik),dims(k))
    if k==nindex then
      if k<nd then
	ik=1:prod(dims(k:$))
	dims1(k:nd,1)=dims(k:nd)
      end
    end
  else
    dims1(k,1)=max(max(ik),dims(k))
  end
  if size(ik,'*')>1 then
    ik=ik(:)
    if size(I,'*')>1 then
      I=(dims1(k)*I).*.ones(ik)+ones(I).*.(ik-1)
    else
      I=dims1(k)*I+ik-1
    end
  else
    I=dims1(k)*I+ik-1
  end
end
endfunction

function v1=enlarge_shape(dims,dims1,v)
//dims :  vector of old dimensions
//dims1 : vector of new dimensions
//v     : list of entries
//v1    : new list of entries
//
// undefined elements are set to []
// and(dims1>=dims) must be true
ki=find(dims1>dims)
if ki==nd then // the last dimension -> add empty entries at the end
  for k=prod(dims)+1:prod(dims1),v(k)=[];v1=v;end
else // enlarge and merge
  v1=list();for k=1:prod(dims1),v1(k)=[],end
  //merge compute the new index of M entries into enlarged cell
  I1=[];
  for k=size(dims1,'*'):-1:1
    ik1=(1:dims(k))'
    if ik1<>[] then
      if dims1(k)>1 then
	if size(I1,'*')>1 then
	  I1=(dims1(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
	else
	  I1=dims1(k)*I1+ik1-1
	end
      else
	I1=dims1(k)*I1+ik1-1
      end
    end
  end
  for iw=1:size(I1,'*'),v1(I1(iw)+1)=v(iw);end
end

endfunction
