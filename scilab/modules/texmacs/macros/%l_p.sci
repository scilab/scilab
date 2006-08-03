function %l_p(s)
  str=' '
  select type(s)
  case 16 then
    f=getfield(1,s),1,-1
    first=2
    write(%io(2),'   tlist of type '+f(1)+' with fields:')
  case 17 then
    f=getfield(1,s),1,-1
    first=2
    write(%io(2),'   mlist of type '+f(1)+' with fields:')
  else
    f=[]
    first=1
    write(%io(2),'   list with entries:')
  end
  ll=lines()
  nf=size(f,'*');ns=lstsize(s)
  for k=first:max(nf,ns)
    if k<=nf then 
      var=f(k),
      value=s(var)
      label=var
    else 
      var=k,
      if k<=ns then value=s(k),else value='Undefined',end
      label=string(k)
    end
    txt='    '+label+': '
    if or(type(value)==[130 9])
      txt=txt+'['+strcat(string(size(value)),'x')+' '+typeof(value)+']'
    else
      str=sci2exp(value,ll(1))
      if size(str,'*')==1 then
	txt=txt+str
      else
	txt=txt+'['+strcat(string(size(value)),'x')+' '+typeof(value)+']'
      end
    end
    write(%io(2),txt,'(a)')
  end
endfunction
