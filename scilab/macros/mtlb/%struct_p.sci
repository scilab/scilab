function %struct_p(s)
f=getfield(1,s)
ll=lines()
for k=2:size(f,'*')
  var=f(k)
  value=s(var)
  str=sci2exp(value,ll(1))
  txt='   '+var+': '
  if size(str,'*')==1 then
    txt=txt+str
  else
    tp=typeof(value)
    txt=txt+'['+strcat(string(size(value)),'x')+' '+tp+']'
  end
  write(%io(2),txt,'(a)')
end
endfunction
