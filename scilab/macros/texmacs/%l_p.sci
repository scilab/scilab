function %l_p(s)
  if or(type(s)==[16,17])
    f=getfield(1,s)
    first=2
  else
    f=1:lstsize(s)
    first=1
  end
  ll=lines()
  for k=first:size(f,'*')
    var=f(k)
    value=s(var)
    str=sci2exp(value,ll(1))
    txt='   '+string(var)+': '
    if size(str,'*')==1 then
      txt=txt+str
    else
      tp=typeof(value)
      txt=txt+'['+strcat(string(size(value)),'x')+' '+tp+']'
    end
    write(%io(2),txt,'(a)')
  end
endfunction
