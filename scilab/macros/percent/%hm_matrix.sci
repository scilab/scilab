function a=%hm_matrix(a,varargin)
  dims=size(a);nd=size(dims,'*')
  if size(varargin)==1 then
    sz=varargin(1)
  else
    sz=[]
    for k=1:size(varargin)
      if size(varargin(k),'*')<>1|type(varargin(k))<>1 then
	error('Size arguments must be integer scalars')
      end
      sz=[sz varargin(k)]
    end
    sz=round(sz)
  end
  nd1=size(sz,'*')
  if nd1<=0 then error(60),end
  if nd1<=2 then a=matrix(a.entries,sz),return,end
  if nd<nd1 then 
    dims(nd+1:nd1)=1
  elseif nd>nd1 then 
    dims=[dims(1:nd1-1) prod(dims(nd1:nd))]
  end
  nd=size(dims,'*')
  k=find(sz==-1)
  P=prod(dims)
  if k<>[] then
    if size(k,'*')>1 then 
      error('Only one -1 value admitted')
    end
    sz(k)=floor(P/(prod(sz(sz>0))))
  end
  
  if prod(P)<>prod(sz) then
    error('MATRIX: input and output matrices  must have the same number"+...
	  " of elements')
  end
  
  if type(a)<>17 then 
    a=hypermat(sz,a)
    return
  end

  a.dims=int32(matrix(sz,1,-1));

endfunction
