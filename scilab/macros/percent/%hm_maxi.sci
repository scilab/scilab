function x=%hm_maxi(varargin)
n=size(varargin)
if n==1 then
  x=max(varargin(1).entries)
else
  if n==2 then
    d=varargin(2)
    if type(d)==1|type(d)==10 then
      x=%hm_oriented_max(varargin(1),d)
      return
    end
  end
  x=varargin(1).entries
  dims=varargin(1).dims
  for k=2:n
    if or(dims<>varargin(k).dims) then 
      if prod(dims)<>1&prod(varargin(k).dims)<>1 then
	error(42)
      end
      if prod(dims)==1 then dims=varargin(k).dims,end
    end
    x=max(x,varargin(k).entries)
  end
  x=hypermat(dims,x)
end


function x=%hm_oriented_max(m,d)
if d=='r' then 
  d=1
elseif d=='c' then
  d=2
end
dims=m.dims;
N=size(dims,'*');
p1=prod(dims(1:d-1));// step for one max
p2=p1*dims(d);//step for beginning of vector to max 
ind=(0:p1:p2-1)';// selection for vector to max
deb=(1:p1);
I=ind*ones(deb)+ones(ind)*deb 

ind=(0:p2:prod(dims)-1);
I=ones(ind).*.I+ind.*.ones(I)

x=max(matrix(m.entries(I),dims(d),-1),'r')

dims(d)=1
if d==N then
  dims=dims(1:$)
else
  dims(d)=1
end
if size(dims,'*')==2 then 
  x=matrix(x,dims(1),dims(2))
else
  x=hypermat(dims,x(:))
end
