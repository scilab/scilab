function M=%hm_e(varargin)
// Copyright INRIA
//extract an sub_hypermatrix
[lhs,rhs]=argn(0)
M=varargin(rhs)
dims=M('dims')
//v=M('entries')(:)


if rhs-1>size(dims,'*') then
  dims(rhs-1)=1
  //error('Too many subscripts')
end
dims1=[]
I=0

for k=rhs-1:-1:1
  ik=varargin(k)
  if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
    ik=round(horner(ik,dims(k))) // explicit subscript
  elseif type(ik)==4 then // boolean subscript
    ik=find(ik)
  elseif mini(size(ik))<0 then // :
    if rhs==2 then
      M=M('entries')(:)
      return
    end
    ik=1:dims(k)
    if k==rhs-1 then
      if k<size(dims,'*') then
	ik=1:prod(dims(k:$))
      end
    end
  else
    ik=round(ik)
  end
  dims1=[size(ik,'*'),dims1]
  if size(ik,'*')>1 then
    ik=ik(:)
    if size(I,'*')>1 then
      I=(dims(k)*I).*.ones(ik)+ones(I).*.(ik-1)
    else
      I=dims(k)*I+ik-1
    end
  else
    I=dims(k)*I+ik-1
  end
end
//
dims1(max(find(dims1>1))+1:$)=[]


while  dims1($)==1 then dims1($)=[],end
select size(dims1,'*')
case 0
  M=M('entries')(I+1)
case 1
  M=M('entries')(I+1)
case 2
  M=matrix(M('entries')(I+1),dims1(1),dims1(2))
else
  M=mlist(['hm','dims','entries'],dims1,M('entries')(I+1))
end
endfunction
