function [Newdims,I]=convertindex(dims,varargin)
//convert N-D indexes given in varargin to 1-D index.
//if indexes exceed matrix dimensions given in dims, dimensions are
//extented and new dimensions are returned in Newdims
// Copyright INRIA

// VC 27/05/2004
// if dims is a list (INSERTION) it contains:
//  1) dims: vector of dimensions of matrix where data are inserted
//  2) Ndims: vector of dimensions of matrix to insert
// else (EXTRACTION) is is a vector (vector of dimensions)

// Modified files: %ce_i_ce.sci %st_i_st.sci %b_i_hm.sci %hm_i_b.sci %hm_i_hm.sci %hm_i_i.sci %hm_i_p.sci %hm_i_s.sci %i_i_hm.sci %p_i_hm.sci

if type(dims)==15 then // insertion
  Ndims=dims(2)
  dims=dims(1)
else // extraction
  Ndims=[]
end

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
	  if isempty(Ndims) then // extraction
	    ik=[]
	  else // insertion
	    ik=1:Ndims(3-iimp)
	  end
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
