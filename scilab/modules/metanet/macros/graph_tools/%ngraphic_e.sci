function d=%ngraphic_e(i,d)
  F=getfield(1,d);F=F(2:$)
  F(F=='defaults'|F=='display')=[];
  if i==[] then
    for f=F,d(f)=[],end
  else
    for f=F,d(f)=d(f)(:,i),end
  end
endfunction
