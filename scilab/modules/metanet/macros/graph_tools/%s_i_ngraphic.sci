function D=%s_i_ngraphic(i,d,D)
  FD=getfield(1,D);
  if d<>[] then error('Invalid assignment'),end
  FD(FD=='defaults'|FD=='display')=[];
  for f=FD(2:$)
    D(f)(:,i)=[]
  end
endfunction
