function M=%b_i_hm(varargin)
// Copyright INRIA
//insertion of an boolean matrix in an hypermatrix
  rhs=argn(2)
  M=varargin(rhs)// destination matrix
  N=varargin(rhs-1)//inserted matrix
  
  dims=matrix(M.dims,-1,1)
  v=matrix(M.entries,-1,1)

  nd=size(dims,'*')
  if rhs-2>nd then dims(nd+1:rhs-2)=1;end  

  //convert N-dimensionnal indexes to 1-D
  [Ndims,I]=convertindex(list(dims,double(matrix(N.dims,1,-1))),varargin(1:$-2))

  //extend resulting matrix if necessary
  if or(Ndims>dims) then
    I1=0
    for k=size(Ndims,'*'):-1:1
      ik1=(1:dims(k))'
      if ik1<>[] then
	if Ndims(k)>1 then
	  if size(I1,'*')>1 then
	    I1=(Ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
	  else
	    I1=Ndims(k)*I1+ik1-1
	  end
	else
	  I1=Ndims(k)*I1+ik1-1
	end
      end
    end
    v1=%f(ones(prod(Ndims),1))
    v1(I1+1)=v;v=v1
  end

  //insert the elements
  v(I)=matrix(N,-1,1)

  //reduce the dimensionality if possible
  while  Ndims($)==1 then Ndims($)=[],end

  select size(Ndims,'*')
    case 0
    M=v
    case 1
    M=v
    case 2
    M=matrix(v,Ndims(1),Ndims(2))
  else
    M=mlist(['hm','dims','entries'],matrix(Ndims,1,-1),v)
  end
endfunction
