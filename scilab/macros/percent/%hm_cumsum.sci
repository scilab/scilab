function x=%hm_cumsum(m,d)
if argn(2)==1 then
  x=hypermat(m.dims,cumsum(m.entries))
  return
end
dims=m.dims;
N=size(dims,'*');
p1=prod(dims(1:d-1));//summation step
p2=p1*dims(d);//step for next to sum
ind=(0:p1:p2-1);// selection for summation
deb=(1:p1)';
I=deb*ones(ind)+ones(deb)*ind


ind=(0:p2:prod(dims)-1)';
I=ones(ind).*.I+ind.*.ones(I)
x=cumsum(matrix(m.entries(I),-1,dims(d)),2)
x(I)=x(:)
x=hypermat(dims,x)

