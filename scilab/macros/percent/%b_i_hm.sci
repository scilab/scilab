function M=%b_i_hm(varargin)
// Copyright INRIA
//insertion of an boolean matrix in an hypermatrix
  rhs=argn(2)
  M=varargin(rhs)// destination matrix
  N=varargin(rhs-1)//inserted matrix
  
  dims=M('dims')(:);
  v=M('entries');v=v(:)

  nd=size(dims,'*')
  if rhs-2>nd then dims(nd+1:rhs-2)=1;end  

  //convert N-dimensionnal indexes to 1-D
  [ndims,I]=convertindex(dims,varargin(1:$-2))

  //extend resulting matrix if necessary
  if or(ndims>dims) then
    I1=0
    for k=size(ndims,'*'):-1:1
      ik1=(1:dims(k))'
      if ik1<>[] then
	if ndims(k)>1 then
	  if size(I1,'*')>1 then
	    I1=(ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
	  else
	    I1=ndims(k)*I1+ik1-1
	  end
	else
	  I1=ndims(k)*I1+ik1-1
	end
      end
    end
    v1=%f(ones(prod(ndims),1))
    v1(I1+1)=v;v=v1
  end

  //insert the elements
  v(I)=N(:)

  //reduce the dimensionality if possible
  while  ndims($)==1 then ndims($)=[],end

  select size(ndims,'*')
    case 0
    M=v
    case 1
    M=v
    case 2
    M=matrix(v,ndims(1),ndims(2))
  else
    M=mlist(['hm','dims','entries'],matrix(ndims,1,-1),v)
  end
endfunction
