function D=%s_i_nodedata(i,d,D)
  FD=getfield(1,D);
  if d<>[] then error('Invalid assignment'),end
  for f=FD(2:$)
    D(f)(:,i)=[]
  end
endfunction
