function x=%hm_median(m,d)
if argn(2)==1 then
  x=median(m.entries)
  return
end
dims=m.dims;
N=size(dims,'*');
p1=prod(dims(1:d-1));// step to build one vector on which median is applied
p2=p1*dims(d);//step for beginning of next vectors
ind=(0:p1:p2-1)';// selection for building one vector
deb=(1:p1);
I=ind*ones(deb)+ones(ind)*deb 

ind=(0:p2:prod(dims)-1);
I=ones(ind).*.I+ind.*.ones(I)

x=median(matrix(m.entries(I),dims(d),-1),1)
dims(d)=1
if d==N then
  dims=dims(1:$)
else
  dims(d)=1
end
if size(dims,'*')==2 then 
  x=matrix(x,dims(1),dims(2))
else
  x=hypermat(dims,x)
end
endfunction
