function r=%h_e(i,h)
  if type(i)==10 then
    r=get(h,i)
  elseif type(i)==15 then
    for p=i,  h=get(h,p),end
    r=h
  else
    error('Invalid path')
  end
endfunction
