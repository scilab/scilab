function [Newdims,I]=convertindex(dims,varargin)
//convert N-D indexes given in varargin to 1-D index.
//if indexes exceed matrix dimensions given in dims, dimensions are
//extented and new dimensions are returned in Newdims
// Copyright INRIA
  Newdims=[]
  I=0
  iimp=0
  for k=size(varargin):-1:1
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
    Newdims(k)=max([max(ik),dims(k)])
    if size(ik,'*')>1 then
      ik=ik(:)
      if size(I,'*')>1 then
	I=(Newdims(k)*I).*.ones(ik)+ones(I).*.(ik-1);
      else
	I=Newdims(k)*I+ik-1;
      end
    else
      I=Newdims(k)*I+ik-1;
    end
  end
I=I+1
endfunction
