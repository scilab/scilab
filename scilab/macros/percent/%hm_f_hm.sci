function M1=%hm_f_hm(M1,M2)
// Copyright INRIA
d1=M1('dims');
d2=M2('dims')
if size(d1,'*')<>size(d2,'*') then
  error('In concatenation the number of dimensions for each component must match');
end


kd=find(d1(2:$)<>d2(2:$))
if kd<>[] then
  error('inconsistent dimensions')
end
nr=d1(1)
v=[matrix(M1('entries'),nr,prod(d1)/nr);matrix(M2('entries'),nr,prod(d2)/nr)]
M1('entries')=v(:)
M1('dims')(1)=M1('dims')(1)+M1('dims')(1)




