function %st_p(s)
//matlab-like struct display
f=getfield(1,s)
ll=lines()
t=[]
Dims=double(s.dims);
multi=prod(Dims);
if multi> 1 then
t=strcat(string(double(s.dims)),'x') + '  struct';
end
for k=3:size(f,'*')
  var=f(k)
  value=s(var)
  tp=typeof(value)
  str=sci2exp(value,ll(1))
  txt='   '+var+': '
  if size(str,'*')==1 then
    txt=txt+str
  else
    tp=typeof(value)
    txt=txt+'{'+strcat(string(size(value)),'x')+' '+tp+'}'
  end
  t=[t;txt]
end
  write(%io(2),t,'(a)')
endfunction
