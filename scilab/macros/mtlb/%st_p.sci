function %st_p(s)
//matlab-like struct display
  f=getfield(1,s)
  ll=lines()
  t=[]
  Dims=double(s.dims);
  multi=prod(Dims);
  if multi==0 then
    t='0x0 struct array with fields:'
    for k=3:size(f,'*')
      t=[t;'   '+f(k)]
    end
    write(%io(2),t,'(a)')
    return
  end
  
  if multi> 1 then
    t=strcat(string(double(s.dims)),'x') + ' struct array with fields:';
  end
  for k=3:size(f,'*')
    var=f(k)
    value=s(var)
    tp=typeof(value)
    if tp~='st' then
      sz=size(value)
      if sz(1)==1 then // This line can avoid some stacksize error when field contains a big matrix 
	str=sci2exp(value,ll(1))
      else
	str='['+strcat(string(size(value)),'x')+' '+tp+']'
      end
    else
      str="[" + strcat(string(size(value)),"x")+" struct" + "]";
    end
    txt='   '+var
    if multi<=1 then
      if size(str,'*')==1 then
	txt=txt+': '+str
      else
	tp=typeof(value)
	txt=txt+': '+'['+strcat(string(size(value)),'x')+' '+tp+']'
      end
    end
    t=[t;txt]
  end
  write(%io(2),t,'(a)')
endfunction
