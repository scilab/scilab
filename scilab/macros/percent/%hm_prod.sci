function x=%hm_prod(m,d)
if argn(2)==1 then
  x=prod(m.entries)
  return
end
dims=m.dims;
N=size(dims,'*');
p1=prod(dims(1:d-1));//product step
p2=p1*dims(d);//step for next to prod
ind=(0:p1:p2-1)';// selection for product
deb=(1:p1);
I=ind*ones(deb)+ones(ind)*deb 

ind=(0:p2:prod(dims)-1);
I=ones(ind).*.I+ind.*.ones(I)

x=prod(matrix(m.entries(I),dims(d),-1),1)
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
