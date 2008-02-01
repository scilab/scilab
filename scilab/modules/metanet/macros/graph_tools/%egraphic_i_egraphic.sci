function D=%egraphic_i_egraphic(i,d,D)
  FD=getfield(1,D);
  Fd=getfield(1,d)
  if or(FD<>Fd) then 
    error('Incompatible edge graphic fields')
  end
  FD(FD=='profiles'|FD=='defaults'|FD=='display')=[];
  for f=FD(2:$), D(f)(:,i)=d(f); end
endfunction
