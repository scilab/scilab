function r=%h_e(i,h)
  if type(i)==10 then
    r=get(h,i)
  elseif type(i)==15 then
    n=lstsize(i)
    
    for k=1:n
      p=i(k)
      if type(p)==10 then
	h=get(h,p),
      elseif type(p)==1|type(p)==4|type(p)==2|type(p)==129 then
	h=h(p)
      elseif type(p)==15 then
	h=h(p(:))
      else
	error('Invalid path')
      end
    end
    r=h
  else
    error('Invalid path')
  end
endfunction
