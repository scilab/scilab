function M=%i_i_s(varargin)
//insertion of an integer matrix in an matrix of double
// Copyright INRIA

[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  M=createstruct(index,N)
  return
end


if M<>[] then N=double(N),end
M=mlist(['hm','dims','entries'],size(M),M(:))
dims=M('dims')
v=M('entries');v=v(:)


if rhs-2>size(dims,'*') then
  dims(size(dims,'*')+1:(rhs-2))=0
end
dims1=[]
I=0
for k=rhs-2:-1:1
  ik=varargin(k)
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
end
//
if prod(dims1)>size(v,'*') then v(prod(dims1))=iconvert(0,inttype(N)),end
v(I+1)=N(:)


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






//  rhs=argn(2)
//  M=varargin(rhs)// destination matrix
//  N=varargin(rhs-1)//inserted matrix
if 0 
  if M<>[] then 
    //insertion of an integer matrix in a double one
     M(varargin(1:$-2))=double(N),
  else  
    M=mlist(['hm','dims','entries'],size(M),M(:))
    M(varargin(1:$-2))=N
  end
  end
endfunction
