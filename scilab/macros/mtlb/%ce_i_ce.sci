function M=%ce_i_ce(varargin)
// Authors: F. Delebecque, S. Steer, V. Couvert, Copyright INRIA 
//insert the cell varargin($-1) into the cell varargin($)
//at position varargin(1), varargin(2),varargin(3),...
//insert the cell varargin($-1) as the field varargin(1)
//of the cell varargin($)

  [lhs,rhs]=argn(0)
  M=varargin($)
  N=varargin($-1)//inserted matrix
  dims=double(matrix(M.dims,1,-1));
  // Dimensions modified so that convertindex works correctly

  //computing the dimension of the result
  nd=size(dims,'*')
  
  olddims=dims
  
  reduced_index=%f
  if rhs-2>nd then  //more indices than M number of dims
    dims(nd+1:rhs-2)=1;
  elseif rhs-2<nd  then //less indices than M number of dims
    dims=[dims(1:rhs-3) prod(dims(rhs-2:$))]
    if size(find(dims>1),'*')>1 then reduced_index=%t,end
  end
  //convert N-dimensionnal indexes to 1-D and extend dims if necessary
  [Ndims,I]=convertindex(list(dims,double(matrix(N.dims,1,-1))),varargin(1:$-2));
  Ndims=matrix(Ndims,1,-1)
  if reduced_index&or(Ndims<>dims)  then error(21),end
  if or(Ndims>dims) then
    //extend the destination matrix
    I1=0
    for k=size(Ndims,'*'):-1:1
      ik1=(1:dims(k))';
      if ik1<>[] then
	if Ndims(k)>1 then
	  if size(I1,'*')>1 then
	    I1=(Ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1);
	  else
	    I1=Ndims(k)*I1+ik1-1;
	  end
	else
	  I1=Ndims(k)*I1+ik1-1;
	end
      end
    end
    // create the resulting matrix
    v2=list();for k=1:prod(Ndims),v2(k)=[];end
    
    // populate it with M entries
    w=getfield(3,M);
    if type(w)<>15 then w=list(w),end
    if type(v2)<>15 then v2=list(v2);end
    for i=1:size(I1,'*'), 
      if w<>list() then
	v2(I1(i)+1)=w(i);
      else
	v2(I1(i)+1)=[];
      end
    end
    R=mlist(['ce','dims','entries'],int32(Ndims),v2);
  else //the dimension agree
    R=M
  end
  
  //insert N entries into result  
  for k=1
    kf=1
    v2=getfield(3,R);
    w=getfield(3,N);
    if type(w)<>15 then w=list(w),end
    if type(v2)<>15 then v2=list(v2);end
    for i=1:size(I,'*'), 
      if w<>list() then
	v2(I(i))=w(i);
      else
	v2(I(i))=[];
      end
    end 
    if length(v2)==1 then v2=v2(1);end
    setfield(3,v2,R);
  end
  
  //remove trailing unitary dimensions
  if reduced_index then
    Ndims=olddims
  else
    while  Ndims($)==1 then Ndims($)=[],end
    select size(Ndims,'*')
    case 0 then
      Ndims=[1,1]
    case 1 then
      k=find(olddims<>1&olddims<>0)
      if k==[] // M was a scalar
	if mtlb_mode() then
	  Ndims=[1,Ndims]
	else
	  Ndims=[Ndims,1]
	end
      else // M was a vector
	olddims(k)=Ndims
	Ndims=olddims;
      end
    else 
      Ndims=matrix(Ndims,1,-1)
    end
  end
  R.dims=int32(Ndims)
  M=R
endfunction
