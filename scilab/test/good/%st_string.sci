function t=%st_string(s)
f=getfield(1,s)
ll=lines()
t=[]
for k=3:size(f,'*')
  var=f(k)
  value=s(var)
  tp=typeof(value)
  if tp=="ce" then
    t=[t;%ce_string(value)]
  elseif tp=="st" then
    t=[t;%st_string(value)]
  else
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
end
endfunction
