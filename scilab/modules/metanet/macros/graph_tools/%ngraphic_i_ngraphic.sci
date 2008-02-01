function D=%ngraphic_i_ngraphic(i,d,D)
  FD=getfield(1,D);
  Fd=getfield(1,d)
  if or(FD<>Fd) then 
    error('Incompatible node graphic fields')
  end
  FD(FD=='defaults'|FD=='display')=[];
  for f=FD(2:$), D(f)(:,i)=d(f); end
endfunction
