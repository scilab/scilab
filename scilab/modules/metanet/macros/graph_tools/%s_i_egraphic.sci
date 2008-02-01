function D=%s_i_egraphic(i,d,D)
  FD=getfield(1,D);
  if d<>[] then error('Invalid assignment'),end
  FD(FD=='profiles'|FD=='defaults'|FD=='display')=[];

  for f=FD(2:$)
    D(f)(:,i)=[]
  end
endfunction
