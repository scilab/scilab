function M=%hm_i_hm(varargin)
// Copyright INRIA
//insertion of a matrix in an hypermatrix
  [lhs,rhs]=argn(0)
  M=varargin(rhs)
  N=varargin(rhs-1)//inserted matrix
  dims=M.dims(:);

  v=M.entries;v=v(:)


  nd=size(dims,'*')
  if rhs-2>nd then dims(nd+1:rhs-2)=1;end  

  //convert N-dimensionnal indexes to 1-D
  [ndims,I]=convertindex(dims,varargin(1:$-2))

  if or(ndims>dims) then
    //extend the destination matrix
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
    select type(v)
      case 1
      v1=zeros(prod(ndims),1)
      case 2 then
      v1=zeros(prod(ndims),1)
      case 4 then
      v1=(zeros(prod(ndims),1)==1)
      case 8 then
      v1=iconvert(zeros(prod(ndims),1),inttype(v))
      case 10 then
      v1=emptystr(prod(ndims),1)
    end
    v1(I1+1)=v;v=v1
  end

  v(I)=N.entries(:)

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
