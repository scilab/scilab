function x=%hm_fft(x,flag)
dims=x('dims')(:);
v=x('entries')(:);
n=size(dims,'*')
incr=1
for k=1:n
  dk=double(dims(k));
  v=fft(v ,flag,dk,incr)
  incr=incr*dk
end
x('entries')=v
endfunction
