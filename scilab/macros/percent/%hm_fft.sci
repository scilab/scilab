function x=%hm_fft(x,flag)
dims=x('dims')(:);
v=x('entries')(:);
n=size(dims,'*')
incr=1
for k=1:n
  v=fft(v ,flag,dims(k),incr)
  incr=incr*dims(k)
end
x('entries')=v

