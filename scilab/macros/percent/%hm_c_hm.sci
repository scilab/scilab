function M1=%hm_c_hm(M1,M2)
// Copyright INRIA
d1=M1('dims');
d2=M2('dims')
if size(d1,'*')<>size(d2,'*') then
  error('In concatenation the number of dimensions for each component must match');
end


kd=find(d1<>d2)
kd(find(kd==2))=[]
if kd<>[] then
  error('inconsistent dimensions')
end


v=[matrix(M1('entries'),d1(1)*d1(2),prod(d1)/(d1(1)*d1(2)));
    matrix(M2('entries'),d2(1)*d2(2),prod(d2)/(d2(1)*d2(2)))]


M1('entries')=v(:)
M1('dims')(2)=M1('dims')(2)+M1('dims')(2)






