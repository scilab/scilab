function R=%st_t(M)
// Copyright INRIA
  D=double(M.dims)
  if size(D,'*')>2 then
    error('Transpose not defined')
  end
  NewInd=matrix(matrix(1:prod(D),D)',1,-1)
  F=getfield(1,M)
  R=mlist(F,int32([D(2) D(1)]))
  for k=3:size(F,'*')
    v=getfield(k,M)
    setfield(k,list(v(NewInd)),R)
  end
endfunction
