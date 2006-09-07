function [I,dims1]=maxindex(dims,varargin)
//
// I vector of 1D indexes, 
// dims1: maximum value of each individual index
nindex=size(dims,'*')
dims1=[]
I=0;I1=0

for k=nindex:-1:1
  // evaluate kth index to numerical vector
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
  // form 1D index
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
