function M=%ce_i_ce(varargin)
// Copyright INRIA
//insertion 
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
dims=M('dims')(:);

v=M('entries');

Ndims=rhs-2
nd=size(dims,'*')
if Ndims>nd then dims(nd+1:Ndims)=0;end  

dims1=[]
I=0;I1=0
for k=Ndims:-1:1
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
    if k==Ndims then
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
if or(dims1>dims) then
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
  v1=list();for iw=1:prod(dims1),v1(iw)=[],end
  for iw=1:size(I1,'*')
    v1(I1(iw)+1)=v(iw);
  end
  v=v1
end
v(I+1)=N


dims12=dims1(3:$)
while  dims12($)==1 then dims12($)=[],end
dims1=[dims1;dims12]
if size(dims1,'*')<2 then dims1(2)=1,end

M=mlist(['ce','dims','entries'],int32(dims1),v)

endfunction
