function M=%s_i_s(varargin)
// Copyright INRIA
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //

if rhs==3&(type(index)==10|type(index)==15) then
  if type(index)<>15 then
   M=struct()
    M(index)=N
    if index=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
   else
    M=createstruct(index,N)
    if type(index(1))<>10 & index(2)=="entries" then
      // change struct to cell
      f=getfield(1,M);f(1)="ce"
      setfield(1,f,M)
    end
  end
  return
end


//X(i,j,k)=n  hypermatrix
M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
dims=double(M('dims'))
v=M('entries');v=v(:)


if rhs-2>size(dims,'*') then
  dims(size(dims,'*')+1:(rhs-2))=0
end
dims1=[]
I=0
iimp=0
for k=rhs-2:-1:1
  ik=varargin(k)
  if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
    ik=horner(ik,dims(k)) // explicit subscript
  elseif type(ik)==4 then // boolean subscript
    ik=find(ik)
  elseif mini(size(ik))<0 then // :
    if dims(k)<>0 then
      ik=1:dims(k)
    else
      iimp=iimp+1
      if iimp<=2 then
	ik=1:size(N,3-iimp)
      else
	ik=1
      end
    end
  end
  dims1(k)=max([max(ik),dims(k)])
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
if prod(dims1)>size(v,'*') then v(prod(dims1))=0,end
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
  M=mlist(['hm','dims','entries'],int32(matrix(dims1,1,-1)),v)
end
endfunction


function [index,N]=pre_trait(index,N)
//St.h....  .f(i1,i2,...in)=A <=> St.h....  .f=B
//with B(i1,...,in)=A
if type(INDX)==1 then
//  INDX<-> (i1,i2,  ,in) = i1 ,  numeric index (integer) 
 xxx(INDX)=N;N=xxx;index($)=null();
 if size(index)==1 then index=index(1);end
 return;
end

if type(INDX)==15 then
// INDX<-> (i1,i2, in) =list(i1,i2,  in) , list index (all integers)
  numeric=1
  for kk=INDX
    if type(kk)~=1 
      numeric=0;break;
    end
  end
  if numeric==1 then
  xxx(INDX(:))=N;N=xxx;index($)=null();
  if size(index)==1 then
  index=index(1);
  end
  return;
  else
  error('invalid struct indexing');
  end
end
endfunction
