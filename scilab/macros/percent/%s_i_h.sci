function h=%s_i_h(i,v,h)
  if type(i)==10 then
    set(h,i,v)
  elseif type(i)==15 then
    hdl=h
    for p=1:lstsize(i)-1, hdl=get(hdl,i(p)),end
    set(hdl,i($),v)
  else
    error('Invalid path')
  end
endfunction

  
