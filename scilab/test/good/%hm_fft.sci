function x=%hm_fft(x,flag)
rhs=argn(2)
if rhs == 1 then
  flag = -1
end
dims=matrix(x.dims,-1,1)
v=matrix(x.entries,-1,1);
n=size(dims,'*')
incr=1
for k=1:n
  dk=double(dims(k));
  v=fft(v ,flag,dk,incr)
  incr=incr*dk
end
x.entries=v
endfunction
