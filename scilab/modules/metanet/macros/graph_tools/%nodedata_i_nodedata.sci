function D=%nodedata_i_nodedata(i,d,D)
  FD=getfield(1,D);
  Fd=getfield(1,d)
  if or(FD<>Fd) then 
    error('Incompatible node data fields')
  end
  for f=FD(2:$), D(f)(:,i)=d(f); end
endfunction
