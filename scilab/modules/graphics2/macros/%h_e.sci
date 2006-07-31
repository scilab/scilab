function r=%h_e(i,h)
h=h
  if type(i)==10 then
    r=get(h,i)
  elseif type(i)==15 then
    n=lstsize(i)
    
    for k=1:n
      p=i(k)
      if type(p)==10 then
	if type(h)==9 then
	  h=get(h,p),
	else
	  h=h(p)
	end
      elseif or(type(p)==[1 2 4 8 129]) then
	h=h(p)
      elseif type(p)==15 then
	h=h(p(:))
      else
	error('Invalid path')
      end
    end
    r=h
  elseif type(i)==1 then
    r=h(i)
  else
    error('Invalid path')
  end
  if type(r)==10 then r=stripblanks(r),end
endfunction
