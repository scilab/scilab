function M=%p_i_hm(varargin)
// Copyright INRIA
//insertion of a matrix in an hypermatrix
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
dims=M('dims')
v=M('entries');v=v(:)
if type(v)==4 then
  z=%f
  N=N<>0
else
  z=0
end
ndims=rhs-2
if ndims>size(dims,'*') then
  dims(size(dims,'*')+1:ndims)=0
elseif ndims<size(dims,'*') then
  for k=1:size(dims,'*')-ndims
    varargin(ndims+1)=1
    ndims=ndims+1
  end
end  
del=N==[];count=[]
dims1=[]
I=0;I1=0
for k=ndims:-1:1
  ik=varargin(k)//the kth subscript
  if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
    ik=horner(ik,dims(k)) // explicit subscript
  elseif type(ik)==4 then // boolean subscript
    ik=find(ik)
  elseif mini(size(ik))<0 then // :
    ik=1:dims(k)
  end
  dims1(k)=max(max(ik),dims(k))
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
  if del then
    if or(ik<>(1:dims1(k))') then
      count=[count k]
      nk=size(ik,'*')
    end
  end
end
if ~del&or(dims1>dims) then
  I1=0
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
  v1=zeros(prod(dims1),1)
  v1(I1+1)=v;v=v1
end
//if prod(dims1)>size(v,'*') then v(prod(dims1))=z,end
v(I+1)=N(:)
if del then
  if size(count,'*')>1 then
    error('A null assignment can have only one non-colon index')
  end
  dims1(count)=dims1(count)-nk
end


while  dims1($)==1 then dims1($)=[],end
select size(dims1,'*')
case 0
  M=v
case 1
  M=v
case 2
  M=matrix(v,dims1(1),dims1(2))
else
  M=mlist(['hm','dims','entries'],dims1,v)
end




