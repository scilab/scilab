function R=%st_c_st(M1,M2)
// Copyright INRIA
  d1=M1.dims;
  d2=M2.dims;
  if size(d1,'*')<>size(d2,'*') then
    error('In concatenation the number of dimensions for each component must match');
  end
  F=getfield(1,M1)
  if or(F<>getfield(1,M2)) then
    error('field names mismatch')
  end
  kd=find(d1<>d2)

  kd(find(kd==2))=[]

  if kd<>[] then
    error('inconsistent dimensions')
  end
  newdim=d1;newdim(2)=d1(2)+d2(2)
  R=mlist(F,newdim)
  for k=3:size(F,'*') //loop on fields
    x1=getfield(k,M1),if type(x1)<>15 then x1=list(x1),end
    x2=getfield(k,M2),if type(x2)<>15 then x2=list(x2),end
    setfield(k,lstcat(x1,x2),R)
  end
endfunction
