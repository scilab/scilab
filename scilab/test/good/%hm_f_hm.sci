function M1=%hm_f_hm(M1,M2)
// Copyright INRIA
d1=M1.dims;
d2=M2.dims;
if size(d1,'*')<>size(d2,'*') then
  error('In concatenation the number of dimensions for each component must match');
end


kd=find(d1(2:$)<>d2(2:$))
if kd<>[] then
  error('inconsistent dimensions')
end

M1.entries=matrix([matrix(M1.entries,double(d1(1)),-1);
		   matrix(M2.entries,double(d2(1)),-1)],-1,1)
M1.dims(1)=d1(1)+d2(1);
endfunction
