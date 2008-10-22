function [x,k]=%hm_mini(varargin)
  n=size(varargin)
  if n==1 then
    [x,k1]=min(varargin(1).entries)
    if argn(1)>1 then k=ind2sub(varargin(1).dims,k1(1)),end
  else
    if n==2 then
      d=varargin(2)
      if type(d)==1|type(d)==10 then
	[x,k]=%hm_oriented_min(varargin(1),d)
	return
      end
    end
    x=varargin(1).entries
    dims=varargin(1).dims
    for kk=2:n
      if or(dims<>varargin(kk).dims) then 
	if prod(dims)<>1&prod(varargin(kk).dims)<>1 then
	  error(42)
	end
	if prod(dims)==1 then dims=varargin(kk).dims,end
      end
      [x,k]=min(x,varargin(kk).entries)
    end
    x=hypermat(dims,x)
    k=hypermat(dims,k)
  end
endfunction

function [x,k]=%hm_oriented_min(m,d)
  if d=='m' then
    d=find(dims>1,1)
    if d==[] then 
      [x,k1]=max(m.entries)
      k=ind2sub(m.dims,k1(1))
      return,
    end
  elseif d=='r' then 
    d=1
  elseif d=='c' then
    d=2
  end
  dims=m.dims;
  if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
  N=size(dims,'*');
  p1=prod(dims(1:d-1));// step for one min
  p2=p1*dims(d);//step for beginning of vector to min 
  ind=(0:p1:p2-1)';// selection for vector to min
  deb=(1:p1);
  I=ind*ones(deb)+ones(ind)*deb 

  ind=(0:p2:prod(dims)-1);
  I=ones(ind).*.I+ind.*.ones(I)

  [x,k]=min(matrix(m.entries(I),dims(d),-1),'r')

  dims(d)=1
  if d==N then
    dims=dims(1:$)
  else
    dims(d)=1
  end
  if size(dims,'*')==2 then
    if flag==1 then dims=int32(dims);end
    x=matrix(x,dims(1),dims(2))
    k=matrix(k,dims(1),dims(2))
  else
    if flag==1 then dims=int32(dims);end
    x=hypermat(dims,x)
    k=hypermat(dims,matrix(k,-1,1))
  end
endfunction
